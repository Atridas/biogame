#define __DONT_INCLUDE_MEM_LEAKS__
#include "ComponentIABrain.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "ComponentObject3D.h"
#include "ComponentLaser.h"
#include "ComponentRagdoll.h" 
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

CComponentIABrain* CComponentIABrain::AddToEntity(CGameEntity *_pEntity, const string& _szPlayerEntityName, const string& _szOnDeathScript, const string& _szDestinyNode)
{
  CComponentIABrain *l_pComp = new CComponentIABrain();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szPlayerEntityName, _szOnDeathScript, _szDestinyNode))
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

bool CComponentIABrain::Init(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szOnDeathScript, const string& _szDestinyNode)
{
  m_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);

  m_szOnDeathScript = _szOnDeathScript;
  m_szDestinyNode = _szDestinyNode;

  m_pCover = 0;
  m_bDead = false;

  SetOk(true);
  return IsOk();
}


void CComponentIABrain::Shoot(float _fShootPrecision)
{
  CComponentRenderableObject* l_pCR = GetEntity()->GetComponent<CComponentRenderableObject>();
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
  l_pEM->InitParticles("disparar", l_vPos, Vect3f(.01f,.01f,.01f), 2.5f, l_vDir);
  l_pEM->InitLaser(l_vPos,l_vDir,SHOOT_POWER, CORE->GetPhysicsManager()->GetCollisionMask(ECG_RAY_SHOOT));

}

void CComponentIABrain::ReceiveShoot(SEvent _sEvent)
{
  CPhysxBone* l_pHeadBone = 0;
  CPhysicActor* l_pActor = 0;
  CComponentRagdoll* l_pRagdoll = 0;

  if(_sEvent.Msg == SEvent::REBRE_IMPACTE)
  {
    l_pRagdoll = GetEntity()->GetComponent<CComponentRagdoll>();

    assert(l_pRagdoll);

    float l_fVida = GetEntity()->GetComponent<CComponentVida>()->GetHP();

    if(l_fVida > 0.0f)
    {

      l_pHeadBone = l_pRagdoll->GetBone("Bip01 Head");

      assert(l_pHeadBone);

      if(_sEvent.Info[2].Type == SEventInfo::PTR)
      {
        l_pActor = (CPhysicActor*)_sEvent.Info[2].ptr;
      }else{
        LOGGER->AddNewLog(ELL_ERROR,"CComponentIABrain::ReciveShoot El missatge a Info[2] no es del tipus PTR");
        return;
      }

      if(l_pHeadBone->GetPhysxActor() == l_pActor)
      {
        if(!m_bDead)
        {
          ActivateRagdoll();
          SEvent l_morir;
          l_morir.Msg = SEvent::MORIR;
          l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
          ENTITY_MANAGER->SendEvent(l_morir);
        }

        Vect3f l_vDir(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

        l_pActor->AddForceAtLocalPos(l_vDir,Vect3f(0.0f),SHOOT_POWER);
      }

    }else{

      l_pActor = (CPhysicActor*)_sEvent.Info[2].i;

      Vect3f l_vDir(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

      l_pActor->AddForceAtLocalPos(l_vDir,Vect3f(0.0f),SHOOT_POWER);
    }

    assert(_sEvent.Info[3].Type == SEventInfo::VECTOR);
    Vect3f l_vPos(_sEvent.Info[3].v.x, _sEvent.Info[3].v.y, _sEvent.Info[3].v.z);
    
    //ENTITY_MANAGER->InitParticles("impacte ragdoll", l_vPos, Vect3f(.5f,.5f,.5f), 5.f);
    ENTITY_MANAGER->InitParticles("impacte ragdoll vapor", l_vPos, Vect3f(.05f,.05f,.05f), 5.f);
    ENTITY_MANAGER->InitParticles("impacte ragdoll sang", l_vPos, Vect3f(.01f,.01f,.01f), 5.f);
  
  }


}

void CComponentIABrain::ReceiveForce(SEvent _sEvent)
{
  Mat44f l_matBonePos;
  CPhysxBone* l_pPhysxBone = 0;
  CComponentRagdoll* l_pRagdoll = 0;
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

    l_pRagdoll = GetEntity()->GetComponent<CComponentRagdoll>();
    l_vSenderPos = Vect3f(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

    //l_vSenderPos = ENTITY_MANAGER->GetEntity(_sEvent.Sender)->GetComponent<CComponentObject3D>()->GetPosition();

    if(l_pRagdoll)
    {
      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Head");
      if(l_pPhysxBone)
      {
        l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
        Vect3f l_vBonePos = l_matBonePos.GetPos();
        Vect3f l_vRayDir = (l_vSenderPos-l_vBonePos).Normalize();
        l_pUserData = l_pPM->RaycastClosestActor(l_vBonePos,l_vRayDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);

        if(!l_pUserData || l_pUserData->GetEntity() != ENTITY_MANAGER->GetEntity(_sEvent.Sender))
        {
          return;
        }
      }

      if(!m_bDead)
      {
        ActivateRagdoll();
        SEvent l_morir;
        l_morir.Msg = SEvent::MORIR;
        l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
        ENTITY_MANAGER->SendEvent(l_morir);
      }

      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Spine");
      assert(l_pPhysxBone);
      l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
      l_vDirection = (l_matBonePos.GetPos() - l_vSenderPos).Normalize();
      l_pPhysxBone->GetPhysxActor()->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),50.0f);

      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Spine1");
      assert(l_pPhysxBone);
      l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
      l_vDirection = (l_matBonePos.GetPos() - l_vSenderPos).Normalize();
      l_pPhysxBone->GetPhysxActor()->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),50.0f);

      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Spine2");
      assert(l_pPhysxBone);
      l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
      l_vDirection = (l_matBonePos.GetPos() - l_vSenderPos).Normalize();
      l_pPhysxBone->GetPhysxActor()->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),50.0f);
    }
    
  }
}

