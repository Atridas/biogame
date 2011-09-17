#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysXCollisionEntityController.h"
#include "ComponentCollisionReport.h"
#include "PhysicsManager.h"
#include "EntityDefines.h"

void CPhysXCollisionEntityController::OnStartTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape)
{
  CGameEntity* m_pEntity = _pEntity->GetEntity();
  CGameEntity* m_pOtherEntity   = _pOther_shape->GetEntity();

  if(m_pEntity && m_pOtherEntity)
  {
    CComponentCollisionReport *l_pCompReport = m_pEntity->GetComponent<CComponentCollisionReport>();
    if(l_pCompReport)
    {
      l_pCompReport->OnStartTouch(m_pOtherEntity);
    }
  }
}

void CPhysXCollisionEntityController::OnTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape)
{
  CGameEntity* m_pEntity = _pEntity->GetEntity();
  CGameEntity* m_pOtherEntity   = _pOther_shape->GetEntity();

  if(m_pEntity && m_pOtherEntity)
  {
    CComponentCollisionReport *l_pCompReport = m_pEntity->GetComponent<CComponentCollisionReport>();
    if(l_pCompReport)
    {
      l_pCompReport->OnTouch(m_pOtherEntity);
    }
  }
}

void CPhysXCollisionEntityController::OnEndTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape)
{
  CGameEntity* m_pEntity = _pEntity->GetEntity();
  CGameEntity* m_pOtherEntity   = _pOther_shape->GetEntity();

  if(m_pEntity && m_pOtherEntity)
  {
    CComponentCollisionReport *l_pCompReport = m_pEntity->GetComponent<CComponentCollisionReport>();
    if(l_pCompReport)
    {
      l_pCompReport->OnEndTouch(m_pOtherEntity);
    }
  }
}

void CPhysXCollisionEntityController::OnStartTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape)
{
  CGameEntity* m_pEntity = _pEntity->GetEntity();
  CGameEntity* m_pOtherEntity   = _pOther_shape->GetEntity();

  if(m_pEntity && m_pOtherEntity)
  {
    CComponentCollisionReport *l_pCompReport = m_pEntity->GetComponent<CComponentCollisionReport>();
    if(l_pCompReport)
    {
      l_pCompReport->OnStartTouchForceThreshold(m_pOtherEntity);
    }
  }
}

void CPhysXCollisionEntityController::OnTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape)
{
  CGameEntity* m_pEntity = _pEntity->GetEntity();
  CGameEntity* m_pOtherEntity   = _pOther_shape->GetEntity();

  if(m_pEntity && m_pOtherEntity)
  {
    CComponentCollisionReport *l_pCompReport = m_pEntity->GetComponent<CComponentCollisionReport>();
    if(l_pCompReport)
    {
      l_pCompReport->OnTouchForceThreshold(m_pOtherEntity);
    }
  }
}

void CPhysXCollisionEntityController::OnEndTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape)
{
  CGameEntity* m_pEntity = _pEntity->GetEntity();
  CGameEntity* m_pOtherEntity   = _pOther_shape->GetEntity();

  if(m_pEntity && m_pOtherEntity)
  {
    CComponentCollisionReport *l_pCompReport = m_pEntity->GetComponent<CComponentCollisionReport>();
    if(l_pCompReport)
    {
      l_pCompReport->OnEndTouchForceThreshold(m_pOtherEntity);
    }
  }
}
