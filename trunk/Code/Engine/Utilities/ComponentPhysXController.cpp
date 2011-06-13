
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "PhysicController.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "ComponentPhysXController.h"

bool CComponentPhysXController::Init(CGameEntity *_pEntity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 _iCollisionMask 
            )
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_pMovement = _pEntity->GetComponent<CComponentMovement>(ECT_MOVEMENT);
  assert(m_pMovement); //TODO fer missatges d'error més elavorats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colGREEN);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXController = new CPhysicController(
                                  radius, height, slope, skinwidth, stepOffset, _iCollisionMask,
                                  m_pPhysXData,
                                  m_pObject3D->GetPosition());
  
  m_pPhysXController->SetPitch(m_pObject3D->GetPitch());
  m_pPhysXController->SetYaw  (m_pObject3D->GetYaw()  );
  m_pPhysXController->SetRoll (m_pObject3D->GetRoll() );

  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysXController);

  SetOk(true);
  return IsOk();
}

void CComponentPhysXController::UpdatePrePhysX(float _fDeltaTime)
{
  m_pPhysXController->Move(m_pMovement->m_vMovement, _fDeltaTime);
}

void CComponentPhysXController::UpdatePostPhysX(float _fDeltaTime)
{
  Vect3f l_vPhy2 = m_pPhysXController->GetPosition();
  m_pObject3D->SetPosition(l_vPhy2);
}


void CComponentPhysXController::Release(void)
{
  //treure controller del physic manager
  CORE->GetPhysicsManager()->ReleasePhysicController(m_pPhysXController);

  CHECKED_DELETE(m_pPhysXController);
  CHECKED_DELETE(m_pPhysXData);
}
