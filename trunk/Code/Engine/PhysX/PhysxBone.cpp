#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysxBone.h"
#include <cal3d/cal3d.h>
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "PhysicController.h"
#include "PhysicFixedJoint.h"
#include "PhysicSphericalJoint.h"
#include "AnimatedModelManager.h"
#include "RenderManager.h"
#include "Core.h"
#include "base.h"
//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
//---------------------//

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

  if (m_pActor != 0)
  {
    CalQuaternion l_vRotate;
    NxQuat l_pQuat = m_pActor->GetPhXActor()->getGlobalOrientationQuat();
    l_vRotate.x = -l_pQuat.x;
    l_vRotate.y = -l_pQuat.y;
    l_vRotate.z = -l_pQuat.z;
    l_vRotate.w = l_pQuat.w;

    m_pCalBone->setRotation(l_vRotate);
  }
}


//Funcions per configurar el Ragdoll

void CPhysxBone::Load(float _fDensity, string _szType, Vect3f _fMiddlePoint,Vect3f _vSize, string _szName)
{

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();
  Mat44f l_vMatActor;
  l_vMatActor = GetBoneLeftHandedAbsoluteTransformation(m_pCalBone);
  m_vMiddlePoint = _fMiddlePoint;

  //l_vMatActor.m00 = -l_vMatActor.m00;
  //l_vMatActor.m01 = -l_vMatActor.m01;
  //l_vMatActor.m02 = -l_vMatActor.m02;
  //l_vMatActor.m03 = -l_vMatActor.m03;

  if (_szType == "box")
  {
    CPhysicUserData* l_pUserData = new CPhysicUserData(_szName);
    l_pUserData->SetPaint(true);
    l_pUserData->SetColor(colGREEN);
    CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
		l_pActor->AddBoxSphape(Vect3f(_vSize.x,_vSize.z,_vSize.y)*0.5f,Vect3f(_fMiddlePoint.x,0.0f,0.0f),NULL,GROUP_COLLIDABLE_PUSHABLE);
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
    l_pActor->AddSphereShape(_vSize.x*0.5f,Vect3f(_fMiddlePoint.x,0.0f,0.0f),NULL,GROUP_COLLIDABLE_PUSHABLE);
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

void CPhysxBone::InitPhysXJoint(CPhysxBone* _pParent)
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

  Mat33f l_Rotation = Mat33f( l_RotationMatrix.dxdx   ,l_RotationMatrix.dydx  ,l_RotationMatrix.dzdx,
                              l_RotationMatrix.dxdy   ,l_RotationMatrix.dydy  ,l_RotationMatrix.dzdy,
                              l_RotationMatrix.dxdz   ,l_RotationMatrix.dydz  ,l_RotationMatrix.dzdz);

  float l_fAngleX = FLOAT_PI_VALUE - l_Rotation.GetAngleX();
  float l_fAngleY = FLOAT_PI_VALUE - l_Rotation.GetAngleY();
  float l_fAngleZ = FLOAT_PI_VALUE - l_Rotation.GetAngleZ();

  Mat44f l_Transform;
  l_Transform.SetIdentity();
  l_Transform.RotByAnglesYXZ(l_fAngleY,l_fAngleX,l_fAngleZ);
  l_Transform.Translate(Vect3f(-l_vTranslation.x,l_vTranslation.y,l_vTranslation.z));

  return l_Transform;
  
}




