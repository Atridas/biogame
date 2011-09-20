#include "RenderObjectsPostSceneRendererStep.h"
#include "SceneRendererStep.h"
#include "RenderManager.h"
#include "Core.h"

bool CRenderObjectsPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget)
{
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
  m_pSceneRendererStep->Render(_pRM, _pCamera,_vOpaqueObjects,_vAlphaObjects,_vParticleEmiters);
}
