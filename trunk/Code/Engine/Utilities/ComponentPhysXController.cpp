
#include "ComponentObject3D.h"
#include "PhysicController.h"
#include "PhysicsManager.h"
#include "Core.h"

#include <sstream>

#include "ComponentPhysXController.h"

bool CComponentPhysXController::Init(CGameEntity *_pEntity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 collisionGroups 
            )
{
  assert(_pEntity->IsOk());

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  SetEntity(_pEntity);
  stringstream l_SStream;

  l_SStream << "Entity " << _pEntity->GetGUID();

  m_pPhisXData = new CPhysicUserData(l_SStream.str().c_str());

  m_pPhysXController = new CPhysicController(
                                  radius, height, slope, skinwidth, stepOffset, collisionGroups,
                                  m_pPhisXData,
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
  Vect3f l_vMovementVector = m_pObject3D->GetPosition() - m_pPhysXController->GetPosition();

  m_pPhysXController->Move(l_vMovementVector, _fDeltaTime);
  Vect3f l_ControllerPos = m_pPhysXController->GetPosition();
  m_pObject3D->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));
}


void CComponentPhysXController::Release(void)
{
  //TODO treure controller del physic manager
  //CORE->GetPhysicsManager()

  CHECKED_DELETE(m_pPhysXController);
  CHECKED_DELETE(m_pPhisXData);
}
