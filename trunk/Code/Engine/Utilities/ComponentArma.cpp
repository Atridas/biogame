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
#include "RenderManager.h"


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

  m_pAnimatedModel = (CRenderableAnimatedInstanceModel*)_pEntity->GetComponent<CComponentRenderableObject>()->GetRenderableObject();

  if(!m_pRenderableObject || !m_pAnimatedModel)
  {
    SetOk(false);
  }else{
    SetOk(true);
  }
  
  return IsOk();
}

void CComponentArma::Release()
{

}

void CComponentArma::Enable()
{
  m_pRenderableObject->SetVisible(true);
}

void CComponentArma::Disable()
{
  m_pRenderableObject->SetVisible(false);
}


void CComponentArma::Update(float _fDeltaTime)
{
  CalSkeleton* l_pSkeleton = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
  
  int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Hand");
  CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
  
  Mat44f l_vMat = CPhysxBone::GetBoneLeftHandedAbsoluteTransformation(l_pBone);
  Mat44f l_vTotal = m_pAnimatedModel->GetMat44()*l_vMat;

  if (m_pRenderableObject != 0)
  {
    m_pRenderableObject->SetMat44(l_vTotal);
  }
}

Vect3f CComponentArma::GetPosition()
{
  if(m_pRenderableObject)
  {
    return m_pRenderableObject->GetMat44().GetPos();
  }

  return Vect3f(0.0f);
}

Vect3f CComponentArma::GetAimDirection()
{
  if(m_pRenderableObject)
  {
    Vect3f l_vDir(1.0f,0.0f,0.0f);

    Mat44f l_mTransform = m_pRenderableObject->GetMat44();
    Mat33f l_mRot;
    l_mRot.SetIdentity();
    l_mRot.SetRotByAnglesYXZ(l_mTransform.GetAngleY()-FLOAT_PI_VALUE,l_mTransform.GetAngleX(),l_mTransform.GetAngleZ());

    return l_mRot*l_vDir;
  }

  return Vect3f(1.0f,0.0f,0.0f);
}

void CComponentArma::DebugRender(CRenderManager* _pRM)
{
  Mat44f mat;
  mat.SetIdentity();

  _pRM->SetTransform(mat);

  _pRM->DrawLine(GetPosition(), GetPosition() + GetAimDirection()*3.0f, colRED);
}