#include "PreSceneRendererStep.h"
#include "XML/XMLTreeNode.h"

bool CPreSceneRendererStep::Init(CXMLTreeNode& _treePreSceneRenderer)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CPreSceneRendererStep::Init PreSceneRenderer:");

  bool l_bActive = _treePreSceneRenderer.GetBoolProperty("active",false,false);

  SetActive(l_bActive);

  SetOk(CSceneRendererStep::Init(_treePreSceneRenderer));

  return IsOk();
}

void CPreSceneRendererStep::Release()
{
  CSceneRendererStep::Release();
}
