#include "ComponentMirilla.h"
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
#include "Core.h"


CComponentMirilla* CComponentMirilla::AddToEntity(CGameEntity *_pEntity, const string& _szMeshName)
{
  CComponentMirilla *l_pComp = new CComponentMirilla();
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


bool CComponentMirilla::Init(CGameEntity* _pEntity, const string& _szMeshName)
{
  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();
  m_pRenderableObject = l_pROM->AddMeshInstance(_szMeshName,_szMeshName);
  m_pRenderableObject->SetVisible(m_bActive);

  SetOk(true);
  return IsOk();
}

void CComponentMirilla::Release()
{
  //CHECKED_DELETE(m_pRagdoll);
}


void CComponentMirilla::SetActive(bool _bActive)
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

void CComponentMirilla::PostUpdate(float _fDeltaTime)
{
  if(m_bActive)
  {
    //

    CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
    CCamera* l_pCamera = GetEntity()->GetComponent<CComponent3rdPSCamera>(ECT_3RD_PERSON_SHOOTER_CAMERA)->GetCamera();
    const Vect3f l_PosCamera = l_pCamera->GetEye();
    const Vect3f& l_DirCamera = l_pCamera->GetDirection().Normalize();
    CPhysicUserData* l_pUserData = 0;
  
    SCollisionInfo l_CInfo;
    l_pUserData = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,l_pPhysManager->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);

    Mat44f l_vMat;
    l_vMat.SetIdentity();
    l_vMat.Translate(l_CInfo.m_CollisionPoint);
    
    if (m_pRenderableObject != 0)
    {
      m_pRenderableObject->SetMat44(l_vMat);
    }
  }
}
