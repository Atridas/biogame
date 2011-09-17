#include "PreSceneRendererStep.h"
#include "XML/XMLTreeNode.h"

bool CPreSceneRendererStep::Init(CXMLTreeNode& _treePreSceneRenderer, const string& _szDefaultRenderTarget)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CPreSceneRendererStep::Init PreSceneRenderer:");

  bool l_bActive = _treePreSceneRenderer.GetBoolProperty("active",false,false);

  SetActive(l_bActive);

  SetOk(CSceneRendererStep::Init(_treePreSceneRenderer, _szDefaultRenderTarget));

  return IsOk();
}

void CPreSceneRendererStep::Release()
{
  CSceneRendererStep::Release();
}
