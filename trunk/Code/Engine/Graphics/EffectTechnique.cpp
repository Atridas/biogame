#include "XML/XMLTreeNode.h"
#include "EffectTechnique.h"
#include "Effect.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include <Core.h>


void CEffectTechnique::Init(CXMLTreeNode& _XMLParams)
{
  //Release();
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectTechnique::Init  Initializing params from XML.");

  //strings
  SetName(_XMLParams.GetPszProperty("name" ,""));
  string l_szEffectName = _XMLParams.GetPszProperty("effect" ,"");

  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectTechnique::Init  Effect \"%s\" Technique \"%s\".",GetName().c_str(), l_szEffectName.c_str());

  //booleans
  m_bUseCameraPosition = _XMLParams.GetBoolProperty("use_camera_position");
  m_bUseInverseProjMatrix = _XMLParams.GetBoolProperty("use_inverse_proj_matrix");
  m_bUseInverseViewMatrix = _XMLParams.GetBoolProperty("use_inverse_view_matrix");
  m_bUseInverseWorldMatrix = _XMLParams.GetBoolProperty("use_inverse_world_matrix");
  m_bUseLights = _XMLParams.GetBoolProperty("use_lights");
  m_bUseProjMatrix = _XMLParams.GetBoolProperty("use_proj_matrix");
  m_bUseViewMatrix = _XMLParams.GetBoolProperty("use_view_matrix");
  m_bUseWorldMatrix = _XMLParams.GetBoolProperty("use_world_matrix");
  m_bUseWorldViewMatrix = _XMLParams.GetBoolProperty("use_world_view_matrix");
  m_bUseWorldViewProjectionMatrix = _XMLParams.GetBoolProperty("use_world_view_projection_matrix");
  m_bUseViewProjectionMatrix = _XMLParams.GetBoolProperty("use_view_projection_matrix");
  m_bUseViewToLightProjectionMatrix = _XMLParams.GetBoolProperty("use_view_to_light_projection_matrix");
  m_bUseTime = _XMLParams.GetBoolProperty("use_time");
  m_bAnimated = _XMLParams.GetBoolProperty("animated");

  //integers
  m_iNumOfLights= _XMLParams.GetIntProperty("num_of_lights",0);

  //non XML dependant
  CEffectManager* l_pEffectManager = RENDER_MANAGER->GetEffectManager();
  m_pEffect = l_pEffectManager->GetEffect(l_szEffectName);

  if(m_pEffect->IsOk())
  {
    if(m_bAnimated)
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "CEffectTechnique::Init  Adding animated technique to EffectManager.");
      l_pEffectManager->SetAnimatedModelTechnique(this);
    }
    m_pD3DTechnique = m_pEffect->GetTechniqueByName(GetName());
  
    if(m_pEffect && m_pD3DTechnique)
      SetOk(true);
    else
    {
      if(!m_pEffect)
        LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Init  Effect \"%s\" not loaded.", l_szEffectName.c_str());
      if(!m_pD3DTechnique)
        LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Init  D3DTechnique \"%s\" not loaded.", GetName().c_str());

      SetOk(false);
    }
  } else {
    LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Init  Effect \"%s\" not loaded.", l_szEffectName.c_str());
  }
}

bool CEffectTechnique::BeginRender()
{
  if(m_pEffect && m_pEffect->IsOk())
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
        l_pD3DEffect->SetFloatArray   (m_pEffect->m_pAmbientLight,                        m_pEffect->m_aAmbientLight,                 3);

        l_pD3DEffect->SetBoolArray    (m_pEffect->m_pLightsEnabledParameter,      (BOOL*) m_pEffect->m_aLightsEnabled,                MAX_LIGHTS_BY_SHADER);
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
      l_pD3DEffect->SetMatrix(m_pEffect->m_pProjectionMatrixParameter,&l_pEM->GetInverseProjectionMatrix().GetD3DXMatrix());
    //World
    if(m_bUseWorldMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldMatrixParameter,&l_pEM->GetWorldMatrix().GetD3DXMatrix());
    if(m_bUseInverseWorldMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldMatrixParameter,&l_pEM->GetInverseWorldMatrix().GetD3DXMatrix());
    //View
    if(m_bUseViewMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewMatrixParameter,&l_pEM->GetViewMatrix().GetD3DXMatrix());
    if(m_bUseInverseViewMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewMatrixParameter,&l_pEM->GetInverseViewMatrix().GetD3DXMatrix());
    //Composed
    if(m_bUseViewProjectionMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewProjectionMatrixParameter,&l_pEM->GetViewProjectionMatrix().GetD3DXMatrix());
    if(m_bUseWorldViewMatrix)
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldViewMatrixParameter,&(l_pEM->GetWorldViewMatrix()).GetD3DXMatrix());
    if(m_bUseWorldViewProjectionMatrix)
    {
      //D3DXMATRIX l_World=l_pEM->GetWorldMatrix().GetD3DXMatrix();
      //D3DXMATRIX l_View=l_pEM->GetViewMatrix().GetD3DXMatrix();
      //D3DXMATRIX l_Projection=l_pEM->GetProjectionMatrix().GetD3DXMatrix();
      //D3DXMATRIX l_WorldViewProjection=l_World*l_View*l_Projection;
      
      //l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldViewProjectionMatrixParameter,&l_WorldViewProjection);
      l_pD3DEffect->SetMatrix(m_pEffect->m_pWorldViewProjectionMatrixParameter,&(l_pEM->GetWorldViewProjectionMatrix()).GetD3DXMatrix());
    }
    //Light
    if(m_bUseViewToLightProjectionMatrix)
      //LightView, no LightViewProjection
      l_pD3DEffect->SetMatrix(m_pEffect->m_pViewToLightProjectionMatrixParameter,&l_pEM->GetLightViewMatrix().GetD3DXMatrix());

  } else {
    //LOGGER->AddNewLog(ELL_WARNING, "CEffectTechnique::BeginRender  No effect specified.");
    return false;
  }
  return true;
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
  if(m_bAnimated)
  {
    CEffectManager* l_pEffectManager = RENDER_MANAGER->GetEffectManager();
    if(l_pEffectManager->GetAnimatedModelTechnique() == this)
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "CEffectTechnique::Release  Removing animated technique from EffectManager.");
      l_pEffectManager->SetAnimatedModelTechnique(0);
    }
  }
}