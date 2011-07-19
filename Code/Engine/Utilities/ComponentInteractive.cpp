#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentInteractive.h"
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

  SetOk(true);
  return IsOk();
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