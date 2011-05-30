#include "PhysxBone.h"
#include <cal3d/cal3d.h>
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "PhysicController.h"
#include "PhysicFixedJoint.h"
#include "AnimatedModelManager.h"
#include "RenderManager.h"
#include "Core.h"
#include "base.h"

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

void CPhysxBone::Load(float _fDensity, string _szType, Vect3f _fMiddlePoint,Vect3f _vSize, string _szName)
{

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();
  Mat44f l_vMatActor;
  l_vMatActor = GetBoneLeftHandedAbsoluteTransformation(m_pCalBone);

  D3DXMATRIX l_D3DXRotation;

  CalQuaternion l_vQuaternionAbsolute = m_pCalBone->getRotationAbsolute();
  //l_vQuaternionAbsolute.x = -l_vQuaternionAbsolute.x; 
  //l_vQuaternionAbsolute.y = -l_vQuaternionAbsolute.y; 
  //l_vQuaternionAbsolute.z = -l_vQuaternionAbsolute.z; 

  D3DXMatrixRotationQuaternion(&l_D3DXRotation,(CONST D3DXQUATERNION*)& l_vQuaternionAbsolute);
  CalVector l_vTranslationBoneSpace = m_pCalBone->getTranslationAbsolute();
  l_D3DXRotation._14 = l_vTranslationBoneSpace.x;
  l_D3DXRotation._24 = l_vTranslationBoneSpace.y;
  l_D3DXRotation._34 = l_vTranslationBoneSpace.z;


  Mat44f l_MyMat(l_D3DXRotation);
  l_MyMat.Transpose();
  l_vMatActor=l_MyMat;

  if (_szType == "box")
  {
    CPhysicUserData* l_pUserData = new CPhysicUserData(_szName);
    l_pUserData->SetPaint(true);
    l_pUserData->SetColor(colGREEN);
    CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
		l_pActor->AddBoxSphape(Vect3f(_vSize.x,_vSize.z,_vSize.y)*0.5f,_fMiddlePoint,NULL,GROUP_COLLIDABLE_NON_PUSHABLE);
    l_pActor->CreateBody(_fDensity);

    l_pPM->AddPhysicActor(l_pActor);
    l_pActor->SetMat44(l_vMatActor);


    m_pActor = l_pActor;
    m_pBoneUserData = l_pUserData;
      
  }

  if (_szType == "sphere")
  {
    CPhysicUserData* l_pUserData = new CPhysicUserData(_szName);
    l_pUserData->SetPaint(true);
    l_pUserData->SetColor(colYELLOW);
    CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
    l_pActor->AddSphereShape(_vSize.x*0.5f,v3fZERO,NULL,GROUP_COLLIDABLE_NON_PUSHABLE);
    l_pActor->CreateBody(_fDensity);

    l_pPM->AddPhysicActor(l_pActor);
    l_pActor->SetMat44(l_vMatActor);

    m_pActor = l_pActor;
    m_pBoneUserData = l_pUserData;
      
  }

}

void CPhysxBone::InitBoneMatrix()
{
 
}

void CPhysxBone::InitPhysXActor()
{

}

void CPhysxBone::InitPhysXJoint()
{

}


Mat44f CPhysxBone::GetBoneLeftHandedAbsoluteTransformation(CalBone* _pBone)
{
  //rotacio i translacio del bone (absoluta)
  CalVector l_vTranslation = _pBone->getTranslationAbsolute();
  CalQuaternion l_RotationQuaternion = _pBone->getRotationAbsolute();

  //passem el quaternion a left handed
  l_RotationQuaternion.x = -l_RotationQuaternion.x;
  l_RotationQuaternion.y = -l_RotationQuaternion.y;
  l_RotationQuaternion.z = -l_RotationQuaternion.z;

  //creem la matriu de transformacio Cal3d (absolute) -> Mat44f
  CalMatrix l_RotationMatrix(l_RotationQuaternion);

  return Mat44f(
              -l_RotationMatrix.dxdx  ,-l_RotationMatrix.dydx ,-l_RotationMatrix.dzdx ,-l_vTranslation.x,
              l_RotationMatrix.dxdy   ,l_RotationMatrix.dydy  ,l_RotationMatrix.dzdy  ,l_vTranslation.y,
              l_RotationMatrix.dxdz   ,l_RotationMatrix.dydz  ,l_RotationMatrix.dzdz  ,l_vTranslation.z,
              0.0f                    ,0.0f                   ,0.0f                   ,1.0f);
}
