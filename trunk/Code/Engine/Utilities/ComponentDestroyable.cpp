#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentDestroyable.h"
#include "ComponentVida.h"
#include "ComponentRenderableObject.h"
#include "PhysicsManager.h"
#include "ScriptManager.h"
#include "Core.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Utils\MemLeaks.h"
#include "Utils\Logger.h"


CComponentDestroyable* CComponentDestroyable::AddToEntity(CGameEntity* _pEntity, const string& _szAction, const string& _szResource)
{
  CComponentDestroyable *l_pComp = new CComponentDestroyable();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szAction, _szResource))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentDestroyable::Init(CGameEntity* _pEntity, const string& _szAction, const string& _szResource)
{
  //és necessari un component de vida
  assert(_pEntity->GetComponent<CComponentVida>(ECT_VIDA));

  m_szAction = _szAction;
  m_szResource = _szResource;

  m_bDestroyed = false;

  SetOk(true);
  return IsOk();
}

void CComponentDestroyable::CallAction()
{
  if(m_szAction != "") 
  {
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, m_szAction.c_str(), GetEntity());
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);

      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has launched script \"%s\" has failed with error \"%s\"", 
                          GetEntity()->GetName().c_str(), m_szAction.c_str(), _TheError.what());
    }
  }
}

void CComponentDestroyable::ReceiveEvent(const SEvent& _Event)
{
  CGameEntity* l_pEntity = GetEntity();
  if(_Event.Receiver == l_pEntity->GetGUID())
  {
    if(_Event.Msg == SEvent::MORIR)
    {
      //Script de destrucció
      m_bDestroyed = true;
      CallAction();

      //Canvi de mesh
      CComponentRenderableObject* l_pRO = l_pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);

      if(l_pRO)
      {
        if(m_szResource != "")
        {
          if(!l_pRO->ChangeInstance(m_szResource))
            LOGGER->AddNewLog(ELL_WARNING, "CComponentDestroyable::No s'ha pogut crear l'objecte destruït.");
        }else
        {
          l_pRO->m_bRemoveRenderableObject = true;
          ENTITY_MANAGER->RemoveEntity(l_pEntity);
        }
      }
    }
  }
}

void CComponentDestroyable::Explosion(Vect3f _vPos,float _fRadius)
{
    vector<CPhysicUserData*> l_vImpactObjects;
    CPhysicsManager *l_pPM = PHYSICS_MANAGER;

    l_pPM->OverlapSphereActor(_fRadius,_vPos,l_vImpactObjects,l_pPM->GetCollisionMask(ECG_FORCE));

    vector<CPhysicUserData*>::iterator l_itUserData;
    vector<CPhysicUserData*>::iterator l_itUserDataEnd = l_vImpactObjects.end();

    set<CGameEntity*> l_vImpactEntities;

    for(l_itUserData = l_vImpactObjects.begin(); l_itUserData != l_itUserDataEnd; ++l_itUserData)
    {
      CPhysicUserData* l_pUserData = *l_itUserData;
      l_vImpactEntities.insert(l_pUserData->GetEntity());
    }

    set<CGameEntity*>::iterator l_itEntity;
    set<CGameEntity*>::iterator l_itEntityEnd = l_vImpactEntities.end();

    //missatge de força
    SEvent l_impacte;
    l_impacte.Msg = SEvent::REBRE_FORCE;
    l_impacte.Info[0].Type = SEventInfo::FLOAT;
    l_impacte.Info[0].f    = 100;
    l_impacte.Sender = GetEntity()->GetGUID();


    for(l_itEntity = l_vImpactEntities.begin(); l_itEntity != l_itEntityEnd; ++l_itEntity)
    {
      CGameEntity* l_pEntity = *l_itEntity;

      if(GetEntity() != l_pEntity)
      {
        l_impacte.Receiver = l_pEntity->GetGUID();

        ENTITY_MANAGER->SendEvent(l_impacte);
      }
    }

}