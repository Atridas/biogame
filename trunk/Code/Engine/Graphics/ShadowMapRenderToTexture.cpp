#include "ShadowMapRenderToTexture.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "EffectManager.h"
#include "Texture.h"
#include "Process.h"

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

void CShadowMapRenderToTexture::PreRender(CRenderManager *_pRM, CProcess *_pProc)
{
  //Render ShadowMap
  if(m_pLightShadowCast!=NULL)
  {
    //Nos debemos guardar las matrices de View y de Proyección actualmente activas en el EffectManager
    const Vect3f &l_Position=m_pLightShadowCast->GetPosition();
    CDirectionalLight* l_pDirLight = ((CDirectionalLight *)m_pLightShadowCast); 
    Vect3f l_VDir= l_pDirLight->GetDirection();

    //Creamos las matrices de View y de Proyección según la luz
    CEffectManager *l_pEffectManager=_pRM->GetEffectManager();
    Mat44f l_LightViewMatrix = l_pDirLight->GetLightViewMatrix();
    Mat44f l_LightProjectionMatrix = l_pDirLight->GetLightViewMatrix();
    Vect3f l_LightPosition = l_pDirLight->GetPosition();


    l_pEffectManager->ActivateCamera(l_LightViewMatrix, l_LightProjectionMatrix, l_LightPosition);
    l_pEffectManager->SetShadowProjectionMatrix(l_LightProjectionMatrix);
    l_pEffectManager->SetLightViewMatrix(l_LightViewMatrix);
    m_pTexture->SetAsRenderTarget();

    _pRM->BeginRendering();

    l_pEffectManager->SetStaticMeshTechnique(m_pStaticMeshTechnique);
    l_pEffectManager->SetAnimatedModelTechnique(m_pAnimatedModelTechnique);
    _pProc->RenderScene(_pRM);

    _pRM->EndRendering();
    m_pTexture->UnsetAsRenderTarget();
    //Debemos establecer las matrices de View y de Proyección guardadas previamente en el EffectManager
    //End render shadowMap

    
  }
}

void CShadowMapRenderToTexture::Release()
{
  CRenderToTextureSceneEffect::Release();
}