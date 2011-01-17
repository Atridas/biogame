#include "XML/XMLTreeNode.h"
#include "EffectTechnique.h"
#include "Effect.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include <Core.h>


void CEffectTechnique::Init(CXMLTreeNode& _XMLParams)
{
  Release();
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectTechnique::Init  Initializing params from XML.");

  //strings
  SetName(_XMLParams.GetPszProperty("name" ,""));
  string l_szEffectName = _XMLParams.GetPszProperty("effect" ,"");

  //booleans
  m_bUseCameraPosition = _XMLParams.GetBoolProperty("use_camera_position");
  m_bUseInverseProjMatrix = _XMLParams.GetBoolProperty("use_inverse_proj_matrix");
  m_bUseInverseViewMatrix = _XMLParams.GetBoolProperty("use_inverse_view_matrix");
  m_bUseInverseWorldMatrix = _XMLParams.GetBoolProperty("use_inverse_world_matrix");
  m_bUseLights = _XMLParams.GetBoolProperty("use_lights");
  m_bUseLightAmbientColor = _XMLParams.GetBoolProperty("use_light_ambient_color");
  m_bUseProjMatrix = _XMLParams.GetBoolProperty("use_proj_matrix");
  m_bUseViewMatrix = _XMLParams.GetBoolProperty("use_view_matrix");
  m_bUseWorldMatrix = _XMLParams.GetBoolProperty("use_world_matrix");
  m_bUseWorldViewMatrix = _XMLParams.GetBoolProperty("use_world_view_matrix");
  m_bUseWorldViewProjectionMatrix = _XMLParams.GetBoolProperty("use_world_view_projection_matrix");
  m_bUseViewProjectionMatrix = _XMLParams.GetBoolProperty("use_view_projection_matrix");
  m_bUseViewToLightProjectionMatrix = _XMLParams.GetBoolProperty("use_view_to_light_projection_matrix");
  m_bUseTime = _XMLParams.GetBoolProperty("use_time");

  //integers
  m_iNumOfLights= _XMLParams.GetIntProperty("nom_of_lights",0);

  //non XML dependant
  m_pEffect = RENDER_MANAGER->GetEffectManager()->GetEffect(l_szEffectName);
  m_pD3DTechnique = m_pEffect->GetTechniqueByName(l_szEffectName);
  
  if(m_pEffect && m_pD3DTechnique)
    SetOk(true);
  else
  {
    if(!m_pEffect)
      LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Init  Effect not loaded.");
    if(!m_pD3DTechnique)
      LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Init  D3DTechnique not loaded.");

    SetOk(false);
  }

}

bool CEffectTechnique::BeginRender()
{
  if(m_pEffect)
  {
    CEffectManager* l_pEM = RENDER_MANAGER->GetEffectManager();
    LPD3DXEFFECT l_pD3DEffect = m_pEffect->GetD3DEffect();

    //Position
    if(m_bUseCameraPosition)
    {
      Vect3f l_vPos = l_pEM->GetCameraEye();
      const float l_vfPos[3] = {l_vPos.x,l_vPos.y,l_vPos.z};
      l_pD3DEffect->SetFloatArray(m_pEffect->m_pCameraPositionParameter,l_vfPos,3);
    }

    //Lights
    if(m_bUseLights)
    {
      //BOOL l_iLight = true;
      //l_pD3DEffect->GetBool(m_pEffect->m_pLightsEnabledParameter,l_iLight);

      if(m_pEffect->SetLights(m_iNumOfLights))
      {
        l_pD3DEffect->SetBoolArray    (m_pEffect->m_pLightsEnabledParameter,      (BOOL*) m_pEffect->m_aLightsEnabled,                m_iNumOfLights);
        l_pD3DEffect->SetIntArray     (m_pEffect->m_pLightsTypeParameter,                 m_pEffect->m_aLightsType,                   m_iNumOfLights);
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsAngleParameter,                m_pEffect->m_aLightsAngle,                  m_iNumOfLights);

        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsFallOffParameter,              m_pEffect->m_aLightsFallOff,                m_iNumOfLights);
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsStartRangeAttenuationParameter,m_pEffect->m_aLightsStartRangeAttenuation,  m_iNumOfLights);
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsEndRangeAttenuationParameter,  m_pEffect->m_aLightsEndRangeAttenuation,    m_iNumOfLights);
      
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsPositionParameter,     (float*)m_pEffect->m_aLightsPosition,               m_iNumOfLights * 3);
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsDirectionParameter,    (float*)m_pEffect->m_aLightsDirection,              m_iNumOfLights * 3);
      
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pLightsColorParameter,        (float*)m_pEffect->m_aLightsColor,                  m_iNumOfLights * 4);
      }
    }
    //TODO LightManager()->GetAmbientLight();
    //if(m_bUseLightAmbientColor)
    //{
    //  l_pD3DEffect->SetFloatArray(m_pEffect->m_pLightsColorParameter,CORE->GetLightManager());
    //}
    //TODO GetElapsedTime() El podem propagar com a tots els renders.
    //Time
    //if(m_bUseTime)
    //  l_pD3DEffect->SetFloat(m_pEffect->m_pTimeParameter,&_fElapsedTime);
    
    //Matrix
    //Projection
    if(m_bUseProjMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pProjectionMatrixParameter,&l_pEM->GetProjectionMatrix().GetD3DXMatrix());
    if(m_bUseInverseProjMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pProjectionMatrixParameter,&l_pEM->GetProjectionMatrix().GetInverted().GetD3DXMatrix());
    //World
    if(m_bUseWorldMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldMatrixParameter,&l_pEM->GetWorldMatrix().GetD3DXMatrix());
    if(m_bUseInverseWorldMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldMatrixParameter,&l_pEM->GetWorldMatrix().GetInverted().GetD3DXMatrix());
    //View
    if(m_bUseViewMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewMatrixParameter,&l_pEM->GetViewMatrix().GetD3DXMatrix());
    if(m_bUseInverseViewMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewMatrixParameter,&l_pEM->GetViewMatrix().GetInverted().GetD3DXMatrix());
    //Composed
    if(m_bUseViewProjectionMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewProjectionMatrixParameter,&l_pEM->GetViewProjectionMatrix().GetD3DXMatrix());
    if(m_bUseWorldViewMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldViewMatrixParameter,&(l_pEM->GetWorldMatrix()*l_pEM->GetViewMatrix()).GetD3DXMatrix());
    if(m_bUseWorldViewProjectionMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldViewProjectionMatrixParameter,&(l_pEM->GetWorldMatrix()*l_pEM->GetViewMatrix()*l_pEM->GetProjectionMatrix()).GetD3DXMatrix());
    //Light
    if(m_bUseViewToLightProjectionMatrix)
      //LightView, no LightViewProjection
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewToLightProjectionMatrixParameter,&l_pEM->GetLightViewMatrix().GetD3DXMatrix());

  }
  LOGGER->AddNewLog(ELL_WARNING, "CEffectTechnique::BeginRender  No effect specified.");
  return false;
}

bool CEffectTechnique::Refresh()
{
  if(m_pEffect)
  {
      m_pD3DTechnique = m_pEffect->GetTechniqueByName(GetName());
      
      if(m_pD3DTechnique)
        return true;

      LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Refresh  Technique not found.");
      return false;

  }else{

    LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Refresh  Uninitialized Effect.");
    return false;
  }
}

void CEffectTechnique::Release()
{
  //self
  CHECKED_DELETE(m_pEffect);
  //d3d9
  //CHECKED_DELETE(m_pD3DTechnique); Els handles NO s'alliveren.
}