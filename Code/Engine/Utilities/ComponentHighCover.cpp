#include "ComponentObject3D.h"
#include "ComponentHighCover.h"
#include "Core.h"
#include "PhysicActor.h"
#include "ComponentPhysXBox.h"
#include "PhysicsManager.h"

CComponentHighCover* CComponentHighCover::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  CComponentHighCover *l_pComp = new CComponentHighCover();
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

bool CComponentHighCover::Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces)
{
  return CComponentCover::Init(_pEntity,_vSize,_iNumPlaces);
}