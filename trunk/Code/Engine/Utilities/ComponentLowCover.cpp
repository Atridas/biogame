#include "ComponentObject3D.h"
#include "ComponentLowCover.h"
#include "Core.h"
#include "PhysicActor.h"
#include "ComponentPhysXBox.h"
#include "PhysicsManager.h"

bool CComponentLowCover::Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  return CComponentCover::Init(_pEntity,_vSize,_iNumPlaces);
}

void CComponentLowCover::ReceiveEvent(const SEvent& _Event)
{

}