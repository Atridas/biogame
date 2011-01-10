#include "LightManager.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

bool CLightManager::Load(const string& _szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CLightManager::Load");

   m_szFileName = _szFileName;

  CXMLTreeNode l_XMLLights;
  if(!l_XMLLights.LoadFile(_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CLightManager:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
    return false;
  }

  int l_iNumMeshesLights = l_XMLLights.GetNumChildren();
  for(int i = 0; i < l_iNumMeshesLights; i++)
  {
    string l_szName = "";
    string l_szType = "";
    CLight::TLightType l_type = CLight::OMNI;
    Vect3f l_vPosition = Vect3f(0.0f);
    CColor l_colColor = colWHITE;
    bool l_bRenderShadows = false;
    float l_fStartRangeAttenuation = 0.0f;
    float l_fEndRangeAttenuation = 0.0f;

    CXMLTreeNode l_XMLLight = l_XMLLights(i);
    l_szName = l_XMLLight.GetPszProperty("name" ,"");
    l_szType = l_XMLLight.GetPszProperty("type" ,"");
    l_vPosition = l_XMLLight.GetVect3fProperty("pos",Vect3f(0.0f));
    l_colColor = CColor(Vect4f(l_XMLLight.GetVect3fProperty("color",Vect3f(1.0f)),1.0f));
    l_bRenderShadows = l_XMLLight.GetBoolProperty("render_shadows",false);
    l_fStartRangeAttenuation = l_XMLLight.GetFloatProperty("att_start_range",false);
    l_fEndRangeAttenuation = l_XMLLight.GetFloatProperty("att_end_range",false);

    if(l_szType.compare("omni"))
    {
      COmniLight* l_pOmniLight = new COmniLight();
      l_type = CLight::OMNI;
      AddResource(l_szName,l_pOmniLight);

    }else if(l_szType.compare("directional"))
    {
      CDirectionalLight* l_pDirectionalLight = new CDirectionalLight();
      l_type = CLight::DIRECTIONAL;
      Vect3f l_vDirection = l_XMLLight.GetVect3fProperty("direction",Vect3f(1.0f));
      l_pDirectionalLight->SetDirection(l_vDirection);
      AddResource(l_szName,l_pDirectionalLight);

    }else if(l_szType.compare("spot"))
    {
      CSpotLight* l_pSpotLight = new CSpotLight();
      l_type = CLight::SPOT;
      float l_fAngle = l_XMLLight.GetFloatProperty("angle",1.0f);
      float l_fFallOff = l_XMLLight.GetFloatProperty("fall_off",1.0f);
      Vect3f l_vDirection = l_XMLLight.GetVect3fProperty("direction",Vect3f(1.0f));
      l_pSpotLight->SetDirection(l_vDirection);
      l_pSpotLight->SetAngle(l_fAngle);
      l_pSpotLight->SetFallOff(l_fFallOff);
      AddResource(l_szName,l_pSpotLight);
    }

    CLight* l_pLight = GetResource(l_szName);
    l_pLight->SetName(l_szName);
    l_pLight->SetType(l_type);
    l_pLight->SetColor(l_colColor);
    l_pLight->SetPosition(l_vPosition);
    l_pLight->SetRenderShadows(l_bRenderShadows);
    l_pLight->SetStartRangeAttenuation(l_fStartRangeAttenuation);
    l_pLight->SetEndRangeAttenuation(l_fEndRangeAttenuation);

    /*if(!GetResource(l_szName))
    {
      l_pStaticMesh = new CStaticMesh();

      if(l_pStaticMesh->Load(l_szPath))
      {
        LOGGER->AddNewLog(ELL_INFORMATION,"CStaticMeshManager:: Adding mesh: \"%s\"", l_szName.c_str());
        AddResource(l_szName,l_pStaticMesh);
      }else{
        CHECKED_DELETE(l_pStaticMesh);
      }
    }else{
      if(!_bReload)
        LOGGER->AddNewLog(ELL_WARNING,"CStaticMeshManager:: Mesh \"%s\" repetit", l_szName.c_str());
    }*/
  }

  return true;
}

void CLightManager::Render(CRenderManager *RenderManager)
{

}

