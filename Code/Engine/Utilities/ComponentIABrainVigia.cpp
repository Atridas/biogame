#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentIABrainVigia.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "ComponentObject3D.h"
#include "ComponentLaser.h"
#include "ComponentPhysXSphere.h"
#include "ComponentNavNode.h"
#include "ComponentVida.h"
#include "IAManager.h"
#include "GraphDefines.h"

#include "ScriptManager.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "PhysxBone.h"
#include "PhysicActor.h"
#include "cal3d\cal3d.h"
#include "Math/Quaternion.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Utils\MemLeaks.h"
#include "Utils\Logger.h"

#define SHOOT_POWER 10.0f

CComponentIABrainVigia* CComponentIABrainVigia::AddToEntity(CGameEntity *_pEntity, const string& _szPlayerEntityName, const string& _szOnDeathScript)
{
  CComponentIABrainVigia *l_pComp = new CComponentIABrainVigia();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szPlayerEntityName, _szOnDeathScript))
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

bool CComponentIABrainVigia::Init(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szOnDeathScript)
{
  m_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);

  m_szOnDeathScript = _szOnDeathScript;

  m_bDead = false;

  m_fTargetHeight = 1.5f;
  m_vTargetPosition = Vect3f(2.0f,2.5f,2.0f);

  m_PatrolZone.Init( Vect3f(0,1,0), Vect3f(5,3,5) );
  m_vPatrolDirection = Vect3f(0,0,1);
  m_vPatrolPosition = m_vTargetPosition;

  SetOk(true);
  return IsOk();
}

void CComponentIABrainVigia::ChooseNewPatrolPosition()
{
  float x = RandomNumber(m_PatrolZone.GetMin().x, m_PatrolZone.GetMax().x);
  float y = RandomNumber(m_PatrolZone.GetMin().y, m_PatrolZone.GetMax().y);
  float z = RandomNumber(m_PatrolZone.GetMin().z, m_PatrolZone.GetMax().z);

  m_vPatrolDirection = Vect3f(x,y,z) - m_vPatrolPosition;
  m_vPatrolDirection.Normalize();
  m_vPatrolPosition  = Vect3f(x,y,z);
}

void CComponentIABrainVigia::Shoot(float _fShootPrecision)
{
  CComponentObject3D* l_O3D = GetEntity()->GetComponent<CComponentObject3D>();

  Vect3f l_vPos = l_O3D->GetPosition();
  Vect3f l_vPlayerPos = m_pPlayer->GetComponent<CComponentObject3D>()->GetPosition() + Vect3f(0.0f,0.5f,0.0f);

  Vect3f l_vDir = (l_vPlayerPos - l_vPos).GetNormalized();

  CEntityManager* l_pEM = ENTITY_MANAGER;
  l_pEM->InitLaser(l_vPos,l_vDir,SHOOT_POWER, CORE->GetPhysicsManager()->GetCollisionMask(ECG_RAY_SHOOT));

  /*CComponentRenderableObject* l_pCR = GetEntity()->GetComponent<CComponentRenderableObject>();
  CRenderableAnimatedInstanceModel* l_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCR->GetRenderableObject());
  CAnimatedInstanceModel *l_pAnimatedInstanceModel = l_pRAIM->GetAnimatedInstanceModel();

  // ---------------------------------------------------------------------------------------------------------
  Vect3f l_vPlayerPos = m_pPlayer->GetComponent<CComponentObject3D>()->GetPosition();

  float l_fRandY = 0.15f * (rand()/(float)RAND_MAX * 2.0f - 1.0f);
  l_vPlayerPos += Vect3f(0, 0.5f + l_fRandY, 0);
  // ---------------------------------------------------------------------------------------------------------
  CalSkeleton *l_pSkeleton = l_pAnimatedInstanceModel->GetAnimatedCalModel()->getSkeleton();
  CalCoreSkeleton *l_pCoreSkeleton = l_pSkeleton->getCoreSkeleton();
  CalBone* l_pBone = l_pSkeleton->getBone( l_pCoreSkeleton->getCoreBoneId("Bip01 R Hand") );

  Mat44f l_vMat = CPhysxBone::GetBoneLeftHandedAbsoluteTransformation(l_pBone);
  Mat44f l_mTransform = l_pRAIM->GetMat44()*l_vMat;

  // ------------------------------------------------------------------------------------------------------
  Vect3f l_vPos = l_mTransform.GetPos();
  Vect3f l_vDir = (l_vPlayerPos - l_vPos).GetNormalized();
  
  l_vPos = l_vPos - 0.1f*l_vDir;

  float l_fRandYaw = _fShootPrecision * (rand()/(float)RAND_MAX * 2.0f - 1.0f);
  l_vDir.RotateY(l_fRandYaw);
  
  CEntityManager* l_pEM = ENTITY_MANAGER;
  l_pEM->InitParticles("disparar", l_vPos, Vect3f(.5f,.5f,.5f), 2.5f, l_vDir);
  l_pEM->InitLaser(l_vPos,l_vDir,SHOOT_POWER, CORE->GetPhysicsManager()->GetCollisionMask(ECG_RAY_SHOOT));*/

}

