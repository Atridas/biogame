#include "InstanceMesh.h"
#include "Core.h"
#include "RenderManager.h"
#include "StaticMeshManager.h"


CInstanceMesh::CInstanceMesh(const string& _szName): CRenderableObject(_szName)
{
  
}

bool CInstanceMesh::Init(const string& _szCoreName)
{
  if(_szCoreName != "") 
  {
    m_StaticMesh = RENDER_MANAGER->GetStaticMeshManager()->GetResource(_szCoreName);
    if(m_StaticMesh != 0)
    {
      SetOk(true);
    }
  } else {
    SetOk(true);
  }

  return IsOk();
}

void CInstanceMesh::Release()
{
  //res, el static mesh el peta el manager.
}

void CInstanceMesh::RenderRenderableObject(CRenderManager* _pRM)
{
  if(m_StaticMesh)
    m_StaticMesh->Render(_pRM);
}