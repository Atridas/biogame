#include "LightManager.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

bool CLightManager::Load(const string& _szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CLightManager::Load \"%s\"", _szFileName.c_str());

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

    CXMLTreeNode l_XMLLight = l_XMLLights(i);
    l_szName = l_XMLLight.GetPszProperty("name" ,"");
    l_szType = l_XMLLight.GetPszProperty("type" ,"");

    if(l_szType.compare("omni") == 0)
    {
      COmniLight* l_pOmniLight = new COmniLight();
      l_pOmniLight->Init(l_XMLLight);
      AddResource(l_pOmniLight->GetName(),l_pOmniLight);

    }else if(l_szType.compare("directional") == 0)
    {
      CDirectionalLight* l_pDirectionalLight = new CDirectionalLight();
      l_pDirectionalLight->Init(l_XMLLight);
      AddResource(l_pDirectionalLight->GetName(),l_pDirectionalLight);

    }else if(l_szType.compare("spot") == 0)
    {
      CSpotLight* l_pSpotLight = new CSpotLight();
      l_pSpotLight->Init(l_XMLLight);
      AddResource(l_pSpotLight->GetName(),l_pSpotLight);
    }else{
      LOGGER->AddNewLog(ELL_WARNING,"CLightManager:: Unknown light type: \"%s\".", l_szType.c_str());
    }

  }

  return true;
}

void CLightManager::Render(CRenderManager *_pRM) const
{
  TMapResource::const_iterator l_it = m_Resources.cbegin();
  TMapResource::const_iterator l_end = m_Resources.cend();
  for(;l_it != l_end; ++l_it)
  {
    l_it->second->Render(_pRM);
  }
}