void CComponentIABrainVigia::ReceiveShoot(SEvent _sEvent)
{
  CPhysicActor* l_pActor = 0;

  if(_sEvent.Msg == SEvent::REBRE_IMPACTE)
  {
    if(_sEvent.Info[2].Type == SEventInfo::PTR)
    {
      l_pActor = (CPhysicActor*)_sEvent.Info[2].ptr;

      Vect3f l_vDir(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

      CComponentPhysXSphere* l_pSphere = GetEntity()->GetComponent<CComponentPhysXSphere>();
      float l_fRadius = l_pSphere->GetRadius();
      l_pActor->AddVelocityAtLocalPos(l_vDir, 2.0f*l_fRadius*Vect3f(Random01(),Random01(),Random01()) - l_fRadius, SHOOT_POWER);

    }else{
      LOGGER->AddNewLog(ELL_ERROR,"CComponentIABrain::ReciveShoot El missatge a Info[2] no es del tipus PTR");
      return;
    }

  }

}

void CComponentIABrainVigia::ReceiveForce(SEvent _sEvent)
{
  CComponentObject3D* l_O3D = GetEntity()->GetComponent<CComponentObject3D>();

  Vect3f l_vPos = l_O3D->GetPosition();
  Vect3f l_vSenderPos;
  Vect3f l_vDirection;

  CPhysicUserData* l_pUserData = 0;
  SCollisionInfo l_CInfo;
  CPhysicsManager *l_pPM = PHYSICS_MANAGER;

  if(_sEvent.Msg == SEvent::REBRE_FORCE)
  {
    if(_sEvent.Info[1].Type != SEventInfo::VECTOR)
    {
      return;
    }

    l_vSenderPos = Vect3f(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

    Vect3f l_vRayDir = (l_vSenderPos-l_vPos).Normalize();
    l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vRayDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);

    CGameEntity* l_entity1 = l_pUserData->GetEntity();
    CGameEntity* l_entity2 = ENTITY_MANAGER->GetEntity(_sEvent.Sender);

    if(!l_pUserData || l_entity1 != l_entity2)
    {
      return;
    }

    /* Morir?? millor treure vida al rebotar per les parets! mostrant xispes
    if(!m_bDead)
    {
      SEvent l_morir;
      l_morir.Msg = SEvent::MORIR;
      l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
      ENTITY_MANAGER->SendEvent(l_morir);
    }*/

    CComponentPhysXSphere* l_pSphere = GetEntity()->GetComponent<CComponentPhysXSphere>();

    l_vDirection = (l_vPos - l_vSenderPos).Normalize();
    float l_fRadius = l_pSphere->GetRadius();
    l_pSphere->GetActor()->AddVelocityAtLocalPos(l_vDirection, 2.0f*l_fRadius*Vect3f(Random01(),Random01(),Random01()) - l_fRadius, 50.0f);
  }
}

void CComponentIABrainVigia::Fly(bool _bFly)
{
  m_bFly = _bFly;
}

void CComponentIABrainVigia::LookAt(float l_fTimeDelta)
{
  const static float TAU_BLEND = .5f;

  //CComponentObject3D* l_O3D = GetEntity()->GetComponent<CComponentObject3D>();
  CComponentPhysXSphere* l_pSphere = GetEntity()->GetComponent<CComponentPhysXSphere>();
  Mat44f m44;
  l_pSphere->GetActor()->GetMat44(m44);
  
  Vect3f l_vDirection; // (_vPos - m44.GetTranslationVector()).Normalize();
  
  if(m_vPatrolDirection.SquaredLength() > 0)
    l_vDirection = m_vPatrolDirection.GetNormalized();
  else
    l_vDirection = Vect3f(0,0,1);

  float  l_fAngle;
  Vect3f l_vAxis;
  GetFastestRotationFromDirToDir(Vect3f(0,0,1), l_vDirection, l_vAxis, l_fAngle);


  Quat4f l_qDesiredRotation(l_vAxis, l_fAngle);


  Vect4f l_vCurrentDirection4 = m44 * Vect4f(0,0,1,0);
  Vect3f l_vCurrentDirection(l_vCurrentDirection4.x, l_vCurrentDirection4.y, l_vCurrentDirection4.z);
  GetFastestRotationFromDirToDir(Vect3f(0,0,1), l_vCurrentDirection, l_vAxis, l_fAngle);


  Quat4f l_qCurrentRotation(l_vAxis, l_fAngle);

  float d;
  if(l_fTimeDelta < TAU_BLEND)
    d = l_fTimeDelta / TAU_BLEND;
  else
    d = 1;
  l_qCurrentRotation.Blend(d, l_qDesiredRotation);

  l_qCurrentRotation.GetAxisAngle(l_vAxis, l_fAngle);
  
  Mat33f m;
  m.SetIdentity();
  m.FromAxisAngle(l_vAxis, l_fAngle);

  Mat44f m2(m);
  
  m2.SetPos(m44.GetTranslationVector());
  
  //l_O3D->SetMat44(m2);
  
  l_pSphere->GetActor()->SetMat44(m2);
  

  /*
  CComponentObject3D* l_O3D = GetEntity()->GetComponent<CComponentObject3D>();
  Vect3f l_vPos = l_O3D->GetPosition();
  Vect3f l_vDesiredDirection = (_vPos - l_vPos).Normalize();

  
  CComponentPhysXSphere* l_pSphere = GetEntity()->GetComponent<CComponentPhysXSphere>();
  
  Mat44f m;
  l_pSphere->GetActor()->GetMat44(m);
  Vect4f l_vDirection = m * Vect4f(0,0,1,0);
  
  Vect3f l_vAxisRotation;
  float  l_fAngleRotation;
  GetFastestRotationFromDirToDir(Vect3f(l_vDirection.x, l_vDirection.y, l_vDirection.z),
                                 l_vDesiredDirection, 
                                 l_vAxisRotation, l_fAngleRotation);
  
  Vect3f l_vAngularVelocity = l_pSphere->GetActor()->GetAngularVelocity();
  Vect3f l_vAngularError = l_vAxisRotation * l_fAngleRotation;

  
  float l_tn = 0.4f;
  float l_eps = 0;//1.f;

  //Vect3f l_vTorque = (l_vAngularError - 2 * l_tn * l_eps * l_vAngularVelocity)/(l_tn*l_tn);

  float  l_fAngleAngularVelocity = l_vAngularVelocity.Length();
  Vect3f l_vAxisAngularVelocity;
  if(l_fAngleAngularVelocity)
    l_vAxisAngularVelocity  = l_vAngularVelocity.GetNormalized();
  else
    l_vAxisAngularVelocity  = Vect3f(0,0,0);

  Vect3f l_vAxisTorque;
  float  l_fAngleTorque;
  ConcatenateRotations(l_vAxisRotation,        l_fAngleRotation, 
                       l_vAxisAngularVelocity, l_fAngleAngularVelocity * (- 2.f * l_tn * l_eps), 
                       l_vAxisTorque, l_fAngleTorque);
  l_fAngleTorque /= (l_tn*l_tn);

  l_pSphere->GetActor()->AddTorque(l_vAxisTorque * l_fAngleTorque);
  */

  /*
  static const float l_fSpeedTau = .5f;
  static const float l_fTorqueTau = .1f;

  CComponentObject3D* l_O3D = GetEntity()->GetComponent<CComponentObject3D>();
  Vect3f l_vPos = l_O3D->GetPosition();
  Vect3f l_vDesiredDirection = (_vPos - l_vPos).Normalize();
  
  Quat4f l_qDesiredOritentation = ShortestArc( Vect3f(0,0,1), l_vDesiredDirection);
  
  CComponentPhysXSphere* l_pSphere = GetEntity()->GetComponent<CComponentPhysXSphere>();
  
  Mat44f m;
  l_pSphere->GetActor()->GetMat44(m);
  Mat33f m33(m.m00, m.m01, m.m02,
             m.m10, m.m11, m.m12,
             m.m20, m.m21, m.m22);
  Quat4f l_qCurrentOrientation(m33);

  Quat4f l_qDesiredInverted = l_qDesiredOritentation.GetInverted();
  
  ///////////////////
  //Vect4f l_vCurrDir4 = m * Vect4f(0,0,1,0);
  //Vect3f l_vCurrDir(l_vCurrDir4.x, l_vCurrDir4.y, l_vCurrDir4.z);
  //Quat4f l_qOldCurrentOrientation = ShortestArc( Vect3f(0,0,1), l_vCurrDir);
  ///////////////////

  Quat4f l_qDesiredAngularSpeed = (l_qCurrentOrientation * l_qDesiredInverted + (Quat4f()*-1)) / (2.f * l_fSpeedTau);
  Vect3f l_vDesiredAngularSpeed(l_qDesiredAngularSpeed.x, l_qDesiredAngularSpeed.y, l_qDesiredAngularSpeed.z);
  
  l_pSphere->GetActor()->SetAngularVelocity(l_vDesiredAngularSpeed);
  

  //Vect3f l_vDesiredAngularMomentum = l_pSphere->GetActor()->GetInertiaTensor() * l_vDesiredAngularSpeed;
  //Vect3f l_vCurrentAngularMomentum = l_pSphere->GetActor()->GetAngularMomentum();
  //
  //
  //Vect3f l_vDesiredTorque = (l_vDesiredAngularMomentum - l_vCurrentAngularMomentum) / l_fTorqueTau;
  //
  //l_pSphere->GetActor()->AddTorque(l_vDesiredTorque);

  */
}

void CComponentIABrainVigia::UpdatePostPhysX(float _fDeltaTime)
{
  CComponentPhysXSphere* l_pSphere = GetEntity()->GetComponent<CComponentPhysXSphere>();
  CComponentObject3D* l_O3D = GetEntity()->GetComponent<CComponentObject3D>();

  if(m_bFly)
  {
    Vect3f l_vPos = l_O3D->GetPosition();
    Vect3f l_vLinearVel = l_pSphere->GetActor()->GetLinearVelocity();

    if(l_vPos != m_vTargetPosition)
    {
      float l_tn = 0.3f;
      float l_eps = 0.25f;
      l_pSphere->GetActor()->AddAcelerationAtLocalPos(Vect3f(0.0f,1.0f,0.0f),v3fZERO,((m_vTargetPosition.y - l_vPos.y) - 2.0f*l_tn*l_eps*l_vLinearVel.y)/(l_tn*l_tn) + 9.8f);
    
      l_eps = 1.0f;
      l_pSphere->GetActor()->AddAcelerationAtLocalPos(Vect3f(1.0f,0.0f,0.0f),v3fZERO,((m_vTargetPosition.x - l_vPos.x) - 2.0f*l_tn*l_eps*l_vLinearVel.x)/(l_tn*l_tn));
      l_pSphere->GetActor()->AddAcelerationAtLocalPos(Vect3f(0.0f,0.0f,1.0f),v3fZERO,((m_vTargetPosition.z - l_vPos.z) - 2.0f*l_tn*l_eps*l_vLinearVel.z)/(l_tn*l_tn));

    }

    //m_vPatrolDirection = m_pPlayer->GetComponent<CComponentObject3D>()->GetPosition() + Vect3f(0.0f,0.5f,0.0f) - l_O3D->GetPosition();
    //m_vPatrolDirection.Normalize();
    LookAt(_fDeltaTime);
  }
}

void CComponentIABrainVigia::Die()
{
  m_bDead = true;

  CGameEntity* l_pLevelController = ENTITY_MANAGER->GetEntity("LevelController");

  if(l_pLevelController)
  {
    SEvent l_morir;
    l_morir.Msg = SEvent::ENEMY_DEAD;
    l_morir.Receiver = l_pLevelController->GetGUID();
    l_morir.Sender = GetEntity()->GetGUID();
      
    ENTITY_MANAGER->SendEvent(l_morir);
  }

  RunScript();
}

void CComponentIABrainVigia::RunScript()
{
  if(m_szOnDeathScript != "") 
  {
    CScriptManager* m_pSM = CORE->GetScriptManager();

    lua_State *l_pLUA = m_pSM->GetLuaState();

    try {
      luabind::call_function<void>(l_pLUA, m_szOnDeathScript.c_str(), GetEntity());
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);

      LOGGER->AddNewLog(ELL_ERROR,"\tEntity \"%s\" has launched script \"%s\" has failed with error \"%s\"", 
                          GetEntity()->GetName().c_str(), m_szOnDeathScript.c_str(), _TheError.what());
    }
  }
}

void CComponentIABrainVigia::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    ReceiveShoot(_Event);
  }else if(_Event.Msg == SEvent::REBRE_FORCE)
  {
    ReceiveForce(_Event);
  }else if(_Event.Msg == SEvent::MORIR)
  {
    if(!m_bDead)
      Die();
  }
}

void CComponentIABrainVigia::Enable()
{
  CGameEntity* l_pLevelController = ENTITY_MANAGER->GetEntity("LevelController");

  if(l_pLevelController)
  {
    SEvent l_alive;
    l_alive.Msg = SEvent::ENEMY_ALIVE;
    l_alive.Receiver = l_pLevelController->GetGUID();
    l_alive.Sender = GetEntity()->GetGUID();
      
    ENTITY_MANAGER->SendEvent(l_alive);
  }
}