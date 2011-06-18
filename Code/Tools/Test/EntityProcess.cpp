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
#include "ComponentMirilla.h"
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


  //Carregar entitats de l'escenari ----------------------------------------------------------------------------------------
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/NivellProves/XML/GameEntities.xml");

  
  m_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
  m_pCamera = m_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();

  //// Creem la entitat del jugador ----------------------------------------------------------------
  //
  //m_pPlayerEntity = CORE->GetEntityManager()->InitPlayer("Player",Vect3f(-8.0f,2.0f,-4.0f), 0);
  //m_pCamera = m_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();
  //
  //// un enemic --------------------------------------------------------------------------------------------------------------
  //
  //CGameEntity* l_peEnemy = CORE->GetEntityManager()->InitMiner("Player", Vect3f(8.0f,2.0f,4.0f), "Gordo Cabrón");

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

  //CORE->GetEntityManager()->DebugRender(_pRM);
}

void CEntityProcess::RenderINFO(CRenderManager* _pRM)
{
  //CORE->GetPhysicsManager()->DebugRender(_pRM);

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

