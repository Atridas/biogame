#include "EntityProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "RenderableObjectsManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"
#include "ThPSCamera.h"

#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRenderableObject.h"
#include "ComponentVida.h"
#include "ComponentRagdoll.h"
#include "LightManager.h"
#include "SpotLight.h"
#include "Camera.h"
#include "ComponentStateMachine.h"
#include "ComponentAnimation.h"
#include "ComponentIABrain.h"

#include "PhysicActor.h"
#include <PhysicsManager.h>

#include "PortalManager.h"



bool CEntityProcess::Init()
{
  CPortalManager m_PortalManager;
  m_PortalManager.Init("./Data/Levels/NivellProves/XML/Level.xml");


  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();
  CORE->GetLightManager()->SetLightsEnabled(true);

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();

  // Creem la entitat del jugador ----------------------------------------------------------------

  m_pPlayerEntity = CORE->GetEntityManager()->CreateEntity();
  CORE->GetEntityManager()->SetName("Player", m_pPlayerEntity);

  CComponentObject3D *l_pComponentObject3D = new CComponentObject3D();
  l_pComponentObject3D->Init(m_pPlayerEntity);
  l_pComponentObject3D->SetPosition(Vect3f(-8.0f,2.0f,-4.0f));

  (new CComponentMovement)->Init(m_pPlayerEntity);

  CComponentRenderableObject * l_pComponentRenderableObject = new CComponentRenderableObject();
  l_pComponentRenderableObject->InitAnimatedModel(m_pPlayerEntity, "Player Character", "riggle");
  l_pComponentRenderableObject->m_bBlockPitchRoll = true;
  l_pComponentRenderableObject->m_fHeightAdjustment = -1.f;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;


  (new CComponentAnimation())->Init(m_pPlayerEntity);


  CComponentPlayerController *l_pComponentPlayerController = new CComponentPlayerController();
  l_pComponentPlayerController->Init(m_pPlayerEntity,
                                      //Actions
                                     "MoveFwd",
                                     "MoveBack",
                                     "MoveLeft",
                                     "MoveRight",
                                     "Walk",
                                     "Run",
                                     "Aim",
                                     "Shoot",
                                      //Animations
                                     "idle",
                                     "walk",
                                     "walk",
                                     "walk",
                                     "walk",
                                     "point",
                                     "shoot",
                                      //Speed
                                     4, 10, 1, 1,
                                      FLOAT_PI_VALUE/3,
                                     -FLOAT_PI_VALUE/3);

  CComponent3rdPSCamera *l_pComponent3rdPSCamera = new CComponent3rdPSCamera();
  //l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 0, 0);
  //((CThPSCamera*)l_pComponent3rdPSCamera->GetCamera())->SetZoom(0);
  l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 0.55f, 0.85f, 1.8f);

  m_pCamera = l_pComponent3rdPSCamera->GetCamera();

  CComponentPhysXController *l_pComponentPhysXController = new CComponentPhysXController();
  l_pComponentPhysXController->Init(m_pPlayerEntity, 0.3f, 1.5f, 10.0f, 0.1f, 0.01f, ECG_PERSONATGE );

  (new CComponentVida())->Init(m_pPlayerEntity, 100.f);

  (new CComponentStateMachine())->Init(m_pPlayerEntity, "State_Player_Neutre");

  //(new CComponentRagdoll())->Init(m_pPlayerEntity, "Data/Animated Models/Riggle/Skeleton.xml");

  //Carregar entitats de l'escenari ----------------------------------------------------------------------------------------
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/NivellProves/XML/GameEntities.xml");


  // un enemic --------------------------------------------------------------------------------------------------------------
  CGameEntity* l_peEnemy = CORE->GetEntityManager()->CreateEntity();
  CORE->GetEntityManager()->SetName("Gordo Cabrón", l_peEnemy);

  l_pComponentObject3D = new CComponentObject3D();
  l_pComponentObject3D->Init(l_peEnemy);
  l_pComponentObject3D->SetPosition(Vect3f(8.0f,2.0f,4.0f));
  (new CComponentMovement)->Init(l_peEnemy);

  (new CComponentPhysXController())->Init(l_peEnemy, 0.7f, 1.5f, 10.0f, 0.1f, 0.5f,  ECG_ENEMICS );

  l_pComponentRenderableObject = new CComponentRenderableObject();
  l_pComponentRenderableObject->InitAnimatedModel(l_peEnemy, "Bot Character 1", "miner");
  l_pComponentRenderableObject->m_bBlockPitchRoll = true;
  l_pComponentRenderableObject->m_fHeightAdjustment = -1.f;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;

  //(new CComponentIAWalkToPlayer())->Init(l_peEnemy,"Player",2,"walk","impact");
  (new CComponentIABrain())->Init(l_peEnemy,"Player");
  (new CComponentAnimation())->Init(l_peEnemy);
  (new CComponentVida())->Init(l_peEnemy, 100.f);

  (new CComponentStateMachine())->Init(l_peEnemy, "State_Enemy_Idle");

  // llum ----------------------------------------
  
  m_pSpotLight = CORE->GetLightManager()->CreateSpotLight("FreeModeLight",
                                                          Vect3f(-2.15715f,0.0f,-7.32758f),
                                                          Vect3f(-5.4188f,0.0f,3.75613f),
                                                          CColor(Vect3f(1.0f,1.0f,1.0f)),
                                                          20.0f,
                                                          80.0f,
                                                          10.0f,
                                                          45.0f,
                                                          false );

  m_pSpotLight->SetActive(true);

  SetOk(true);
  return IsOk();
}

void CEntityProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Release");

  CHECKED_DELETE(m_pPActorPlane)
  CHECKED_DELETE(m_pUserData)
}

void CEntityProcess::Update(float _fElapsedTime)
{
  CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);

  m_pSpotLight->SetPosition(m_pPlayerPos->GetPosition());
  m_pSpotLight->SetDirection(m_pCamera->GetDirection());
}

void CEntityProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  CORE->GetEntityManager()->DebugRender(_pRM);
}

void CEntityProcess::RenderINFO(CRenderManager* _pRM)
{
  CORE->GetPhysicsManager()->DebugRender(_pRM);

  //CRenderableObject *l_pRO = CORE->GetEntityManager()->GetEntity("Gordo Cabrón")->GetComponent<CComponentRenderableObject>()->GetRenderableObject();
  //
  //CBoundingSphere *l_pBS = l_pRO->GetBoundingSphere();
  //Mat44f t;
  //t = l_pRO->GetMat44();
  //
  //_pRM->SetTransform(t);
  //_pRM->RenderBoundingSphere(l_pBS);
  
  
  /*
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 0;
  int l_iPosicio2 = 130;
  FONT_MANAGER->DrawText(l_iPosicio,l_iPosicio2,colGREEN,l_uiFontType,"Press 'N' to change target");
  */
}

bool CEntityProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  return false;
}

