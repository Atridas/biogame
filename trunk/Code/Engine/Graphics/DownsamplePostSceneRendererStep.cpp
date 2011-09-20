#include "DownsamplePostSceneRendererStep.h"


bool CDownsamplePostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget)
{
  if(CPostSceneRendererStep::Init(_treePostSceneRenderer, _szDefaultRenderTarget, false))
  {

  }

  return IsOk();
}

void CDownsamplePostSceneRendererStep::Release()
{
  CPostSceneRendererStep::Release();
}

void CDownsamplePostSceneRendererStep::Render(CRenderManager* _pRM)
{
  CPostSceneRendererStep::Render(_pRM);
}
