#include "ComponentPhysXActor.h"

#include "PhysicActor.h"
#include "ComponentObject3D.h"
#include "Core.h"
#include "PhysicsManager.h"

void CComponentPhysXActor::UpdatePostPhysX(float _fDeltaTime)
{
  assert(IsOk());
  Mat44f l_mat;
  m_pPhysXActor->GetMat44(l_mat);
  m_pObject3D->SetMat44(l_mat);
}


void CComponentPhysXActor::Release(void)
{ 
  m_pObject3D = 0;

  //treure la box del physic manager
  CORE->GetPhysicsManager()->ReleasePhysicActor(m_pPhysXActor);

  CHECKED_DELETE(m_pPhysXActor);
  CHECKED_DELETE(m_pPhysXData);
}

void CComponentPhysXActor::Activate(bool _bActivate)
{
  if(m_pPhysXActor)
  {
    m_pPhysXActor->Activate(_bActivate);
  }
}