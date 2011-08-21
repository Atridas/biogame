#include "ComponentArma.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "ComponentRenderableObject.h"
#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "Component3rdPSCamera.h"
#include "Camera.h"
#include "RenderableAnimatedInstanceModel.h"
#include "Core.h"
#include "cal3d\cal3d.h"


CComponentArma* CComponentArma::AddToEntity(CGameEntity *_pEntity, const string& _szMeshName)
{
  CComponentArma *l_pComp = new CComponentArma();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szMeshName))
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


bool CComponentArma::Init(CGameEntity* _pEntity, const string& _szMeshName)
{
  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();
  m_pRenderableObject = l_pROM->AddMeshInstance(_szMeshName,_szMeshName);
  m_pRenderableObject->SetVisible(m_bActive);

  m_pAnimatedModel = (CRenderableAnimatedInstanceModel*)_pEntity->GetComponent<CComponentRenderableObject>()->GetRenderableObject();

  SetOk(true);
  return IsOk();
}

void CComponentArma::Release()
{
  //TODO 
}


void CComponentArma::SetActive(bool _bActive)
{
  if(m_bActive != _bActive)
  {
    m_bActive = _bActive;
    
    if(m_bActive)
    {
      m_bActive = true;
      m_pRenderableObject->SetVisible(true);
    }
    else
    {
      m_bActive = false;
      m_pRenderableObject->SetVisible(false);
    }
  }
}

void CComponentArma::PostUpdate(float _fDeltaTime)
{
  if(m_bActive)
  {
    
    CalSkeleton* l_pSkeleton = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
    //l_pSkeleton->calculateBoundingBoxes();
  
    int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Hand");
    CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
  
    Mat44f l_vMat = CPhysxBone::GetBoneLeftHandedAbsoluteTransformation(l_pBone);
    Mat44f l_vTotal = m_pAnimatedModel->GetMat44()*l_vMat;

    if (m_pRenderableObject != 0)
    {
      m_pRenderableObject->SetMat44(l_vTotal);
    }

    //CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
    //CCamera* l_pCamera = GetEntity()->GetComponent<CComponent3rdPSCamera>(ECT_3RD_PERSON_SHOOTER_CAMERA)->GetCamera();
    //const Vect3f l_PosCamera = l_pCamera->GetEye();
    //const Vect3f& l_DirCamera = l_pCamera->GetDirection().Normalize();
    //CPhysicUserData* l_pUserData = 0;
  
    //SCollisionInfo l_CInfo;
    //l_pUserData = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,l_pPhysManager->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);

    //Mat44f l_vMat;
    //l_vMat.SetIdentity();
    //l_vMat.Translate(l_CInfo.m_CollisionPoint);
    //
    //if (m_pRenderableObject != 0)
    //{
    //  m_pRenderableObject->SetMat44(l_vMat);
    //}




  }
}