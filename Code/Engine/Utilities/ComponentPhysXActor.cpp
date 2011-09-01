#include "ComponentPhysXActor.h"

#include "PhysicActor.h"
#include "ComponentObject3D.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "ComponentLifetime.h"
#include "ComponentEmiter.h"

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

Vect3f CComponentPhysXActor::GetPosition()
{
  if(m_pPhysXActor)
  {
    return m_pPhysXActor->GetPosition();
  }

  return Vect3f(0.0f);
}

void CComponentPhysXActor::SetPosition(Vect3f _vPos)
{
  if(m_pPhysXActor)
  {
    m_pPhysXActor->SetGlobalPosition(_vPos);
  }
}

void CComponentPhysXActor::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    assert(_Event.Info[3].Type == SEventInfo::VECTOR);
    Vect3f l_vPos(_Event.Info[3].v.x, _Event.Info[3].v.y, _Event.Info[3].v.z);

    ENTITY_MANAGER->InitParticles(m_szParticulaImpacte, l_vPos, Vect3f(.5f,.5f,.5f), 5.f);
  }
}
