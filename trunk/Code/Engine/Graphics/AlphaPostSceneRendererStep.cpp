#include "AlphaPostSceneRendererStep.h"


bool CAlphaPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget)
{
  bool l_bOk = CPostSceneRendererStep::Init(_treePostSceneRenderer,_szDefaultRenderTarget);
  if(!l_bOk)
    return false;

  m_bRenderOpaque = false;
  m_bRenderAlphas = true;

  return true;
}