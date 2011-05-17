#include "Effect.h"

#include "Light.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <Core.h>
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>

/*void CEffect::SetNullParameters() 
{
  m_pWorldMatrixParameter =
  m_pViewMatrixParameter =
  m_pProjectionMatrixParameter =
  m_pWorldViewMatrixParameter =
  m_pViewProjectionMatrixParameter =
  m_pWorldViewProjectionMatrixParameter =
  m_pViewToLightProjectionMatrixParameter =
  m_pAmbientLight =
  m_pLightsEnabledParameter =
  m_pLightsTypeParameter =
  m_pLightsPositionParameter =
  m_pLightsDirectionParameter =
  m_pLightsAngleParameter =
  m_pLightsColorParameter =
  m_pLightsFallOffParameter =
  m_pLightsStartRangeAttenuationParameter =
  m_pLightsEndRangeAttenuationParameter =
  m_pCameraPositionParameter =
  m_pBonesParameter =
  m_pGlowActive =
  m_pTextureWidth =
  m_pTextureHeight =
  m_pPoissonBlurKernelParameter =
  m_pTimeParameter = 0;
}*/



/*void CEffect::GetParameterBySemantic(const string& _szSemanticName, D3DXHANDLE& _pHandle)
{
  _pHandle=m_pD3DEffect->GetParameterBySemantic(NULL,_szSemanticName.c_str());
  if(_pHandle==NULL)
    LOGGER->AddNewLog(ELL_WARNING,"CEffect::GetParameterBySemantic Parameter by semantic '%s' wasn't found on effect '%s'", _szSemanticName.c_str(),m_szFileName.c_str());
}*/

/*bool CEffect::SetLights(size_t _iNumOfLights)
{
  if(_iNumOfLights > MAX_LIGHTS_BY_SHADER)
  {
    assert(false);
    LOGGER->AddNewLog(ELL_ERROR, "CEffect::SetLights Es volen iniciar %d llums, però com a màxim n'hi ha %d", _iNumOfLights, MAX_LIGHTS_BY_SHADER);
    return false;
  }
  CLightManager* l_pLightManager = CORE->GetLightManager();

  const Vect3f l_Ambient = l_pLightManager->GetAmbientLight();
  m_aAmbientLight[0] = l_Ambient.x;
  m_aAmbientLight[1] = l_Ambient.y;
  m_aAmbientLight[2] = l_Ambient.z;

  vector<CLight*> l_vLights = l_pLightManager->GetLights(_iNumOfLights);

  for(size_t i = 0; i < _iNumOfLights; i++)
  {
    if(l_vLights.size() <= i)
    {
      m_aLightsEnabled[i] = false;
      continue;
    }

    CLight* l_pLight = l_vLights[i];
    if(!l_pLight)
    {
      assert(false);
      LOGGER->AddNewLog(ELL_ERROR, "CEffect::SetLights Light manager no té %d llums.", i);
      return false;
    }
    m_aLightsEnabled[i] = l_pLight->IsActive();
    m_aLightsType[i] = l_pLight->GetType();
    
    m_aLightsStartRangeAttenuation[i] = l_pLight->GetStartRangeAttenuation();
    m_aLightsEndRangeAttenuation[i]   = l_pLight->GetEndRangeAttenuation();
    m_aLightsStartRangeAttenuation[i] *= m_aLightsStartRangeAttenuation[i]; // Make it square
    m_aLightsEndRangeAttenuation[i]   *= m_aLightsEndRangeAttenuation[i];   // Make it square
    m_aLightsPosition[i]              = l_pLight->GetPosition();
    m_aLightsColor[i]                 = l_pLight->GetColor();

    m_aShadowsEnabled[i] = l_pLight->GetRenderShadows();

    if(m_aLightsType[i] == CLight::DIRECTIONAL || m_aLightsType[i] == CLight::SPOT)
    {
      CDirectionalLight* l_pDirectional = (CDirectionalLight*) l_pLight;
      m_aLightsDirection[i] = l_pDirectional->GetDirection();

      if(m_aLightsType[i] == CLight::SPOT)
      {
        CSpotLight* l_pSpot = (CSpotLight*) l_pDirectional;
        m_aLightsAngle[i]   = l_pSpot->GetAngle() * FLOAT_PI_VALUE / 180.0f;
        m_aLightsFallOff[i] = l_pSpot->GetFallOff() * FLOAT_PI_VALUE / 180.0f;

        m_aLightsAngle[i]   *= 0.5f; // agafar només la meitat
        m_aLightsFallOff[i] *= 0.5f;

        m_aLightsAngle[i]   = cosf(m_aLightsAngle[i]); // volem el cosinus, que calcular-lo al shader val un iogurt.
        m_aLightsFallOff[i] = cosf(m_aLightsFallOff[i]);
      }
    }
  }

  for(int i = _iNumOfLights; i < MAX_LIGHTS_BY_SHADER; i++)
  {
    m_aLightsEnabled[i] = false;
  }
  return true;
}*/

