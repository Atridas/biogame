#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentOmni.h"
#include "ComponentObject3D.h"
#include "LightManager.h"
#include "OmniLight.h"
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

CComponentOmni* CComponentOmni::AddToEntity(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt)
{
  CComponentOmni *l_pComp = new CComponentOmni();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vOffsetPosition, _vColor, _fStartRangeAtt, _fEndRangeAtt))
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

CComponentOmni* CComponentOmni::AddToEntityFromResource(CGameEntity* _pEntity, const string& _szResource, const string& _szScript)
{
  CComponentOmni *l_pComp = new CComponentOmni();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->InitFromResource(_pEntity, _szResource, _szScript))
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

bool CComponentOmni::Init(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt)
{
  m_bFromResource = false;

  m_vOffset = _vOffsetPosition;

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>();

  if(!m_pObject3D)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentOmni::Init no s'ha trobat el component Object3D");
    SetOk(false);
    return IsOk();
  }

  m_pOmni = CORE->GetLightManager()->CreateOmniLight(_pEntity->GetName().append("omni"), m_pObject3D->GetPosition() + _vOffsetPosition, _vColor, _fStartRangeAtt, _fEndRangeAtt);

  if(!m_pOmni)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentOmni::Init no s'ha pogut crear la omni \"%s\"", _pEntity->GetName().c_str());
    SetOk(false);
    return IsOk();
  }

  m_pOmni->SetActive(true);

  SetOk(true);
  return IsOk();
}

bool CComponentOmni::InitFromResource(CGameEntity* _pEntity, const string& _szResource, const string& _szScript)
{
  CLight* l_pLight = CORE->GetLightManager()->GetResource(_szResource);

  if(l_pLight)
  {
    if(l_pLight->GetType() == CLight::OMNI)
    {
      m_pOmni = (COmniLight*) l_pLight;
    }
    else
    {
      LOGGER->AddNewLog(ELL_ERROR, "CComponentOmni::InitFromResource la llum \"%s\" no és del tipus omni.", _szResource);
      SetOk(false);
      return IsOk();
    }
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentOmni::InitFromResource no s'ha trobat la llum \"%s\".", _szResource);
    SetOk(false);
    return IsOk();
  }

  SetOk(true);
  return IsOk();
}

void CComponentOmni::RunScript(float _fDeltaTime)
{
  if(m_szScript != "") 
  {
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, m_szScript.c_str(), GetEntity(), _fDeltaTime);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);

      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has launched script \"%s\" has failed with error \"%s\"", 
                          GetEntity()->GetName().c_str(), m_szScript.c_str(), _TheError.what());
    }
  }
}

void CComponentOmni::Update(float _fDeltaTime)
{
  if(m_pOmni->IsActive())
  {
    if(!m_bFromResource)
    {
      m_pOmni->SetPosition(m_pObject3D->GetPosition() + m_vOffset);
    }
    else
    {
      RunScript(_fDeltaTime);
    }
  }
}

void CComponentOmni::Release()
{
  if(!m_bFromResource)
  {
    CORE->GetLightManager()->Remove(m_pOmni->GetName());
  }
  else
  {
    m_pOmni->SetActive(false);
  }
}