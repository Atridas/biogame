
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

bool CComponentPhysXMesh::Init(CGameEntity* _pEntity, float _fDensity, int _iCollisionGroup)
{
  assert(_pEntity->IsOk());
  SetOk(false);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats


  //Agafem el static mesh -----------------------------------------
  CComponentRenderableObject* l_pComponentRenderableObject = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  if(!l_pComponentRenderableObject)
  {
    return false;
  }

  CRenderableObject* l_pRenderableObject = l_pComponentRenderableObject->GetRenderableObject();
  CInstanceMesh* l_pInstanceMesh = dynamic_cast<CInstanceMesh*>(l_pRenderableObject);
  assert(l_pInstanceMesh);
  CStaticMesh* l_pStaticMesh = l_pInstanceMesh->GetStaticMesh();
  // -------------------------------------------------------------------


  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);
  
  const vector<uint32> l_vIndexBuff = l_pStaticMesh->GetIndexBuffer();
  const vector<Vect3f> l_vVertexBuff = l_pStaticMesh->GetVertexBuffer();
  CPhysicCookingMesh* l_pCookingMesh = CORE->GetPhysicsManager()->GetCookingMesh();
  l_pCookingMesh->CreatePhysicMesh(l_vVertexBuff,l_vIndexBuff,_pEntity->GetName());
  NxTriangleMesh* l_pMesh = l_pCookingMesh->GetPhysicMesh(_pEntity->GetName());

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  if(_fDensity > 0)
  {
    m_pPhysXActor->CreateBody(_fDensity);
  }

  m_pPhysXActor->AddMeshShape(l_pMesh,v3fZERO,_iCollisionGroup);


  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );
  
  SetEntity(_pEntity);
  SetOk(true);
  return IsOk();
}
