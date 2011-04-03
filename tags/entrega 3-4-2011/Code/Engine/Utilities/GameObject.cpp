#include "GameObject.h"
#include "PhysicActor.h"
#include "RenderableObject.h"


void  CGameObject::Update(float _fElapsedTime)
{
  if (GetType()=="BoundingBox")
  {
    UpdateBoundingBox(_fElapsedTime);
  }
  else if (GetType()=="BoundingSphere")
  {
    UpdateBoundingSphere(_fElapsedTime);
  }
  else if (GetType()=="mesh")
  {
    UpdateMesh(_fElapsedTime);
  }
  
}

void  CGameObject::Init(CRenderableObject* _pRO,CPhysicActor* _pPA, string _szType)
{
  m_pRenderableObject = _pRO;
  m_pPhysXActor = _pPA;
  m_pUserData = m_pPhysXActor->GetUserData();
  SetType(_szType);
}

void  CGameObject::UpdateMesh(float _fElapsedTime)
{
  Mat44f l_vPhysXMat;
  m_pPhysXActor->GetMat44(l_vPhysXMat);
  m_pRenderableObject->SetMat44(l_vPhysXMat);
}

void  CGameObject::UpdateBoundingBox(float _fElapsedTime)
{
  Mat44f l_vPhysXMat,l_vMt,l_vMtotal;
  m_pPhysXActor->GetMat44(l_vPhysXMat);

  l_vMt.SetIdentity();
  l_vMt.Translate(Vect3f(-m_pRenderableObject->GetBoundingBox()->GetMiddlePoint().x,
                         -m_pRenderableObject->GetBoundingBox()->GetMiddlePoint().y,
                         -m_pRenderableObject->GetBoundingBox()->GetMiddlePoint().z));

  l_vMtotal = l_vPhysXMat*l_vMt;

  m_pRenderableObject->SetMat44(l_vMtotal);

  /*Mat44f l_vPhysXMat;
  m_pPhysXActor->GetMat44(l_vPhysXMat);
  m_pRenderableObject->SetMat44(l_vPhysXMat);*/

}

void  CGameObject::UpdateBoundingSphere(float _fElapsedTime)
{


}



void CGameObject::Release() 
{ 
  //CHECKED_DELETE(m_pPhysXActor)
}; 