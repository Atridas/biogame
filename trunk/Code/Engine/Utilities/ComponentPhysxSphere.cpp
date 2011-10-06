
#include "ComponentObject3D.h"
#include "ComponentRenderableObject.h"
#include "RenderableObject.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "Utils/Logger.h"

#include "ComponentPhysXSphere.h"

#define SHOOT_POWER 30.0f


CComponentPhysXSphere* CComponentPhysXSphere::AddToEntity(CGameEntity *_pEntity, float _fRadius,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            )
{
  CComponentPhysXSphere *l_pComp = new CComponentPhysXSphere();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity,_fRadius,
            _fPosX , _fPosY , _fPosZ,
            _fDensity, _iCollisionGroup))
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

CComponentPhysXSphere* CComponentPhysXSphere::AddToEntity(CGameEntity *_pEntity, float _fDensity, int _iCollisionGroup)
{
  CComponentPhysXSphere *l_pComp = new CComponentPhysXSphere();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fDensity, _iCollisionGroup))
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

bool CComponentPhysXSphere::Init(CGameEntity *_pEntity, float _fRadius, 
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            )
{
  m_fDensity = _fDensity;
  m_iCollisionGroup = _iCollisionGroup;

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elaborats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  if(_fDensity > 0)
  {
    m_pPhysXActor->CreateBody(_fDensity);
  }else{
    m_pPhysXActor->CreateBody(100.0f);
  }


  m_pPhysXActor->AddSphereShape(_fRadius,Vect3f( _fPosX , _fPosY , _fPosZ ), NULL, _iCollisionGroup);


  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);

  if(_fDensity == 0)
  {
    m_pPhysXActor->SetKinematic(true);
  }

  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  //m_vSize = Vect3f(_fSizeX, _fSizeY, _fSizeZ);

  SetOk(true);
  return IsOk();
}


bool CComponentPhysXSphere::Init(CGameEntity *_pEntity, float _fDensity, int _iCollisionGroup)
{
  m_fDensity = _fDensity;
  m_iCollisionGroup = _iCollisionGroup;

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  if(!m_pObject3D)
  {
    LOGGER->AddNewLog(ELL_WARNING,"CComponentPhysXSphere::Init  L'objecte no té el component CComponentObject3D.");
    return false;
  }

  CComponentRenderableObject* l_pComponentRenderableObject = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  if(!l_pComponentRenderableObject)
  {
    LOGGER->AddNewLog(ELL_WARNING,"CComponentPhysXSphere::Init  L'objecte no té el component CComponentRenderableObject.");
    return false;
  }

  CRenderableObject* l_pRenderableObject = l_pComponentRenderableObject->GetRenderableObject();
  CBoundingBox* l_pBB = l_pRenderableObject->GetBoundingBox();
  CBoundingSphere* l_pBS = l_pRenderableObject->GetBoundingSphere();

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  
  m_pPhysXActor->AddSphereShape(0.08f, v3fZERO, NULL, _iCollisionGroup);
  m_pPhysXActor->SetGlobalPosition(m_pObject3D->GetPosition());

  if(_fDensity > 0)
  {
    m_pPhysXActor->CreateBody(_fDensity);
  }else{
    m_pPhysXActor->CreateBody(100.0f);
  }

  //m_pPhysXActor->CreateBody(1);
  
  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);


  /*if(_fDensity == 0)
  {
    m_pPhysXActor->SetKinematic(true);
  }*/

  /*Mat44f l_vMat = m_pObject3D->GetMat44();
  m_pPhysXActor->SetMat44(l_vMat);*/

  
  
  SetOk(true);
  return IsOk();
}

void CComponentPhysXSphere::Reload()
{
  Release();
  Init(GetEntity(), m_fDensity, m_iCollisionGroup);
}

void CComponentPhysXSphere::ReceiveEvent(const SEvent& _Event)
{
  /*CComponentPhysXActor::ReceiveEvent(_Event);
  if(_Event.Msg == SEvent::REBRE_FORCE)
  {
    Vect3f l_vSenderPos = ENTITY_MANAGER->GetEntity(_Event.Sender)->GetComponent<CComponentObject3D>()->GetPosition();

    Vect3f l_vDirection = (GetEntity()->GetComponent<CComponentObject3D>()->GetPosition() - l_vSenderPos).Normalize();

    m_pPhysXActor->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),20.0f);

  }else if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    if(_Event.Info[1].Type == SEventInfo::VECTOR)
    {
      Vect3f l_vDir(_Event.Info[1].v.x,_Event.Info[1].v.y,_Event.Info[1].v.z);

      m_pPhysXActor->AddForceAtLocalPos(l_vDir,Vect3f(0.0f),SHOOT_POWER);
    }
  }*/
}

