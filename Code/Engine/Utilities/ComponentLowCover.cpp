#include "ComponentObject3D.h"
#include "ComponentLowCover.h"
#include "Core.h"
#include "PhysicActor.h"
#include "ComponentPhysXBox.h"
#include "PhysicsManager.h"

bool CComponentLowCover::Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_iNumPlaces = _iNumPlaces;

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);

  m_pPhysXActor->AddBoxSphape(Vect3f( _vSize.x, _vSize.y, _vSize.z), 
                                m_pObject3D->GetPosition(),
                                NULL, ECG_COBERTURES);

  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  return true;
}

void CComponentLowCover::ReceiveEvent(const SEvent& _Event)
{

}