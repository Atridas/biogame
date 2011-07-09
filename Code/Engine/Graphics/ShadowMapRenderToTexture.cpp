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
    LOGGER->AddNewLog(ELL_WARNING, "CShadowMapRenderToTexture::Init  No light \"%s\".", l_szLightShadowCast.c_str());
  }
  SetOk(true);
  return IsOk();
}

void CShadowMapRenderToTexture::PreRender(CRenderManager *_pRM, CProcess *_pProc)
{
  //Render ShadowMap
  if(m_pLightShadowCast != 0)
  {
    if(m_pLightShadowCast->GetRenderShadows())
    {
      //Nos debemos guardar las matrices de View y de Proyección actualmente activas en el EffectManager
      const Vect3f &l_Position=m_pLightShadowCast->GetPosition();
      CDirectionalLight* l_pDirLight = ((CDirectionalLight *)m_pLightShadowCast); 
      Vect3f l_VDir= l_pDirLight->GetDirection();

      //Creamos las matrices de View y de Proyección según la luz
      CEffectManager *l_pEffectManager = CORE->GetEffectManager();
      Mat44f l_LightViewMatrix = l_pDirLight->GetLightViewMatrix();
      Mat44f l_LightProjectionMatrix = l_pDirLight->GetLightProjectionMatrix();
      Vect3f l_LightPosition = l_pDirLight->GetPosition();
      Vect3f l_LightRight       = Vect3f(0,1,0) ^ (l_pDirLight->GetDirection()).GetNormalized();
      if(l_LightRight.SquaredLength() < 0.05f)
      {
        l_LightRight       = (l_pDirLight->GetDirection() ^ Vect3f(0,0,1)).GetNormalized();
      }
      else
      {
        l_LightRight.Normalize();
      }
      Vect3f l_LightUp = (l_pDirLight->GetDirection() ^ l_LightRight).GetNormalized();

      l_pEffectManager->ActivateCamera(l_LightViewMatrix, l_LightProjectionMatrix, l_LightPosition,l_LightUp,l_LightRight);
      l_pEffectManager->SetShadowProjectionMatrix(l_LightProjectionMatrix);
      l_pEffectManager->SetLightViewMatrix(l_LightViewMatrix);
      m_pTexture->SetAsRenderTarget();

      l_pEffectManager->SetForcedStaticMeshEffect(m_pStaticMeshEffect);
      l_pEffectManager->SetForcedAnimatedModelEffect(m_pAnimatedModelEffect);

      _pRM->BeginRendering();
      _pProc->RenderScene(_pRM);
      _pRM->EndRendering();

      m_pTexture->UnsetAsRenderTarget();
      //Debemos establecer las matrices de View y de Proyección guardadas previamente en el EffectManager
      //End render shadowMap
    }
    
  }
}

void CShadowMapRenderToTexture::Release()
{
  CRenderToTextureSceneEffect::Release();
}