void CComponentIABrain::Update(float _fDeltaTime)
{
  CComponentObject3D* l_pObject3D = GetEntity()->GetComponent<CComponentObject3D>();
}

void CComponentIABrain::ActivateRagdoll()
{
  GetEntity()->DeleteComponent(CBaseComponent::ECT_PHYSX_CONTROLLER);

  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();

  if(l_pRC)
  {
    l_pRC->ApplyPhysics(true);
  }
}

void CComponentIABrain::Die()
{
  m_bDead = true;
  ActivateRagdoll();

  if(m_pCover)
    m_pCover->m_bOcupat = false;

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

void CComponentIABrain::RunScript()
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

bool CComponentIABrain::PlanPathToCobertura()
{
  if(m_pCover)
    return true;

  if(m_szDestinyNode != "")
  {
    CGameEntity* l_pNode = ENTITY_MANAGER->GetEntity(m_szDestinyNode);
    if(l_pNode)
    {
      CComponentObject3D* l_pDestPos   = l_pNode->GetComponent<CComponentObject3D>();
      CComponentObject3D* l_pObject3D  = GetEntity()->GetComponent<CComponentObject3D>();
      CComponentNavNode*  l_pGraphNode = l_pNode->GetComponent<CComponentNavNode>();

      if(l_pObject3D && l_pDestPos && l_pGraphNode)
      {
        if(!l_pGraphNode->m_bOcupat)
        {
          m_PathToCobertura = CORE->GetIAManager()->SearchPathA(l_pObject3D->GetPosition(), l_pDestPos->GetPosition());
        }
        else
        {
          return false;
        }
      }
      else
      {
        LOGGER->AddNewLog(ELL_WARNING, "CComponentIABrain::PlanPathToCobertura error al trobar l'object3D del node \"%s\"", m_szDestinyNode.c_str());
        return false;
      }
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING, "CComponentIABrain::PlanPathToCobertura error al trobar el node \"%s\"", m_szDestinyNode.c_str());
      return false;
    }
  }
  else
  {
    m_PathToCobertura = CORE->GetIAManager()->GetClosestCobertura(GetEntity()->GetComponent<CComponentObject3D>()->GetPosition());
  }
  
  return CheckCoverPath();
}

bool CComponentIABrain::PlanPathToCobertura(int _iFirstNodeMaxDistance)
{
  if(m_pCover)
    return true;

  if(m_szDestinyNode != "")
  {
    //cerquem el path standard
    return PlanPathToCobertura();
  }
  else
  {
    m_PathToCobertura = CORE->GetIAManager()->GetClosestCobertura(GetEntity()->GetComponent<CComponentObject3D>()->GetPosition(), _iFirstNodeMaxDistance);
  }
  
  return CheckCoverPath();
}

bool CComponentIABrain::CheckCoverPath()
{
  if( !m_PathToCobertura.empty() )
  {
    m_pCover = (*(--m_PathToCobertura.end()))->GetEntity()->GetComponent<CComponentNavNode>();
    m_pCover->m_bOcupat = true;

    vector<CGraphNode*>::iterator first = m_PathToCobertura.begin();
    vector<CGraphNode*>::iterator last  = m_PathToCobertura.end();

    while ((first!=last)&&(first!=--last))
      swap (*first++,*last);

    return true;
  }

  return false;
}

Vect3f CComponentIABrain::GetNextNodePosition() const
{
  if(!m_PathToCobertura.empty())
    return m_PathToCobertura[m_PathToCobertura.size() - 1]->GetPosition();
  else
    return GetEntity()->GetComponent<CComponentObject3D>()->GetPosition();
}

bool CComponentIABrain::ArrivedAtDestination() const
{
  return m_PathToCobertura.size() == 1;
}

bool CComponentIABrain::ArrivedAtNode(float _fDistanceSq) const
{
  Vect3f l_vPos = GetEntity()->GetComponent<CComponentObject3D>()->GetPosition();
  Vect3f l_vNodePos = GetNextNodePosition();

  if(l_vPos.SqDistance(l_vNodePos) < _fDistanceSq)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void CComponentIABrain::SetNextNode()
{
  if(!m_PathToCobertura.empty())
    m_PathToCobertura.pop_back();
}

void CComponentIABrain::ReceiveEvent(const SEvent& _Event)
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

void CComponentIABrain::Enable()
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