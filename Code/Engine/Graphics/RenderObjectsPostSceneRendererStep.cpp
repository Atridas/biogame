#include "RenderObjectsPostSceneRendererStep.h"
#include "SceneRendererStep.h"
#include "RenderManager.h"
#include "Core.h"
#include "EffectManager.h"

bool CRenderObjectsPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CRenderObjectsPostSceneRendererStep::Init");
  //bool l_bOk = CPostSceneRendererStep::Init(_treePostSceneRenderer,_szDefaultRenderTarget, false);
  //if(!l_bOk)
  //  return false;
  SetOk(true);

  m_pSceneRendererStep = new CSceneRendererStep();
  if(!m_pSceneRendererStep->Init(_treePostSceneRenderer,_szDefaultRenderTarget))
  {
    delete m_pSceneRendererStep;
    SetOk(false);
  }
  else
  {
    SetName(m_pSceneRendererStep->GetName());
    
    m_iSize.x = RENDER_MANAGER->GetScreenWidth();
    m_iSize.y = RENDER_MANAGER->GetScreenHeight();

    m_szRenderTarget = m_pSceneRendererStep->GetRenderTarget();
  }

  return IsOk();
}

void CRenderObjectsPostSceneRendererStep::Release()
{
  CHECKED_DELETE(m_pSceneRendererStep);
  CPostSceneRendererStep::Release();
}

void CRenderObjectsPostSceneRendererStep::Render(CRenderManager* _pRM, CCamera* _pCamera,
                                                 const vector<CObject3DRenderable*>& _vOpaqueObjects,
                                                 const vector<CObject3DRenderable*>& _vAlphaObjects,
                                                 const vector<CObject3DRenderable*>& _vParticleEmiters)
{
  CORE->GetEffectManager()->SetViewport(m_iPos.x, m_iPos.y, m_iSize.x, m_iSize.y);
  m_pSceneRendererStep->Render(_pRM, _pCamera,_vOpaqueObjects,_vAlphaObjects,_vParticleEmiters);
}
