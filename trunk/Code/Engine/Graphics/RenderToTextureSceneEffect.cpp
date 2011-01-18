#include "RenderToTextureSceneEffect.h"
#include "RenderManager.h"
#include "Process.h"
#include "Camera.h"
#include "EffectManager.h"

//TODO: UNCOMMENT
void CRenderToTextureSceneEffect::PreRender(CRenderManager* _pRM, CProcess* _pProc)
{
  CCamera* l_pCamera=_pRM->GetCamera();

  CEffectManager* l_pEffectManager = _pRM->GetEffectManager();

  //l_pEffectManager->ActivateCamera(l_pCamera->GetViewMatrix(), l_pCamera->GetProjectionMatrix(), l_pCamera->GetPosition());
  //m_pTexture->SetAsRenderTarget();

  _pRM->BeginRendering();

  l_pEffectManager->SetStaticMeshTechnique(m_pStaticMeshTechnique);
  l_pEffectManager->SetAnimatedModelTechnique(m_pAnimatedModelTechnique);
  //_pProc->Render3DScene(PSParams);
  _pRM->EndRendering();

  //m_pTexture->UnsetAsRenderTarget();
}