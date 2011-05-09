
#include "ComponentObject3D.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "PhysicCookingMesh.h"
#include "Core.h"
#include "ComponentRenderableObject.h"
#include "RenderableObject.h"
#include "StaticMesh.h"
#include "InstanceMesh.h"

#include "ComponentPhysXMesh.h"

bool CComponentPhysXMesh::Init(CGameEntity* _pEntity, float _fDensity, int _iCollisionMask)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats


  //Agafem el static mesh -----------------------------------------
  CComponentRenderableObject* l_pComponentRenderableObject = dynamic_cast<CComponentRenderableObject*>(_pEntity->GetComponent(ECT_RENDERABLE_OBJECT));
  assert(l_pComponentRenderableObject);

  CRenderableObject* l_pRenderableObject = l_pComponentRenderableObject->GetRenderableObject();
  CInstanceMesh* l_pInstanceMesh = dynamic_cast<CInstanceMesh*>(l_pRenderableObject);
  assert(l_pInstanceMesh);
  CStaticMesh* l_pStaticMesh = l_pInstanceMesh->GetStaticMesh();
  // -------------------------------------------------------------------


  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);

  
  const vector<uint32> l_vIndexBuff = l_pStaticMesh->GetIndexBuffer();
  const vector<Vect3f> l_vVertexBuff = l_pStaticMesh->GetVertexBuffer();
  CPhysicCookingMesh* l_pCookingMesh = CORE->GetPhysicsManager()->GetCookingMesh();
  l_pCookingMesh->CreatePhysicMesh(l_vVertexBuff,l_vIndexBuff,_pEntity->GetName());
  NxTriangleMesh* l_pMesh = l_pCookingMesh->GetPhysicMesh(_pEntity->GetName());

  m_pPhysXMesh = new CPhysicActor(m_pPhysXData);
  if(_fDensity > 0)
  {
    m_pPhysXMesh->CreateBody(_fDensity);
  }

  m_pPhysXMesh->AddMeshShape(l_pMesh,v3fZERO,_iCollisionMask);


  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXMesh);
  m_pPhysXMesh->SetMat44( m_pObject3D->GetMat44() );

  SetOk(true);
  return IsOk();
}

void CComponentPhysXMesh::UpdatePostPhysX(float _fDeltaTime)
{
  assert(IsOk());
  Mat44f l_mat;
  m_pPhysXMesh->GetMat44(l_mat);
  m_pObject3D->SetMat44(l_mat);
}


void CComponentPhysXMesh::Release(void)
{
  assert(IsOk());
  //treure la box del physic manager
  CORE->GetPhysicsManager()->ReleasePhysicActor(m_pPhysXMesh);

  CHECKED_DELETE(m_pPhysXMesh);
  CHECKED_DELETE(m_pPhysXData);
}
