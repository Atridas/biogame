
#include "ComponentObject3D.h"
#include "ComponentRenderableObject.h"
#include "RenderableObject.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "Utils/Logger.h"

#include "ComponentPhysXBox.h"

bool CComponentPhysXBox::Init(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            )
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  if(_fDensity > 0)
  {
    m_pPhysXActor->CreateBody(_fDensity);
  }

  m_pPhysXActor->AddBoxSphape(Vect3f( _fSizeX * .5f, _fSizeY * .5f, _fSizeZ * .5f), 
                            Vect3f( _fPosX , _fPosY , _fPosZ ),
                            NULL, _iCollisionGroup);


  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );

  SetOk(true);
  return IsOk();
}


bool CComponentPhysXBox::Init(CGameEntity *_pEntity, float _fDensity, int _iCollisionGroup)
{
  assert(_pEntity->IsOk());

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  if(!m_pObject3D)
  {
    LOGGER->AddNewLog(ELL_WARNING,"CComponentPhysXBox::Init  L'objecte no té el component CComponentObject3D.");
    return false;
  }

  CComponentRenderableObject* l_pComponentRenderableObject = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  if(!l_pComponentRenderableObject)
  {
    LOGGER->AddNewLog(ELL_WARNING,"CComponentPhysXBox::Init  L'objecte no té el component CComponentRenderableObject.");
    return false;
  }

  CRenderableObject* l_pRenderableObject = l_pComponentRenderableObject->GetRenderableObject();
  CBoundingBox* l_pBB = l_pRenderableObject->GetBoundingBox();

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);
  m_pPhysXData->SetEntity(_pEntity);

  m_pPhysXActor = new CPhysicActor(m_pPhysXData);
  if(_fDensity > 0)
  {
    m_pPhysXActor->CreateBody(_fDensity);
  }

  m_pPhysXActor->AddBoxSphape(l_pBB->GetDimension() * .5f, 
                            l_pBB->GetMiddlePoint(),
                            NULL, _iCollisionGroup);


  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXActor);
  m_pPhysXActor->SetMat44( m_pObject3D->GetMat44() );
  

  SetEntity(_pEntity);
  SetOk(true);
  return IsOk();
}
