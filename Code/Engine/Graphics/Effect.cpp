#include "Effect.h"

#include "Light.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <Core.h>

CEffect::CEffect(void):
  m_szFileName(""),
  m_pD3DEffect(0),
  m_pWorldMatrixParameter(0),
  m_pViewMatrixParameter(0),
  m_pProjectionMatrixParameter(0),
  m_pWorldViewMatrixParameter(0),
  m_pViewProjectionMatrixParameter(0),
  m_pWorldViewProjectionMatrixParameter(0),
  m_pViewToLightProjectionMatrixParameter(0),
  m_pLightEnabledParameter(0),
  m_pLightsTypeParameter(0),
  m_pLightsPositionParameter(0),
  m_pLightsDirectionParameter(0),
  m_pLightsAngleParameter(0),
  m_pLightsColorParameter(0),
  m_pLightsFallOffParameter(0),
  m_pLightsStartRangeAttenuationParameter(0),
  m_pLightsEndRangeAttenuationParameter(0),
  m_pCameraPositionParameter(0),
  m_pBonesParameter(0),
  m_pTimeParameter(0)
{
  for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
  {
    m_aLightsEnabled[i] = false;
    m_aLightsType[i] = 0;
    m_aLightsAngle[i] = 0;
    m_aLightsFallOff[i] = 0;
    m_aLightsStartRangeAttenuation[i] = 0;
    m_aLightsEndRangeAttenuation[i] = 0;
    m_aLightsPosition[i] = Vect3f(0.0f);
    m_aLightsDirection[i] = Vect3f(0.0f);
    m_aLightsColor[i] = colWHITE;
  }
}

void CEffect::Release() 
{
  CHECKED_RELEASE(m_pD3DEffect);
  SetNullParameters();
}

void CEffect::SetNullParameters() 
{
  m_pWorldMatrixParameter = 0;
  m_pViewMatrixParameter = 0;
  m_pProjectionMatrixParameter = 0;
  m_pWorldViewMatrixParameter = 0;
  m_pViewProjectionMatrixParameter = 0;
  m_pWorldViewProjectionMatrixParameter = 0;
  m_pViewToLightProjectionMatrixParameter = 0;
  m_pLightEnabledParameter = 0;
  m_pLightsTypeParameter = 0;
  m_pLightsPositionParameter = 0;
  m_pLightsDirectionParameter = 0;
  m_pLightsAngleParameter = 0;
  m_pLightsColorParameter = 0;
  m_pLightsFallOffParameter = 0;
  m_pLightsStartRangeAttenuationParameter = 0;
  m_pLightsEndRangeAttenuationParameter = 0;
  m_pCameraPositionParameter = 0;
  m_pBonesParameter = 0;
  m_pTimeParameter = 0;
}
void CEffect::GetParameterBySemantic(const string& _szSemanticName, D3DXHANDLE& _pHandle)
{
  _pHandle=m_pD3DEffect->GetParameterBySemantic(NULL,_szSemanticName.c_str());
  if(_pHandle==NULL)
    LOGGER->AddNewLog(ELL_WARNING,"CEffect::GetParameterBySemantic Parameter by semantic '%s' wasn't found on effect '%s'", _szSemanticName.c_str(),m_szFileName.c_str());
}

bool CEffect::LoadEffect()
{
 //TODO 
  return 0;
}

bool CEffect::SetLights(size_t _iNumOfLights){
  if(_iNumOfLights > MAX_LIGHTS_BY_SHADER)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CEffect::SetLights Es volen iniciar %d llums, però com a màxim n'hi ha %d", _iNumOfLights, MAX_LIGHTS_BY_SHADER);
    return false;
  }
  CLightManager* l_pLightManager = CORE->GetLightManager();
  for(size_t i = 0; i < _iNumOfLights; i++)
  {
    CLight* l_pLight = l_pLightManager->GetLight(i);
    if(!l_pLight)
    {
      LOGGER->AddNewLog(ELL_ERROR, "CEffect::SetLights Light manager no té %d llums.", i);
      return false;
    }
    m_aLightsEnabled[i] = true;
    m_aLightsType[i] = l_pLight->GetType();
    
    m_aLightsStartRangeAttenuation[i] = l_pLight->GetStartRangeAttenuation();
    m_aLightsEndRangeAttenuation[i]   = l_pLight->GetEndRangeAttenuation();
    m_aLightsPosition[i]              = l_pLight->GetPosition();
    m_aLightsColor[i]                 = l_pLight->GetColor();

    if(m_aLightsType[i] == CLight::DIRECTIONAL || m_aLightsType[i] == CLight::SPOT)
    {
      CDirectionalLight* l_pDirectional = (CDirectionalLight*) l_pLight;
      m_aLightsDirection[i] = l_pDirectional->GetDirection();

      if(m_aLightsType[i] == CLight::SPOT)
      {
        CSpotLight* l_pSpot = (CSpotLight*) l_pDirectional;
        m_aLightsAngle[i]   = l_pSpot->GetAngle();
        m_aLightsFallOff[i] = l_pSpot->GetFallOff();
      }
    }
  }

  for(int i = _iNumOfLights; i < MAX_LIGHTS_BY_SHADER; i++)
  {
    m_aLightsEnabled[i] = false;
  }
  return 0;
}

bool CEffect::Load(const string& _szFileName){
  SetNullParameters();
  m_szFileName = _szFileName;
  LoadEffect();

  return 0;
}

bool CEffect::Reload()
{
  Release();
  return Load(m_szFileName);
}

//DirectX Methods Interface
D3DXHANDLE CEffect::GetTechniqueByName(const string& _szTechniqueName)
{
 //TODO 
  return 0;
}