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
    SetOk(false);
    return false;
  }

  m_vAmbientLight = l_XMLLights.GetVect3fProperty("ambient",Vect3f(0.3f));

  int l_iNumMeshesLights = l_XMLLights.GetNumChildren();
  for(int i = 0; i < l_iNumMeshesLights; i++)
  {
    CXMLTreeNode l_XMLLight = l_XMLLights(i);
    if(l_XMLLight.IsComment()) {
      continue;
    }

    string l_szName = "";
    string l_szType = "";

    l_szName = l_XMLLight.GetPszISOProperty("name" ,"");
    l_szType = l_XMLLight.GetPszISOProperty("type" ,"");

    if(l_szType.compare("omni") == 0)
    {
      COmniLight* l_pOmniLight = new COmniLight(l_szName);
      l_pOmniLight->Init(l_XMLLight);
      AddResource(l_pOmniLight->GetName(),l_pOmniLight);

    }else if(l_szType.compare("directional") == 0)
    {
      CDirectionalLight* l_pDirectionalLight = new CDirectionalLight(l_szName);
      l_pDirectionalLight->Init(l_XMLLight);
      AddResource(l_pDirectionalLight->GetName(),l_pDirectionalLight);

    }else if(l_szType.compare("spot") == 0)
    {
      CSpotLight* l_pSpotLight = new CSpotLight(l_szName);
      l_pSpotLight->Init(l_XMLLight);
      AddResource(l_pSpotLight->GetName(),l_pSpotLight);
    }else{
      LOGGER->AddNewLog(ELL_WARNING,"CLightManager:: Unknown light type: \"%s\".", l_szType.c_str());
    }

  }
  SetOk(true);
  return IsOk();
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


CLight* CLightManager::GetLight(int _i) const
{
  TMapResource::const_iterator l_it = m_Resources.cbegin();
  TMapResource::const_iterator l_end = m_Resources.cend();

  if(l_it == l_end)
  {
    return 0;
  }
  for(int i = 0; i<_i; ++i)
  {
    ++l_it;
    if(l_it == l_end)
    {
      return 0;
    }
  }
  return l_it->second;
}

vector<CLight*> CLightManager::GetLights(int _num) const
{
  vector<CLight*> l_vOut;
  TMapResource::const_iterator l_it = m_Resources.cbegin();
  TMapResource::const_iterator l_end = m_Resources.cend();

  if(l_it == l_end)
  {
    return l_vOut;
  }
  for(int i = 0; i<_num; ++i)
  {
    l_vOut.push_back(l_it->second);
    ++l_it;
    if(l_it == l_end)
    {
      return l_vOut;
    }
  }
  return l_vOut;
}

