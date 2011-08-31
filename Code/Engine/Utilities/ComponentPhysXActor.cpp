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

void CComponentPhysXActor::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    assert(_Event.Info[3].Type == SEventInfo::VECTOR);
    Vect3f l_vPos(_Event.Info[3].v.x, _Event.Info[3].v.y, _Event.Info[3].v.z);

    CGameEntity* l_pEmiterEntity = ENTITY_MANAGER->CreateEntity();
    CComponentObject3D::AddToEntity(l_pEmiterEntity)->SetPosition(l_vPos);
    CComponentEmiter  ::AddToEntity(l_pEmiterEntity, m_szParticulaImpacte, Vect3f(.5f,.5f,.5f));
    CComponentLifetime::AddToEntity(l_pEmiterEntity, 5.f);
  }
}
