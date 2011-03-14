#include "GameObjectManager.h"
#include "RenderManager.h"
#include "InstanceMesh.h"
#include "StaticMesh.h"
#include <XML/XMLTreeNode.h>
#include "RenderableAnimatedInstanceModel.h"
#include "InstancedData.h"
#include "Core.h"


void CGameObjectManager::Release()
{
  CMapManager<CGameObject>::Release();
}
