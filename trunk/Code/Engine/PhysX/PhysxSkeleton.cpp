#include "PhysicsManager.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include <cal3d/cal3d.h>

bool CPhysxSkeleton::Init(const string& _szFileName, CalModel* _pCalModel)
{

  SetSkeleton(_pCalModel->getSkeleton());
  vector<CalBone*> l_vLlistaBones = m_pCalSkeleton->getVectorBone();

  //Inicialitzem l'estructura del PhysXSkeleton i creem les instancies dels PhysXBones.
  //Tindrem ja tota la info relativa de bones, pares i fills.
  for (size_t i=0;i<l_vLlistaBones.size();++i)
  {
    CPhysxBone* l_pPhysXBone = new CPhysxBone();
    CalBone* l_pBone = l_vLlistaBones[i];
    l_pPhysXBone->Init(l_pBone);
    m_vBones.push_back(l_pPhysXBone);
  }




  //Funcions de configuracio!!
  InitBoneMatrices();
  InitPhysXActors();
  InitPhysXJoints();

  SetOk(true);
  return IsOk();
}


void CPhysxSkeleton::InitBoneMatrices()
{

  for (size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->InitBoneMatrix();
  }


}


void CPhysxSkeleton::InitPhysXActors()
{

  for (size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->InitPhysXActor();
  }


}

void CPhysxSkeleton::InitPhysXJoints()
{
  for (size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->InitPhysXJoint();
  }
}

void CPhysxSkeleton::Release()
{

}

void CPhysxSkeleton::UpdateCal3dFromPhysx()
{


}