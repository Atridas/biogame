#include "ComponentObject3D.h"
#include "ComponentLowCover.h"
#include "Core.h"
#include "PhysicActor.h"
#include "ComponentPhysXBox.h"
#include "PhysicsManager.h"


CComponentLowCover* CComponentLowCover::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  CComponentLowCover *l_pComp = new CComponentLowCover();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vSize, _iNumPlaces))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentLowCover::Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  return CComponentCover::Init(_pEntity,_vSize,_iNumPlaces);
}
