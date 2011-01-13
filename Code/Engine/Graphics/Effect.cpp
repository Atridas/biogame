#include "Effect.h"


CEffect::CEffect(void):
  m_szFileName(""),
  m_pEffect(0),
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
    m_aLightsEnabled[i] = 0;
    m_aLightsType[i] = 0;
    m_aLightsAngle[i] = 0;
    m_aLightsFallOff[i] = 0;
    m_aLightsStartRangeAttenuation[i] = 0;
    m_aLightsEndRangeAttenuation[i] = 0;
    m_aLightsPosition[i] = Vect3f(0.0f);
    m_aLightsDirection[i] = Vect3f(0.0f);
    m_aLightsColor[i] = Vect3f(0.0f);
  }
}

void CEffect::Release() {
 //TODO 
  
}

void CEffect::SetNullParameters(){
 //TODO 
  
}
void CEffect::GetParameterBySemantic(const string& _szSemanticName, D3DXHANDLE& _pHandle){
 //TODO 
  
}
bool CEffect::LoadEffect(){
 //TODO 
  return 0;
}
bool CEffect::SetLights(size_t _iNumOfLights){
 //TODO 
  return 0;
}
bool CEffect::Load(const string& _szFileName){
 //TODO 
  return 0;
}
bool CEffect::Reload(){
 //TODO 
  return 0;
}
//DirectX Methods Interface
LPD3DXEFFECT CEffect::GetD3DEffect() const {
 //TODO 
  return 0;
}
D3DXHANDLE CEffect::GetTechniqueByName(const string& _szTechniqueName){
 //TODO 
  return 0;
}