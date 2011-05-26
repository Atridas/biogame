#include "PhysxBone.h"
#include <cal3d/cal3d.h>

bool CPhysxBone::Init(CalBone* _pBone)
{
  SetCalBone(_pBone);
  CalCoreBone* l_pCoreBone = m_pCalBone->getCoreBone();
  m_iParentID = l_pCoreBone->getParentId();

  if (m_iParentID != -1)
  {
    m_bRoot = false;
  }

  // Afegir Child ID.
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = l_pCoreBone->getListChildId().begin(); iteratorChildId != l_pCoreBone->getListChildId().end(); ++iteratorChildId)
  {
    m_vChildListID.push_back(*iteratorChildId);
  }

  SetOk(true);
  return IsOk();
}

void CPhysxBone::Release()
{
  CHECKED_DELETE(m_pActor);
  CHECKED_DELETE(m_pBoneUserData);
  m_vChildListID.clear();
  
}

void CPhysxBone::UpdateCal3dFromPhysx()
{

}


//Funcions per configurar el Ragdoll
void CPhysxBone::InitBoneMatrix()
{
 
}

void CPhysxBone::InitPhysXActor()
{

}

void CPhysxBone::InitPhysXJoint()
{

}

