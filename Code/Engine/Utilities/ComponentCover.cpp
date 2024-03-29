#include "ComponentObject3D.h"
#include "ComponentCover.h"
#include "Core.h"
#include "PhysicActor.h"
#include "ComponentPhysXBox.h"
#include "PhysicsManager.h"




bool CComponentCover::Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error m�s elavorats

  m_iNumPlaces = _iNumPlaces;

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colCYAN);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);

  m_pPhysXActor->AddBoxSphape(_vSize*0.5f, v3fZERO,
                              m_pObject3D->GetBoundingBox()->GetMiddlePoint(),
                              NULL, ECG_COBERTURES);

  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  SetOk(true);
  return IsOk();
}

void CComponentCover::ReceiveEvent(const SEvent& _Event)
{

}