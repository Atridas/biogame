#include "GameObject.h"
#include "PhysicActor.h"
#include "RenderableObject.h"


void  CGameObject::Update(float _fElapsedTime)
{
  Mat44f l_vPhysXMat,l_vMt,l_vMtotal;
  m_pPhysXActor->GetMat44(l_vPhysXMat);

  l_vMt.SetIdentity();
  l_vMt.Translate(Vect3f(0.0f,-m_pRenderableObject->GetBoundingBox()->GetMiddlePoint().y,0.0f));
  l_vMtotal = l_vPhysXMat*l_vMt;

  m_pRenderableObject->SetMat44(l_vMtotal);
}

void  CGameObject::Init(CRenderableObject* _pRO,CPhysicActor* _pPA)
{
  m_pRenderableObject = _pRO;
  m_pPhysXActor = _pPA;
  m_pUserData = m_pPhysXActor->GetUserData();
}