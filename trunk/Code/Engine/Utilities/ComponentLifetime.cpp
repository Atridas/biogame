#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentLifetime.h"
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


CComponentLifetime* CComponentLifetime::AddToEntity(CGameEntity *_pEntity, float _fTime, const string& _szScript)
{
  CComponentLifetime *l_pComp = new CComponentLifetime();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fTime, _szScript))
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

bool CComponentLifetime::Init(CGameEntity *_pEntity, float _fTime, const string& _szScript)
{
  assert(_fTime >= 0);

  m_fTargetTime = _fTime;

  m_szScript = _szScript;

  m_bKillEntity = true;
  m_bTriggered  = false;

  SetOk(true);
  return IsOk();
}

void CComponentLifetime::Update(float _fDeltaTime) 
{
  m_fTime += _fDeltaTime;
  if(m_fTime >= m_fTargetTime)
  {
    bool l_bKillEntity = m_bKillEntity;
    if(m_szScript != "" && !m_bTriggered) 
    {
      CScriptManager* m_pSM = CORE->GetScriptManager();

      lua_State *l_pLUA = m_pSM->GetLuaState();

      try {
        luabind::call_function<void>(l_pLUA, m_szScript.c_str(), GetEntity());
      } catch(const luabind::error& _TheError)
      {
        CScriptManager::PrintError(_TheError);

        LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has launched script \"%s\" has failed with error \"%s\"", 
                            GetEntity()->GetName().c_str(), m_szScript.c_str(), _TheError.what());
      }

      m_bTriggered = true;
    }

    if(l_bKillEntity)
      ENTITY_MANAGER->RemoveEntity( GetEntity() );
  }
}
