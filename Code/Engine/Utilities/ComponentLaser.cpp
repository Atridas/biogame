#include "ComponentLaser.h"
#include "RenderManager.h"
#include "ComponentObject3D.h"
#include "ComponentRenderableObject.h"
#include "Core.h"
#include "PhysicsManager.h"

#define LASER_RENDER_TIME 0.01f

CComponentLaser* CComponentLaser::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vDir, float _fDamage, uint32 _uiCollisionMask)
{
  CComponentLaser *l_pComp = new CComponentLaser();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vDir, _fDamage, _uiCollisionMask))
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

bool CComponentLaser::Init(CGameEntity* _pEntity, const Vect3f& _vDir, float _fDamage, uint32 _uiCollisionMask)
{
  l_pCO3D = _pEntity->GetComponent<CComponentObject3D>();
  if(!l_pCO3D)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentLaser::Init needs CComponentObject3D");
  }

  m_vDir            = _vDir.GetNormalized() ;
  m_fDamage         = _fDamage;
  m_fSpeed          = DEFAULT_LASER_SPEED;
  m_uiCollisionMask = _uiCollisionMask;

  SetOk(true);
  return IsOk();
}

void CComponentLaser::Update(float _fDeltaTime)
{
  Vect3f l_vPos = l_pCO3D->GetPosition();
  float l_fDistance = m_fSpeed * _fDeltaTime;

  CPhysicsManager *l_pPM = PHYSICS_MANAGER;
  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = l_pPM->RaycastClosestActor(l_vPos,m_vDir,m_uiCollisionMask,l_CInfo);

  
  if(l_pUserData && l_CInfo.m_fDistance <= l_fDistance)
  {
    CEntityManager* l_pEM = ENTITY_MANAGER;

    SEvent l_impacte;
    l_impacte.Msg = SEvent::REBRE_IMPACTE;
    l_impacte.Info[0].Type = SEventInfo::FLOAT;
    l_impacte.Info[0].f    = m_fDamage;
    l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
    l_impacte.Sender = GetEntity()->GetGUID();
    l_impacte.Info[1].Type = SEventInfo::VECTOR;
    l_impacte.Info[1].v.x = m_vDir.x;
    l_impacte.Info[1].v.y = m_vDir.y;
    l_impacte.Info[1].v.z = m_vDir.z;
    l_impacte.Info[2].Type = SEventInfo::PTR;
    l_impacte.Info[2].ptr = (void*)(l_pUserData->GetActor());
    l_impacte.Info[3].Type = SEventInfo::VECTOR;
    l_impacte.Info[3].v.x = l_CInfo.m_CollisionPoint.x;
    l_impacte.Info[3].v.y = l_CInfo.m_CollisionPoint.y;
    l_impacte.Info[3].v.z = l_CInfo.m_CollisionPoint.z;
    l_pEM->SendEvent(l_impacte);

    l_pEM->RemoveEntity(GetEntity());
  }
  else
  {
    l_vPos += m_vDir * l_fDistance;
    l_pCO3D->SetPosition(l_vPos);
  }

  if(m_fTime >= LASER_RENDER_TIME)
  {
    GetEntity()->GetComponent<CComponentRenderableObject>()->SetVisible(true);
  }

  m_fTime += _fDeltaTime;
}

void CComponentLaser::DebugRender(CRenderManager* _pRM)
{
  Mat44f mat;
  mat.SetIdentity();

  _pRM->SetTransform(mat);

  _pRM->DrawLine(l_pCO3D->GetPosition(), l_pCO3D->GetPosition() + m_vDir, colRED);
}