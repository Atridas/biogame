#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentCollisionReport.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "PhysicActor.h"
#include "ScriptManager.h"
#include "Core.h"

#undef min
#undef max
#include "NxPhysics.h"

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


CComponentCollisionReport* CComponentCollisionReport::AddToEntity(
                  CGameEntity* _pEntity,
                  const string& _szOnStartTouch,
                  const string& _szOnTouch,
                  const string& _szOnEndTouch,
                  const string& _szOnStartTouchForceThreshold,
                  const string& _szOnTouchForceThreshold,
                  const string& _szOnEndTouchForceThreshold,
                  const float& _fThreshold)
{
  CComponentCollisionReport *l_pComp = new CComponentCollisionReport();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szOnStartTouch, _szOnTouch, _szOnEndTouch, _szOnStartTouchForceThreshold, _szOnTouchForceThreshold, _szOnEndTouchForceThreshold,_fThreshold))
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

bool CComponentCollisionReport::Init(CGameEntity* _pEntity,
                                const string& _szOnStartTouch,
                                const string& _szOnTouch,
                                const string& _szOnEndTouch,
                                const string& _szOnStartTouchForceThreshold,
                                const string& _szOnTouchForceThreshold,
                                const string& _szOnEndTouchForceThreshold,
                                const float& _fThreshold)
{
  m_szOnStartTouch = _szOnStartTouch;
  m_szOnTouch = _szOnTouch;
  m_szOnEndTouch = _szOnEndTouch;
  m_szOnStartTouchForceThreshold = _szOnStartTouchForceThreshold;
  m_szOnTouchForceThreshold = _szOnTouchForceThreshold;
  m_szOnEndTouchForceThreshold = _szOnEndTouchForceThreshold;

  m_fThreshold = _fThreshold;
  m_uiFlags = 0;

  if(_szOnStartTouch.compare("") != 0)
  {
    m_uiFlags |= NX_NOTIFY_ON_START_TOUCH;
  }

  if(m_szOnTouch.compare("") != 0)
  {
    m_uiFlags |= NX_NOTIFY_ON_TOUCH;
  }

  if(m_szOnEndTouch.compare("") != 0)
  {
    m_uiFlags |= NX_NOTIFY_ON_END_TOUCH;
  }

  if(m_szOnStartTouchForceThreshold.compare("") != 0)
  {
    m_uiFlags |= NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD;
  }

  if(m_szOnTouchForceThreshold.compare("") != 0)
  {
    m_uiFlags |= NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD;
  }

  if(m_szOnEndTouchForceThreshold.compare("") != 0)
  {
    m_uiFlags |= NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD;
  }

  CComponentRagdoll* l_pRagdoll = _pEntity->GetComponent<CComponentRagdoll>();
  if(l_pRagdoll)
  {
    CPhysxSkeleton* l_pSkeleton = l_pRagdoll->GetRagdoll();

    l_pSkeleton->SetContactReportFlags(m_uiFlags);
    l_pSkeleton->SetContactReportThreshold(m_fThreshold);
  }

  CComponentPhysXBox* l_pBox = _pEntity->GetComponent<CComponentPhysXBox>();
  if(l_pBox)
  {
    l_pBox->GetActor()->SetContactReportFlags(m_uiFlags);
    l_pBox->GetActor()->SetContactReportThreshold(m_fThreshold);
  }

  CComponentPhysXMesh* l_pMesh = _pEntity->GetComponent<CComponentPhysXMesh>();
  if(l_pMesh)
  {
    l_pMesh->GetActor()->SetContactReportFlags(m_uiFlags);
    l_pMesh->GetActor()->SetContactReportThreshold(m_fThreshold);
  }

  SetOk(true);
  return IsOk();
}

void CComponentCollisionReport::OnStartTouch(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  if(GetEntity() != _pOther) 
  {
    ExecuteLua(m_szOnStartTouch,_pOther);
  }
}

void CComponentCollisionReport::OnTouch(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  if(GetEntity() != _pOther) 
  {
    ExecuteLua(m_szOnTouch,_pOther);
  }
}

void CComponentCollisionReport::OnEndTouch(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  if(GetEntity() != _pOther) 
  {
    ExecuteLua(m_szOnEndTouch,_pOther);
  }
}

void CComponentCollisionReport::OnStartTouchForceThreshold(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  if(GetEntity() != _pOther) 
  {
    ExecuteLua(m_szOnStartTouchForceThreshold,_pOther);
  }
}

void CComponentCollisionReport::OnTouchForceThreshold(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  if(GetEntity() != _pOther) 
  {
    ExecuteLua(m_szOnTouchForceThreshold,_pOther);
  }
}

void CComponentCollisionReport::OnEndTouchForceThreshold(CGameEntity* _pOther)
{
  if(!IsActive())
    return;

  if(GetEntity() != _pOther) 
  {
    ExecuteLua(m_szOnEndTouchForceThreshold,_pOther);
  }
}

void CComponentCollisionReport::ExecuteLua(const string& _szLuaCode, CGameEntity* _pOther)
{
  if(_szLuaCode == "") 
  {
    LOGGER->AddNewLog(ELL_ERROR,"CComponentCollisionReport::ExecuteLua no code to execute");
    return;
  }

  CScriptManager* m_pSM = CORE->GetScriptManager();

  lua_State *l_pLUA = m_pSM->GetLuaState();

  try {
    luabind::call_function<void>(l_pLUA, _szLuaCode.c_str(), GetEntity(), _pOther);
  } catch(const luabind::error& _TheError)
  {
    CScriptManager::PrintError(_TheError);

    LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has colided with \"%s\" and script \"%s\" has failed with error \"%s\"", 
                        _pOther->GetName().c_str(), GetEntity()->GetName().c_str(), _szLuaCode.c_str(), _TheError.what());
  }
}
