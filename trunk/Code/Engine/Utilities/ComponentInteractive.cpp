#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentInteractive.h"
#include "ScriptManager.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "ComponentObject3D.h"
#include "ComponentPlayerController.h"
#include "ComponentEmiter.h"
#include "EmiterInstance.h"

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


CComponentInteractive* CComponentInteractive::AddToEntity(CGameEntity* _pEntity, const string& _szAction)
{
  CComponentInteractive *l_pComp = new CComponentInteractive();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szAction))
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

bool CComponentInteractive::Init(CGameEntity* _pEntity, const string& _szAction)
{
  m_szAction = _szAction;

  m_bBillboardActive = false;
  m_fLastUpdate = 0;
  m_pObject3D = 0;
  m_fBillboardYOffset = 0;


  SetOk(true);
  return IsOk();
}

void CComponentInteractive::Release()
{
  if(m_bBillboardActive)
  {
    GetEntity()->DeleteComponent(CBaseComponent::ECT_EMITER);
    m_bBillboardActive = false;
  }
}

void CComponentInteractive::CallAction(int _iCaller)
{
  if(m_szAction != "") 
  {
    CGameEntity* l_pPlayer = ENTITY_MANAGER->GetEntity(_iCaller);
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, m_szAction.c_str(), GetEntity(), l_pPlayer);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);

      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has used item \"%s\" and script \"%s\" has failed with error \"%s\"", 
                          l_pPlayer->GetName().c_str(), GetEntity()->GetName().c_str(), m_szAction.c_str(), _TheError.what());
    }
  }
}

void CComponentInteractive::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Receiver == GetEntity()->GetGUID())
  {
    if(_Event.Msg == SEvent::INTERACCIO)
    {
      CallAction(_Event.Sender);
    }
  }
}

void CComponentInteractive::Update(float _fDeltaTime)
{
  m_fLastUpdate += _fDeltaTime;
  if(m_fLastUpdate < UPDATE_BILLBOARD_SECONDS)
    return;

  if(!m_pObject3D)
  {
    m_pObject3D = GetEntity()->GetComponent<CComponentObject3D>();
    if(!m_pObject3D)
      return;
  }

  
  CPhysicsManager *l_pPM = PHYSICS_MANAGER;
  vector<CPhysicUserData*> l_vImpactObjects;

  
  //esfera d'us
  l_pPM->OverlapSphereActor( RADI_ESFERA_INTERACTUABLES            //radiusSphere
                            ,m_pObject3D->GetCenterPosition()      //posSphere
                            ,l_vImpactObjects,                     //impactObjects
                            l_pPM->GetCollisionMask(ECG_ESCENARI));//collision_mask

  
  bool l_bPlayerFound = false;


  vector<CPhysicUserData*>::iterator l_itInit = l_vImpactObjects.begin();
  vector<CPhysicUserData*>::iterator l_itEnd  = l_vImpactObjects.end();
  vector<CPhysicUserData*>::iterator l_itIt;
  
  for(l_itIt = l_itInit; l_itIt < l_itEnd; ++l_itIt)
  {
    CGameEntity* l_pEntity = (*l_itIt)->GetEntity();
    if(l_pEntity && l_pEntity->GetComponent<CComponentPlayerController>())
    {
      l_bPlayerFound = true;
      break;
    }
  }

  if(l_bPlayerFound && !m_bBillboardActive)
  {
    CComponentEmiter *l_pCE = CComponentEmiter::AddToEntity(GetEntity(), "Objecte Interactuable", Vect3f(.5f,.5f,.5f));
    CObject3D l_Obj3D;
    l_Obj3D.SetPosition(Vect3f(0.f, m_fBillboardYOffset, 0.f));
    l_pCE->GetEmiter()->SetOffset(l_Obj3D);
    m_bBillboardActive = true;
  }
  else if(!l_bPlayerFound && m_bBillboardActive)
  {
    GetEntity()->DeleteComponent(CBaseComponent::ECT_EMITER);
    m_bBillboardActive = false;
  }
}
