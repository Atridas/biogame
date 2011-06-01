#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentTrigger.h"
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

#include "PhysicActor.h"
#include "PhysicsManager.h"

#include "Utils\MemLeaks.h"
#include "ComponentPhysXActor.h"


void CComponentTrigger::Init(CGameEntity* _pEntity, const string& _szOnEnter, const string& _szOnExit)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  CComponentPhysXActor* m_pComponentPhysXActor = _pEntity->GetComponent<CComponentPhysXActor>(ECT_PHISX_ACTOR);
  assert(m_pComponentPhysXActor);

  m_pComponentPhysXActor->GetActor()->ActivateAllTriggers();
  m_pComponentPhysXActor->GetUserData()->SetColor(colYELLOW);

  m_szOnEnter = _szOnEnter;
  m_szOnExit  = _szOnExit;
}
  
void CComponentTrigger::OnEnter(CGameEntity* _pOther)
{
  CScriptManager* m_pSM = CORE->GetScriptManager();

  lua_State *l_pLUA = m_pSM->GetLuaState();

  luabind::call_function<void>(l_pLUA, m_szOnEnter.c_str(), GetEntity(), _pOther);
}

void CComponentTrigger::OnExit (CGameEntity* _pOther)
{
  CScriptManager* m_pSM = CORE->GetScriptManager();

  lua_State *l_pLUA = m_pSM->GetLuaState();

  luabind::call_function<void>(l_pLUA, m_szOnExit.c_str(), GetEntity(), _pOther);
}