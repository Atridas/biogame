#include "ComponentIABrain.h"
#include "Core.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "ComponentObject3D.h"
#include "ComponentLaser.h"
#include "cal3d\cal3d.h"


bool CComponentIABrain::Init(CGameEntity* _pEntity, const string& _szPlayerEntityName)
{
  assert(_pEntity);
  SetEntity(_pEntity);

  m_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);

  SetOk(true);
  return IsOk();
}


void CComponentIABrain::Shoot()
{

  CComponentRenderableObject* l_pCR = GetEntity()->GetComponent<CComponentRenderableObject>();
  CRenderableAnimatedInstanceModel* l_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCR->GetRenderableObject());
  CAnimatedInstanceModel *l_pAnimatedInstanceModel = l_pRAIM->GetAnimatedInstanceModel();

  // ---------------------------------------------------------------------------------------------------------
  Vect3f l_vPlayerPos = m_pPlayer->GetComponent<CComponentObject3D>()->GetPosition();

  l_vPlayerPos += Vect3f(0, .5f, 0);
  // ---------------------------------------------------------------------------------------------------------
  CalSkeleton *l_pSkeleton = l_pAnimatedInstanceModel->GetAnimatedCalModel()->getSkeleton();
  CalCoreSkeleton *l_pCoreSkeleton = l_pSkeleton->getCoreSkeleton();
  CalBone* l_pBone = l_pSkeleton->getBone( l_pCoreSkeleton->getCoreBoneId("Bip01 R Hand") );

  CalVector l_vTanslationBone = l_pBone->getTranslationAbsolute();

  Mat44f l_mat = l_pCR->GetRenderableObject()->GetMat44();
  //Mat44f l_mat = m_pObject3D->GetMat44();

  Vect4f l_vAux(-l_vTanslationBone.x, l_vTanslationBone.y, l_vTanslationBone.z, 1);

  l_vAux = l_mat * l_vAux;

  Vect3f l_vMyHand(l_vAux.x / l_vAux.w, l_vAux.y / l_vAux.w, l_vAux.z / l_vAux.w);

  // ------------------------------------------------------------------------------------------------------
  Vect3f l_vPos = l_vMyHand;//l_pPlayerPos + Vect3f(0, .75f, 0);
  Vect3f l_vDir = (l_vPlayerPos - l_vMyHand).GetNormalized();

  //l_vPos += l_vDir;

  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = 0;

  CPhysicsManager *l_pPM = CORE->GetPhysicsManager();

  l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_pUserData,l_CInfo);

  if( l_pUserData )
  {
    Vect3f l_vCenterPoint = l_CInfo.m_CollisionPoint;

    CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
    (new CComponentLaser())->Init(l_pLaser,
                                  Vect3f(l_vMyHand.x,l_vMyHand.y,l_vMyHand.z),
                                  l_vCenterPoint,
                                  1.f);

    if(l_pUserData->GetEntity())
    {
      SEvent l_impacte;
      l_impacte.Msg = SEvent::REBRE_IMPACTE;
      l_impacte.Info[0].Type = SEventInfo::FLOAT;
      l_impacte.Info[0].f    = 20.f;
      l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
      l_impacte.Sender = GetEntity()->GetGUID();

      CORE->GetEntityManager()->SendEvent(l_impacte);
    }
  }
  else
  {
    Vect3f l_vCenterPoint = l_vMyHand + l_vDir * 100;

    CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
    (new CComponentLaser())->Init(l_pLaser,
                                  l_vMyHand,
                                  l_vCenterPoint,
                                  1.f);
  }
}
