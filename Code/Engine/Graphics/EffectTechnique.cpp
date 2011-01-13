#include "XML/XMLTreeNode.h"
#include "EffectTechnique.h"
#include "Effect.h"


void CEffectTechnique::Init(CXMLTreeNode& _XMLParams)
{
  Release();
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectTechnique::Init()  Initializing params from XML.");

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
  //RENDER_MANAGER->GetEffectManager()->GetEffect(l_szEffectName);
  
  if(m_pEffect)
    SetOk(true);
  else
    SetOk(false);

}

bool CEffectTechnique::BeginRender()
{

  return false;
}

bool CEffectTechnique::Refresh()
{
  if(m_pEffect)
  {
      m_pD3DTechnique = m_pEffect->GetTechniqueByName(GetName());
      
      if(m_pD3DTechnique)
        return true;

      LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Refresh()  Technique not found.");
      return false;

  }else{

    LOGGER->AddNewLog(ELL_ERROR, "CEffectTechnique::Refresh()  Uninitialized Effect.");
    return false;
  }
}

void CEffectTechnique::Release()
{
  //self
  CHECKED_DELETE(m_pEffect);
  //d3d9
  CHECKED_DELETE(m_pD3DTechnique);
}