bool CEffect::Init(const CXMLTreeNode& _xmlEffect, LPD3DXEFFECTPOOL _pEffectPool)
{
  SetOk(true);
  //SetNullParameters();
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
                          D3DXSHADER_USE_LEGACY_D3DX9_31_DLL,
                          m_pEffectPool,
                          &m_pD3DEffect,
                          &l_ErrorBuffer);
  if(l_ErrorBuffer)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error creating effect '%s':\n%s", m_szFileName.c_str(), l_ErrorBuffer->GetBufferPointer());
    CHECKED_RELEASE(l_ErrorBuffer);
    SetOk(false);
    return false;
  }

  if(l_HR != D3D_OK)
  {
    if(l_HR == D3DERR_INVALIDCALL)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error crida invàlida");
      SetOk(false);
      return false;
    }

    else if(l_HR == D3DXERR_INVALIDDATA)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error invalid data");
      SetOk(false);
      return false;
    }

    else if(l_HR == E_OUTOFMEMORY)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CEffect::Init Error out of memory");
      SetOk(false);
      return false;
    }
  }

  m_pD3DTechnique = m_pD3DEffect->GetTechniqueByName(m_szTechniqueName.c_str());

  if(m_szInstancedTechniqueName.compare("") == 0)
  {
    m_pD3DInstancedTechnique = m_pD3DEffect->GetTechniqueByName(m_szInstancedTechniqueName.c_str());
  }else{
    m_pD3DInstancedTechnique = 0;
  }

  if(m_pD3DTechnique)
  {
    m_pD3DEffect->SetTechnique(m_pD3DTechnique);
  }

  return IsOk();
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

/*bool CEffect::InitParameters()
{
  //matrixes
  GetParameterBySemantic("World", m_pWorldMatrixParameter);
  GetParameterBySemantic("View", m_pViewMatrixParameter);
  GetParameterBySemantic("Projection", m_pProjectionMatrixParameter);
  GetParameterBySemantic("WorldView", m_pWorldViewMatrixParameter);
  GetParameterBySemantic("ViewProjection", m_pViewProjectionMatrixParameter);
  GetParameterBySemantic("WorldViewProjection", m_pWorldViewProjectionMatrixParameter);
  GetParameterBySemantic("ViewToLightProjection", m_pViewToLightProjectionMatrixParameter);

  //lights
  GetParameterBySemantic("AmbientLight", m_pAmbientLight);
  GetParameterBySemantic("LightsEnabled", m_pLightsEnabledParameter);
  GetParameterBySemantic("LightsType", m_pLightsTypeParameter);
  GetParameterBySemantic("LightsPosition", m_pLightsPositionParameter);
  GetParameterBySemantic("LightsDirection", m_pLightsDirectionParameter);
  GetParameterBySemantic("LightsAngleCos", m_pLightsAngleParameter);
  GetParameterBySemantic("LightsFallOffCos", m_pLightsFallOffParameter);
  GetParameterBySemantic("LightsColor", m_pLightsColorParameter);
  GetParameterBySemantic("LightsStartRangeSQ", m_pLightsStartRangeAttenuationParameter);
  GetParameterBySemantic("LightsEndRangeSQ", m_pLightsEndRangeAttenuationParameter);
  GetParameterBySemantic("ShadowEnabled", m_pShadowsEnabledParameter);

  //Altres
  GetParameterBySemantic("CameraPosition", m_pCameraPositionParameter);
  GetParameterBySemantic("Bones", m_pBonesParameter);
  GetParameterBySemantic("Time", m_pTimeParameter);
  GetParameterBySemantic("GlowActive", m_pGlowActive);
  GetParameterBySemantic("TextureWidth", m_pTextureWidth);
  GetParameterBySemantic("TextureHeight", m_pTextureHeight);
  GetParameterBySemantic("PoissonBlurKernel", m_pPoissonBlurKernelParameter);

  return true;
}*/

/*
D3DXHANDLE CEffect::GetTechniqueByName(const string& _szTechniqueName) const 
{
  return m_pD3DEffect->GetTechniqueByName(_szTechniqueName.c_str());
}
*/