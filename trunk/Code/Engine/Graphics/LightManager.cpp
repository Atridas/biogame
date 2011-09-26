#include "LightManager.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

bool CLightManager::Load(const string& _szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CLightManager::Load \"%s\"", _szFileName.c_str());

  if(m_sFileNames.find(_szFileName) == m_sFileNames.cend())
  {
    m_sFileNames.insert(_szFileName);
  }
  else
  {
    LOGGER->AddNewLog(ELL_WARNING,"CLightManager:: Fitxer XML repetit \"%s\"", _szFileName.c_str());
    SetOk(false);
    return false;
  }

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
      m_vLights.push_back(l_pOmniLight);

    }else if(l_szType.compare("directional") == 0)
    {
      CDirectionalLight* l_pDirectionalLight = new CDirectionalLight(l_szName);
      l_pDirectionalLight->Init(l_XMLLight);
      AddResource(l_pDirectionalLight->GetName(),l_pDirectionalLight);
      m_vLights.push_back(l_pDirectionalLight);

    }else if(l_szType.compare("spot") == 0)
    {
      CSpotLight* l_pSpotLight = new CSpotLight(l_szName);
      l_pSpotLight->Init(l_XMLLight);
      AddResource(l_pSpotLight->GetName(),l_pSpotLight);
      m_vLights.push_back(l_pSpotLight);
    }else{
      LOGGER->AddNewLog(ELL_WARNING,"CLightManager:: Unknown light type: \"%s\".", l_szType.c_str());
    }

  }
  SetOk(true);
  return IsOk();
}

bool CLightManager::Reload()
{
  set<string> l_sPreviousFileNames = m_sFileNames;
  Release();

  set<string>::iterator l_itEnd = l_sPreviousFileNames.cend();

  for(set<string>::iterator l_itCurrent = l_sPreviousFileNames.cbegin(); l_itCurrent != l_itEnd; ++l_itCurrent)
  {
    if(!Load(*l_itCurrent))
    {
      return false;
    }
  }

  return true;  
}

void CLightManager::Release()
{
  CMapManager::Release();
  m_vLights.clear();
  m_sFileNames.clear();
}

CDirectionalLight* CLightManager::CreateDirectionalLight(string _szName,
                                          Vect3f& _vPosition,
                                          Vect3f& _vDirection,
                                          CColor& _colColor,
                                          float _fStartRangeAtt,
                                          float _fEndRangeAtt,
                                          bool _bRenderShadows)
{
  CLight* l_pLight = GetResource(_szName);
  CDirectionalLight* l_pDirectionalLight = 0;

  if(!l_pLight)
  {
    l_pDirectionalLight = new CDirectionalLight(_szName);

    l_pDirectionalLight->SetDirection(_vDirection);
    l_pDirectionalLight->SetPosition(_vPosition);
    l_pDirectionalLight->SetColor(_colColor);
    l_pDirectionalLight->SetStartRangeAttenuation(_fStartRangeAtt);
    l_pDirectionalLight->SetEndRangeAttenuation(_fEndRangeAtt);
    l_pDirectionalLight->SetRenderShadows(_bRenderShadows);
    AddResource(l_pDirectionalLight->GetName(),l_pDirectionalLight);
    m_vLights.push_back(l_pDirectionalLight);
    
  }else{

    if(l_pLight->GetType() != CLight::DIRECTIONAL)
      return 0;

    l_pDirectionalLight = (CDirectionalLight*)l_pLight;

    l_pDirectionalLight->SetDirection(_vDirection);
    l_pDirectionalLight->SetPosition(_vPosition);
    l_pDirectionalLight->SetColor(_colColor);
    l_pDirectionalLight->SetStartRangeAttenuation(_fStartRangeAtt);
    l_pDirectionalLight->SetEndRangeAttenuation(_fEndRangeAtt);
    l_pDirectionalLight->SetRenderShadows(_bRenderShadows); 
  }
  
  return l_pDirectionalLight;
}

CSpotLight* CLightManager::CreateSpotLight(string _szName,
                                            Vect3f& _vPosition,
                                            Vect3f& _vDirection,
                                            CColor& _colColor,
                                            float _fStartRangeAtt,
                                            float _fEndRangeAtt,
                                            float _fAngle,
                                            float _fFallOff,
                                            bool _bRenderShadows)
 {

  CLight* l_pLight = GetResource(_szName);

  CSpotLight* l_pSpotLight = 0;

  if(!l_pLight)
  {
    l_pSpotLight = new CSpotLight(_szName);

    l_pSpotLight->SetDirection(_vDirection);
    l_pSpotLight->SetPosition(_vPosition);
    l_pSpotLight->SetColor(_colColor);
    l_pSpotLight->SetStartRangeAttenuation(_fStartRangeAtt);
    l_pSpotLight->SetEndRangeAttenuation(_fEndRangeAtt);
    l_pSpotLight->SetRenderShadows(_bRenderShadows);
    l_pSpotLight->SetAngle(_fAngle);
    l_pSpotLight->SetFallOff(_fFallOff);
    AddResource(l_pSpotLight->GetName(),l_pSpotLight);
    m_vLights.push_back(l_pSpotLight);

  }else{

    if(l_pLight->GetType() != CLight::SPOT)
      return 0;

    l_pSpotLight = (CSpotLight*)l_pLight;

    l_pSpotLight->SetDirection(_vDirection);
    l_pSpotLight->SetPosition(_vPosition);
    l_pSpotLight->SetColor(_colColor);
    l_pSpotLight->SetStartRangeAttenuation(_fStartRangeAtt);
    l_pSpotLight->SetEndRangeAttenuation(_fEndRangeAtt);
    l_pSpotLight->SetRenderShadows(_bRenderShadows);
    l_pSpotLight->SetAngle(_fAngle);
    l_pSpotLight->SetFallOff(_fFallOff);
  }

  return l_pSpotLight;
}

COmniLight* CLightManager::CreateOmniLight(string _szName,
                                          Vect3f& _vPosition,
                                          CColor& _colColor,
                                          float _fStartRangeAtt,
                                          float _fEndRangeAtt)
{
  CLight* l_pLight = GetResource(_szName);

  if(!l_pLight)
  {
    COmniLight* l_pOmniLight = new COmniLight(_szName);

    l_pOmniLight->SetPosition(_vPosition);
    l_pOmniLight->SetColor(_colColor);
    l_pOmniLight->SetStartRangeAttenuation(_fStartRangeAtt);
    l_pOmniLight->SetEndRangeAttenuation(_fEndRangeAtt);
    l_pOmniLight->SetRenderShadows(false);
    AddResource(l_pOmniLight->GetName(),l_pOmniLight);
    m_vLights.push_back(l_pOmniLight);
    return l_pOmniLight;
  }

  return 0;
}

void CLightManager::Render(CRenderManager *_pRM) const
{
  vector<CLight*>::const_iterator l_it = m_vLights.cbegin();
  vector<CLight*>::const_iterator l_end = m_vLights.cend();

  for(;l_it != l_end; ++l_it)
  {
    (*l_it)->Render(_pRM);
  }
}


CLight* CLightManager::GetLight(int _i) const
{
  if(_i < 0 || _i > (int)m_vLights.size())
  {
    return 0;
  }

  return m_vLights[_i];
}

void CLightManager::SetLightsEnabled(bool _bEnabled)
{
  vector<CLight*>::const_iterator l_it = m_vLights.cbegin();
  vector<CLight*>::const_iterator l_end = m_vLights.cend();

  for(;l_it != l_end; ++l_it)
  {
    (*l_it)->SetActive(_bEnabled);
  }
}

