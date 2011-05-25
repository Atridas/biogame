#include "PhysicsManager.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include <cal3d/cal3d.h>

bool CPhysxSkeleton::Init(const string& _szFileName, CalModel* _pCalModel)
{
  SetOk(true);
  return IsOk();
}

void CPhysxSkeleton::Release()
{

}

void CPhysxSkeleton::UpdateCal3dFromPhysx()
{

}