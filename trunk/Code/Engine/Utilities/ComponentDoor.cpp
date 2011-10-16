#include "ComponentDoor.h"
#include "ComponentObject3D.h"
#include "ComponentRenderableObject.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "Core.h"

CComponentDoor* CComponentDoor::AddToEntity(CGameEntity *_pEntity, bool _bOpen, Vect3f _vSize, const float _fOpenTime, const float _fCloseTime)
{
  CComponentDoor *l_pComp = new CComponentDoor();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _bOpen, _vSize, _fOpenTime, _fCloseTime))
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

bool CComponentDoor::Init(CGameEntity* _pEntity, bool _bOpen, Vect3f _vSize, const float _fOpenTime, const float _fCloseTime)
{
  m_vSizeBox = _vSize;
  m_fOpenTime = _fOpenTime;
  m_fCloseTime = _fCloseTime;

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>();
  assert(m_pObject3D);

  CComponentRenderableObject* l_pComponentRenderableObject = _pEntity->GetComponent<CComponentRenderableObject>();
  assert(l_pComponentRenderableObject);

  float l_fTotalHeight = _vSize.y * 0.5f;

  l_pComponentRenderableObject->m_fHeightAdjustment = -l_fTotalHeight;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;
  l_pComponentRenderableObject->m_bBlockPitchRoll = false;

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  m_pPhysXActor->CreateBody(100.0f);

  m_pPhysXActor->AddBoxSphape(_vSize*0.5f,v3fZERO , v3fZERO, NULL, ECG_ESCENARI);

  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);

  m_pPhysXActor->SetKinematic(true);

  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  SetActive(!_bOpen);

  SetOk(true);
  return IsOk();
}

void CComponentDoor::Enable(void)
{
  if(!m_bBlock)
  {
    m_pPhysXActor->Activate(true);
    m_pPhysXActor->SetGlobalPosition(m_pObject3D->GetPosition());
  }
}

void CComponentDoor::Disable(void)
{
  if(!m_bBlock)
  {
    m_pPhysXActor->Activate(false);
    m_pPhysXActor->SetGlobalPosition(m_pObject3D->GetPosition() - Vect3f(0.0f,-1000.0f,0.0f));
  }
}

void CComponentDoor::Release()
{
  CORE->GetPhysicsManager()->ReleasePhysicActor(m_pPhysXActor);

  CHECKED_DELETE(m_pPhysXActor);
  CHECKED_DELETE(m_pPhysXData);
}
