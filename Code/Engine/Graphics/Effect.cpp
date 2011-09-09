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

  if(l_szName.compare("") == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Empty name");
    SetOk(false);
    return false;
  }

  SetName(l_szName); 

  m_iTextureMask = _xmlEffect.GetIntProperty("texture_mask",0,false);

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

  m_szInstancedTechniqueName = _xmlEffect.GetPszISOProperty("instanced_technique","", false);

  m_szD3DAlphaTechniqueName  = _xmlEffect.GetPszISOProperty("alpha_technique","", false);

  SetOk(LoadEffect(_pEffectPool));
  return IsOk();
}

bool CEffect::LoadEffect(LPD3DXEFFECTPOOL _pEffectPool)
{
  LPD3DXBUFFER l_ErrorBuffer=NULL;

#ifdef _DEBUG
    HRESULT l_HR = D3DXCreateEffectFromFile(
                          RENDER_MANAGER->GetDevice(),
                          m_szFileName.c_str(),
                          NULL,
                          NULL,
                          D3DXSHADER_OPTIMIZATION_LEVEL0,
                          _pEffectPool,
                          &m_pD3DEffect,
                          &l_ErrorBuffer);
#else
    HRESULT l_HR = D3DXCreateEffectFromFile(
                          RENDER_MANAGER->GetDevice(),
                          m_szFileName.c_str(),
                          NULL,
                          NULL,
                          D3DXSHADER_OPTIMIZATION_LEVEL3,
                          _pEffectPool,
                          &m_pD3DEffect,
                          &l_ErrorBuffer);
#endif

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

  m_pD3DAlphaTechnique = m_pD3DEffect->GetTechniqueByName(m_szD3DAlphaTechniqueName.c_str());

  if(m_pD3DTechnique)
  {
    m_pD3DEffect->SetTechnique(m_pD3DTechnique);
  }

  return true;
}

bool CEffect::Reload(LPD3DXEFFECTPOOL _pEffectPool)
{
  Release();
  return LoadEffect(_pEffectPool);
}

void CEffect::Release() 
{
  CHECKED_RELEASE(m_pD3DEffect);
  m_pD3DTechnique = 0;
  m_pD3DInstancedTechnique = 0;
}


void CEffect::ActivateDefaultRendering()
{
  if(m_pD3DTechnique)
  {
    m_pD3DEffect->SetTechnique( m_pD3DTechnique );
  }
};

void CEffect::ActivateAlphaRendering()
{
  if(m_pD3DAlphaTechnique)
  {
    m_pD3DEffect->SetTechnique( m_pD3DAlphaTechnique );
  }
};

void CEffect::ActivateInstancedRendering()
{
  if(m_pD3DInstancedTechnique)
  {
    m_pD3DEffect->SetTechnique( m_pD3DInstancedTechnique );
  }
};

void CEffect::SetValue(const string& _szParameterName,int _iValue)
{
  D3DXHANDLE l_pParameter = 0;
  l_pParameter = m_pD3DEffect->GetParameterBySemantic(NULL,_szParameterName.c_str());

  if(l_pParameter)
  {
    m_pD3DEffect->SetInt(l_pParameter,_iValue);
  }
}

void CEffect::SetValue(const string& _szParameterName,float _fValue)
{
  D3DXHANDLE l_pParameter = 0;
  l_pParameter = m_pD3DEffect->GetParameterBySemantic(NULL,_szParameterName.c_str());

  if(l_pParameter)
  {
    m_pD3DEffect->SetFloat(l_pParameter,_fValue);
  }
}

void CEffect::SetValue(const string& _szParameterName,Vect3f& _vValue)
{
  float l_vVector[3] = {_vValue.x,_vValue.y,_vValue.z};

  SetValue(_szParameterName,l_vVector,3);
}

void CEffect::SetValue(const string& _szParameterName,Mat44f _matValue)
{
  D3DXHANDLE l_pParameter = 0;
  l_pParameter = m_pD3DEffect->GetParameterBySemantic(NULL,_szParameterName.c_str());

  if(l_pParameter)
  {
    m_pD3DEffect->SetMatrix(l_pParameter,&(_matValue.GetD3DXMatrix()));
  }
}

void CEffect::SetValue(const string& _szParameterName,int _iValue[], uint32 _uiSize)
{
  D3DXHANDLE l_pParameter = 0;
  l_pParameter = m_pD3DEffect->GetParameterBySemantic(NULL,_szParameterName.c_str());

  if(l_pParameter)
  {
    m_pD3DEffect->SetIntArray(l_pParameter,_iValue,_uiSize);
  }
}

void CEffect::SetValue(const string& _szParameterName,float _fValue[], uint32 _uiSize)
{
  D3DXHANDLE l_pParameter = 0;
  l_pParameter = m_pD3DEffect->GetParameterBySemantic(NULL,_szParameterName.c_str());

  if(l_pParameter)
  {
    m_pD3DEffect->SetFloatArray(l_pParameter,_fValue,_uiSize);
  }
}
