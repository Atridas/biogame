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


CComponentTrigger* CComponentTrigger::AddToEntity(
            CGameEntity* _pEntity,
            const Vect3f& _vSize,
            const string& _szOnEnter, const string& _szOnExit,
            int _iCollisionGroup)
{
  CComponentTrigger *l_pComp = new CComponentTrigger();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vSize, _szOnEnter, _szOnExit, _iCollisionGroup))
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

bool CComponentTrigger::Init(
            CGameEntity* _pEntity,
            const Vect3f& _vSize,
            const string& _szOnEnter, const string& _szOnExit,
            int _iCollisionGroup)
{
  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colYELLOW);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);

  m_pPhysXActor->CreateBoxTrigger(_vSize * .5f, _iCollisionGroup);

  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  m_szOnEnter = _szOnEnter;
  m_szOnExit  = _szOnExit;

  SetOk(true);
  return IsOk();
}
  
void CComponentTrigger::OnEnter(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  ExecuteLua(m_szOnEnter,_pOther);
}

void CComponentTrigger::OnExit (CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  ExecuteLua(m_szOnExit,_pOther);
}

void CComponentTrigger::ExecuteLua(const string& _szLuaCode, CGameEntity* _pOther)
{
  if(_szLuaCode != "")
  {
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, _szLuaCode.c_str(), GetEntity(), _pOther);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);

      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has entered trigger \"%s\" and script \"%s\" has failed with error \"%s\"", 
                          _pOther->GetName().c_str(), GetEntity()->GetName().c_str(), _szLuaCode.c_str(), _TheError.what());
    }
  }
}