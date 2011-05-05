
#include "ComponentObject3D.h"
#include "PhysicController.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "ComponentPhysXController.h"

bool CComponentPhysXController::Init(CGameEntity *_pEntity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 collisionGroups 
            )
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colGREEN);

  m_pPhysXController = new CPhysicController(
                                  radius, height, slope, skinwidth, stepOffset, collisionGroups,
                                  m_pPhysXData,
                                  m_pObject3D->GetPosition());
  
  m_pPhysXController->SetPitch(m_pObject3D->GetPitch());
  m_pPhysXController->SetYaw  (m_pObject3D->GetYaw()  );
  m_pPhysXController->SetRoll (m_pObject3D->GetRoll() );

  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysXController);

  SetOk(true);
  return IsOk();
}

void CComponentPhysXController::Update(float _fDeltaTime)
{
  if(m_pObject3D->m_bModified)
  {
    Vect3f l_vObj = m_pObject3D->GetPosition();
    Vect3f l_vPhy = m_pPhysXController->GetPosition();
    Vect3f l_vMovementVector = l_vObj - l_vPhy;

    m_pPhysXController->Move(l_vMovementVector, _fDeltaTime);
  }
  else
  {
    m_pPhysXController->Move(Vect3f(0,0,0), _fDeltaTime);
  }
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
