#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentDestroyable.h"
#include "ComponentVida.h"
#include "ComponentRenderableObject.h"
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
        if(!l_pRO->ChangeInstance(m_szResource))
          LOGGER->AddNewLog(ELL_WARNING, "CComponentDestroyable::No s'ha pogut crear l'objecte destruït.");
    }
  }
}