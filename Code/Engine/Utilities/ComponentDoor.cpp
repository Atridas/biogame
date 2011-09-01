#include "ComponentDoor.h"
#include "ComponentObject3D.h"
#include "ComponentRenderableObject.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "Core.h"

CComponentDoor* CComponentDoor::AddToEntity(CGameEntity *_pEntity, bool _bOpen, Vect3f _vSize)
{
  CComponentDoor *l_pComp = new CComponentDoor();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _bOpen, _vSize))
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

bool CComponentDoor::Init(CGameEntity* _pEntity, bool _bOpen, Vect3f _vSize)
{
  m_bOpen = _bOpen;

  m_vSizeBox = _vSize;

  CComponentObject3D* l_pObject3D = _pEntity->GetComponent<CComponentObject3D>();
  assert(l_pObject3D);

  CComponentRenderableObject* l_pComponentRenderableObject = _pEntity->GetComponent<CComponentRenderableObject>();
  assert(l_pComponentRenderableObject);

  float l_fTotalHeight = _vSize.y * 0.5f;

  m_vPosBox = l_pObject3D->GetPosition();

  l_pComponentRenderableObject->m_fHeightAdjustment = -l_fTotalHeight;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;
  l_pComponentRenderableObject->m_bBlockPitchRoll = false;

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  m_pPhysXActor->CreateBody(100.0f);

  m_pPhysXActor->AddBoxSphape(_vSize*0.5f, v3fZERO, NULL, ECG_ESCENARI);

  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);

  m_pPhysXActor->SetKinematic(true);

  m_pPhysXActor->SetMat44( l_pObject3D->GetMat44() );

  SetOk(true);
  return IsOk();
}

bool CComponentDoor::Open()
{
  if(!m_bBlock)
  {
    m_bOpen = true;

    Vect3f a = m_pPhysXActor->GetPosition();
    m_pPhysXActor->SetGlobalPosition(Vect3f(0.0f,-1000.0f,0.0f));

    return true;
  }
  return false;
}

bool CComponentDoor::Close()
{
  if(!m_bBlock)
  {
    m_bOpen = false;

    m_pPhysXActor->SetGlobalPosition(m_vPosBox);

    return true;
  }
  return false;
}

void CComponentDoor::Release()
{
  CORE->GetPhysicsManager()->ReleasePhysicActor(m_pPhysXActor);

  CHECKED_DELETE(m_pPhysXActor);
  CHECKED_DELETE(m_pPhysXData);
}
