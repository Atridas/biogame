#include "ShadowMapRenderToTexture.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "LightManager.h"
#include "Light.h"

bool CShadowMapRenderToTexture::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CShadowMapRenderToTexture::Init  Initializing CShadowMapRenderToTexture.");

  if(!CRenderToTextureSceneEffect::Init(_params))
    return false; 

  string l_szLightShadowCast = _params.GetPszISOProperty("light_shadow_cast","");

  m_pLightShadowCast = CORE->GetLightManager()->GetResource(l_szLightShadowCast);

  if(!m_pLightShadowCast)
  {
    LOGGER->AddNewLog(ELL_INFORMATION, "CShadowMapRenderToTexture::Init  No light \"%s\".", l_szLightShadowCast.c_str());
    SetOk(false);
  }
  return IsOk();
}

void CShadowMapRenderToTexture::Release()
{
  CRenderToTextureSceneEffect::Release();
}