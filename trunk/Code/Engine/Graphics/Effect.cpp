#include "Effect.h"

#include "Light.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <Core.h>
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>

bool CEffect::Init(const CXMLTreeNode& _xmlEffect, LPD3DXEFFECTPOOL _pEffectPool)
{
  SetOk(true);

  string l_szName = _xmlEffect.GetPszISOProperty("name","");

  m_pEffectPool = _pEffectPool;

  if(l_szName.compare("") == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Empty name");
    SetOk(false);
    return false;
  }

  SetName(l_szName); 

  m_szFileName = _xmlEffect.GetPszISOProperty("file","");

  if(m_szFileName.compare("") == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Empty effect path");
    SetOk(false);
    return false;
  }

  m_szTechniqueName = _xmlEffect.GetPszISOProperty("technique","");

  if(m_szTechniqueName.compare("") == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Empty technique name");
    SetOk(false);
    return false;
  }

  m_szInstancedTechniqueName = _xmlEffect.GetPszISOProperty("instanced_technique","");

  SetOk(LoadEffect());
  return IsOk();
}

bool CEffect::LoadEffect()
{
  LPD3DXBUFFER l_ErrorBuffer=NULL;
  HRESULT l_HR = D3DXCreateEffectFromFile(
                          RENDER_MANAGER->GetDevice(),
                          m_szFileName.c_str(),
                          NULL,
                          NULL,
                          NULL,
                          m_pEffectPool,
                          &m_pD3DEffect,
                          &l_ErrorBuffer);
  if(l_ErrorBuffer)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error creating effect '%s':\n%s", m_szFileName.c_str(), l_ErrorBuffer->GetBufferPointer());
    CHECKED_RELEASE(l_ErrorBuffer);
  }

  if(l_HR != D3D_OK)
  {
    if(l_HR == D3DERR_INVALIDCALL)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error crida invàlida");
    }
    else if(l_HR == D3DXERR_INVALIDDATA)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error invalid data");
    }
    else if(l_HR == E_OUTOFMEMORY)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error out of memory");
    }

    return false;
  }

  m_pD3DTechnique = m_pD3DEffect->GetTechniqueByName(m_szTechniqueName.c_str());

  m_pD3DInstancedTechnique = m_pD3DEffect->GetTechniqueByName(m_szInstancedTechniqueName.c_str());

  if(m_pD3DTechnique)
  {
    m_pD3DEffect->SetTechnique(m_pD3DTechnique);
  }

  return true;
}

bool CEffect::Reload()
{
  Release();
  return LoadEffect();
}

void CEffect::Release() 
{
  CHECKED_RELEASE(m_pD3DEffect);
  m_pD3DTechnique = 0;
  m_pD3DInstancedTechnique = 0;
  m_pEffectPool = 0;
}
