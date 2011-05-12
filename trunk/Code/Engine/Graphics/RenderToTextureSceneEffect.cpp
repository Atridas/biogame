#include "RenderToTextureSceneEffect.h"
#include "RenderManager.h"
#include "Process.h"
#include "Camera.h"
#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "EffectTechnique.h"
#include "Texture.h"


bool CRenderToTextureSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderToTextureSceneEffect::Init  Initializing CRenderToTextureSceneEffect.");

  if(!CRenderTextureSceneEffect::Init(_params))
    return false;
  
  string l_szStaticTechnique = _params.GetPszISOProperty("static_mesh_technique","");
  string l_szAnimatedTechnique = _params.GetPszISOProperty("animated_model_technique","");
  
  CEffectManager* l_pEffectManager = CORE->GetEffectManager();
  //m_pStaticMeshTechnique    = l_pEffectManager->GetEffectTechnique(l_szStaticTechnique);
  //m_pAnimatedModelTechnique = l_pEffectManager->GetEffectTechnique(l_szAnimatedTechnique);

  if(!m_pStaticMeshTechnique || !m_pAnimatedModelTechnique)
  {
    if(!m_pStaticMeshTechnique)
      LOGGER->AddNewLog(ELL_ERROR, "CRenderToTextureSceneEffect::Init  No technique \"%s\".", l_szStaticTechnique.c_str());
    if(!m_pAnimatedModelTechnique)
      LOGGER->AddNewLog(ELL_ERROR, "CRenderToTextureSceneEffect::Init  No technique \"%s\".", l_szAnimatedTechnique.c_str());

    m_pStaticMeshTechnique = m_pAnimatedModelTechnique = 0;
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

  _pRM->BeginRendering();

  //l_pEffectManager->SetStaticMeshTechnique(m_pStaticMeshTechnique);
  //l_pEffectManager->SetAnimatedModelTechnique(m_pAnimatedModelTechnique);
  _pProc->RenderScene(_pRM);
  _pRM->EndRendering();

  m_pTexture->UnsetAsRenderTarget();
}

void CRenderToTextureSceneEffect::Release()
{
  CRenderTextureSceneEffect::Release();
}