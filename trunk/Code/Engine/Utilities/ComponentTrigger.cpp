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
#include "ComponentObject3D.h"
#include "Utils\Logger.h"

bool CComponentTrigger::Init(
            CGameEntity* _pEntity,
            const Vect3f& _vSize,
            const string& _szOnEnter, const string& _szOnExit,
            int _iCollisionMask)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colYELLOW);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);

  m_pPhysXActor->CreateBoxTrigger(_vSize, _iCollisionMask);

  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  m_szOnEnter = _szOnEnter;
  m_szOnExit  = _szOnExit;

  SetOk(true);
  return IsOk();
}
  
void CComponentTrigger::OnEnter(CGameEntity* _pOther)
{
  if(m_szOnEnter != "") 
  {
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, m_szOnEnter.c_str(), GetEntity(), _pOther);
    } catch(const std::exception& _TheError)
    {
      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has entered trigger \"%s\" and script \"%s\" has failed with error \"%s\"", 
                          _pOther->GetName().c_str(), GetEntity()->GetName().c_str(), m_szOnEnter.c_str(), _TheError.what());
    }
  }
}

void CComponentTrigger::OnExit (CGameEntity* _pOther)
{
  if(m_szOnExit != "")
  {
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, m_szOnExit.c_str(), GetEntity(), _pOther);
    } catch(const std::exception& _TheError)
    {
      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has exited trigger \"%s\" and script \"%s\" has failed with error \"%s\"", 
                            _pOther->GetName().c_str(), GetEntity()->GetName().c_str(), m_szOnEnter.c_str(), _TheError.what());
    }
  }
}