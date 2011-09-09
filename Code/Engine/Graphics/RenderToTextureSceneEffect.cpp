#include "RenderToTextureSceneEffect.h"
#include "RenderManager.h"
#include "Process.h"
#include "Camera.h"
#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Effect.h"
#include "Texture.h"


bool CRenderToTextureSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderToTextureSceneEffect::Init  Initializing CRenderToTextureSceneEffect.");

  if(!CRenderTextureSceneEffect::Init(_params))
    return false;
  
  string l_szStaticEffect = _params.GetPszISOProperty("static_mesh_effect","");
  string l_szAnimatedEffect = _params.GetPszISOProperty("animated_model_effect","");
  
  CEffectManager* l_pEffectManager = CORE->GetEffectManager();
  m_pStaticMeshEffect    = l_pEffectManager->GetEffect(l_szStaticEffect);
  m_pAnimatedModelEffect = l_pEffectManager->GetEffect(l_szAnimatedEffect);

  if(!m_pStaticMeshEffect || !m_pAnimatedModelEffect)
  {
    if(!m_pStaticMeshEffect)
      LOGGER->AddNewLog(ELL_ERROR, "CRenderToTextureSceneEffect::Init  No technique \"%s\".", l_szStaticEffect.c_str());
    if(!m_pAnimatedModelEffect)
      LOGGER->AddNewLog(ELL_ERROR, "CRenderToTextureSceneEffect::Init  No technique \"%s\".", l_szAnimatedEffect.c_str());

    m_pStaticMeshEffect = m_pAnimatedModelEffect = 0;
    SetOk(false);
  }
  if( IsOk() ) 
  {
    return true;
  } else {
    Release();
    return false;
  }
}

//TODO: UNCOMMENT
void CRenderToTextureSceneEffect::PreRender(CRenderManager* _pRM, CProcess* _pProc)
{
  CCamera* l_pCamera=_pRM->GetCamera();

  CEffectManager* l_pEffectManager = CORE->GetEffectManager();

  //l_pEffectManager->ActivateCamera(l_pCamera->GetViewMatrix(), l_pCamera->GetProjectionMatrix(), l_pCamera->GetPosition());
  _pRM->SetupMatrices(_pProc->GetCamera());
  m_pTexture->SetAsRenderTarget();

  //l_pEffectManager->SetForcedStaticMeshEffect(m_pStaticMeshEffect);
  //l_pEffectManager->SetForcedAnimatedModelEffect(m_pAnimatedModelEffect);

  //_pRM->BeginRendering();
  //_pProc->RenderScene(_pRM);
  //_pRM->EndRendering();

  m_pTexture->UnsetAsRenderTarget();
}

void CRenderToTextureSceneEffect::Release()
{
  CRenderTextureSceneEffect::Release();
}