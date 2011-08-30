#pragma once
#ifndef __PHYSX_COLLISION_ENTITY_CONTROLLER__
#define __PHYSX_COLLISION_ENTITY_CONTROLLER__

#include "PhysicCollisionReport.h"

class CPhysXCollisionEntityController : public CPhysicCollisionReport
{
public:

  virtual void	OnStartTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape);
  virtual	void	OnTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape);
  virtual	void	OnEndTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape);
  virtual void	OnStartTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape);
  virtual	void	OnTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape);
  virtual	void	OnEndTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape);
};

#endif
