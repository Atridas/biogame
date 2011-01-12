#include "Effect.h"


CEffect::CEffect(void):
  m_FileName(""),
  m_Effect(0),
  m_WorldMatrixParameter(0),
  m_ViewMatrixParameter(0),
  m_ProjectionMatrixParameter(0),
  m_WorldViewMatrixParameter(0),
  m_ViewProjectionMatrixParameter(0),
  m_WorldViewProjectionMatrixParameter(0),
  m_ViewToLightProjectionMatrixParameter(0),
  m_LightEnabledParameter(0),
  m_LightsTypeParameter(0),
  m_LightsPositionParameter(0),
  m_LightsDirectionParameter(0),
  m_LightsAngleParameter(0),
  m_LightsColorParameter(0),
  m_LightsFallOffParameter(0),
  m_LightsStartRangeAttenuationParameter(0),
  m_LightsEndRangeAttenuationParameter(0),
  m_CameraPositionParameter(0),
  m_BonesParameter(0),
  m_TimeParameter(0)
{
  for(int i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
  {
    m_LightsEnabled[i] = 0;
    m_LightsType[i] = 0;
    m_LightsAngle[i] = 0;
    m_LightsFallOff[i] = 0;
    m_LightsStartRangeAttenuation[i] = 0;
    m_LightsEndRangeAttenuation[i] = 0;
    m_LightsPosition[i] = Vect3f(0.0f);
    m_LightsDirection[i] = Vect3f(0.0f);
    m_LightsColor[i] = Vect3f(0.0f);
  }
}

void CEffect::Release() {
 //TODO 
  
}

void CEffect::SetNullParameters(){
 //TODO 
  
}
void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle){
 //TODO 
  
}
bool CEffect::LoadEffect(){
 //TODO 
  return 0;
}
bool CEffect::SetLights(size_t NumOfLights){
 //TODO 
  return 0;
}
bool CEffect::Load(const std::string &FileName){
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
D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName){
 //TODO 
  return 0;
}