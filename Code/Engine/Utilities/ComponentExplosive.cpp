#include "ComponentExplosive.h"
#include "PhysicsManager.h"
#include "ComponentObject3D.h"
#include "Core.h"

CComponentExplosive* CComponentExplosive::AddToEntity(CGameEntity *_pEntity)
{
  CComponentExplosive *l_pComp = new CComponentExplosive();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity))
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

bool CComponentExplosive::Init(CGameEntity *_pEntity)
{
  SetOk(true);
  return IsOk();
}

void CComponentExplosive::Explode(Vect3f _vPos, float _fRadius)
{
    vector<CPhysicUserData*> l_vImpactObjects;
    CPhysicsManager *l_pPM = PHYSICS_MANAGER;

    l_pPM->OverlapSphereActor(_fRadius,_vPos,l_vImpactObjects,l_pPM->GetCollisionMask(ECG_FORCE));

    vector<CPhysicUserData*>::iterator l_itUserData;
    vector<CPhysicUserData*>::iterator l_itUserDataEnd = l_vImpactObjects.end();

    set<CGameEntity*> l_vImpactEntities;

    for(l_itUserData = l_vImpactObjects.begin(); l_itUserData != l_itUserDataEnd; ++l_itUserData)
    {
      CPhysicUserData* l_pUserData = *l_itUserData;
      l_vImpactEntities.insert(l_pUserData->GetEntity());
    }

    set<CGameEntity*>::iterator l_itEntity;
    set<CGameEntity*>::iterator l_itEntityEnd = l_vImpactEntities.end();

    //missatge de força
    Vect3f l_v = GetEntity()->GetComponent<CComponentObject3D>()->GetPosition();
    SEvent l_impacte;
    l_impacte.Msg = SEvent::REBRE_FORCE;
    l_impacte.Info[0].Type = SEventInfo::FLOAT;
    l_impacte.Info[0].f    = 100;
    l_impacte.Info[1].Type = SEventInfo::VECTOR;
    l_impacte.Info[1].v.x    = l_v.x;
    l_impacte.Info[1].v.y    = l_v.y;
    l_impacte.Info[1].v.z    = l_v.z;
    l_impacte.Sender = GetEntity()->GetGUID();


    for(l_itEntity = l_vImpactEntities.begin(); l_itEntity != l_itEntityEnd; ++l_itEntity)
    {
      CGameEntity* l_pEntity = *l_itEntity;

      if(GetEntity() != l_pEntity)
      {
        l_impacte.Receiver = l_pEntity->GetGUID();

        ENTITY_MANAGER->SendEvent(l_impacte);
      }
    }
}