#pragma once
#ifndef __PHYSX_TRIGGER_ENTITY_CONTROLLER__
#define __PHYSX_TRIGGER_ENTITY_CONTROLLER__

#include "PhysicTriggerReport.h"

class CPhysXTriggerEntityController:
  public CPhysicTriggerReport

{
	//---CPhysicTrigger Interface---
	virtual void	OnEnter		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
	virtual	void	OnLeave		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
};

#endif