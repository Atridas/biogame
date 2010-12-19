#include "InstanceMesh.h"
#include "Core.h"
#include "RenderManager.h"
#include "StaticMeshManager.h"

CInstanceMesh::CInstanceMesh(const string& _szName, const string& _szCoreName)
{
  //_szName deu ser de CNamed
  if(_szCoreName != "")
    m_StaticMesh = RENDER_MANAGER->GetStaticMeshManager()->GetResource(_szCoreName);
}

void CInstanceMesh::Render(CRenderManager* _pRM)
{
  if(m_StaticMesh)
    m_StaticMesh->Render(_pRM);
}