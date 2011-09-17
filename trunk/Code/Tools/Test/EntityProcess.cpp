#include "EntityProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "RenderableObjectsManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"
#include "ThPSCamera.h"

#include "SoundManager.h"
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
#include "IAManager.h"
#include "GraphDefines.h"
#include "EmiterManager.h"
#include "StaticMeshManager.h"
#include "LevelChanger.h"

#include "PhysicActor.h"
#include <PhysicsManager.h>

#include "PortalManager.h"

#include "SphereCamera.h"

#include "Renderer.h"
#include "ShadowMapPreRendererStep.h"

bool CEntityProcess::Init()
{
  //m_PortalManager.Init("./Data/Levels/NivellProves/XML/Level.xml");


  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Init");
  
  //m_pSceneEffectManager = CORE->GetSceneEffectManager();
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
                                                          Vect3f(0.0f,15.0f,0.0f),
                                                          Vect3f(0.3f,-1.0f,0.0f),
                                                          CColor(Vect3f(1.0f,1.0f,1.0f)),
                                                          20.0f,
                                                          80.0f,
                                                          10.0f,
                                                          45.0f,
                                                          true );
  m_pSpotLight->SetPosition(Vect3f(0.0f,15.0f,0.0f));
  m_pSpotLight->SetDirection(Vect3f(0.3f,-1.0f,0.0f).Normalize());
  m_pSpotLight->SetActive(true);

  //CShadowMapPreRendererStep* l_pShadowMapPreRenderStep = (CShadowMapPreRendererStep*)CORE->GetRenderer()->GetPreSceneRendererStep("shadow_map_renderer");
  //l_pShadowMapPreRenderStep->SetShadowMapLightCast(m_pSpotLight);

  CORE->GetIAManager()->CompleteGraph();

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
  m_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
  m_pCamera = m_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();


  CObject3D* m_pPlayerPos = m_pPlayerEntity->GetComponent<CComponentObject3D>();

  m_pSpotLight->SetPosition(m_pCamera->GetEye());
  m_pSpotLight->SetDirection(m_pCamera->GetDirection());

  CORE->GetSoundManager()->UpdateSound3DSystem(m_pPlayerPos->GetPosition(),m_pCamera->GetDirection());
}

/*
void CEntityProcess::RenderScene(CRenderManager* _pRM)
{
  if(CORE->GetActionManager()->IsActionActive("ToggleDebug"))
  {
    CObject3D l_Poslluny;
    l_Poslluny.SetPosition(Vect3f(0,25,0));
    CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);
  
  
    CSphereCamera l_SphereCamera( 1, 700, 55.0f * FLOAT_PI_VALUE/180.0f,
                                  ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                                  &l_Poslluny,m_pPlayerPos);
  
  
    _pRM->SetupMatrices(&l_SphereCamera,false,false);
  }
  
  CORE->GetPortalManager()->Render(_pRM);
  //CORE->GetEmiterManager()->Render(_pRM);
}
*/

void CEntityProcess::RenderINFO(CRenderManager* _pRM)
{
  //CORE->GetPhysicsManager()->DebugRender(_pRM);
  //CORE->GetPortalManager()->DebugRender(_pRM);
  //CORE->GetEntityManager()->DebugRender(_pRM);
  //CORE->GetIAManager()->GetGraph()->DebugRender(_pRM);
  //CORE->GetEmiterManager()->DebugRender(_pRM);
  /*if(CORE->GetActionManager()->IsActionActive("ToggleDebug"))
  {
    CORE->GetPhysicsManager()->DebugRender(_pRM);
    CORE->GetPortalManager()->DebugRender(_pRM);
    CORE->GetEntityManager()->DebugRender(_pRM);
    CORE->GetIAManager()->GetGraph()->DebugRender(_pRM);
    CORE->GetEmiterManager()->DebugRender(_pRM);

    CFrustum l_Frustum;
    l_Frustum.Update(m_pCamera);
  
    Mat44f i;
    i.SetIdentity();
    _pRM->SetTransform(i);
  
    _pRM->DrawFrustum(&l_Frustum, colBLACK);
  }*/

}

bool CEntityProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "Hangar") == 0)
  {
    //CORE->SetLoadLevel("Hangar");
    CORE->GetLevelChanger()->SetNewLevel("Hangar");

    //CORE->GetEntityManager()->Done();
    //CORE->GetPortalManager()->Done();
    //CORE->GetRenderableObjectsManager()->Done();
    //CORE->GetStaticMeshManager()->Done();
    //CORE->GetIAManager()->Done();
    //
    //string m_szPhysxFile = CORE->GetPhysicsManager()->GetConfigFileName();
    //CORE->GetPhysicsManager()->Done();
    //
    //CORE->GetStaticMeshManager()->Load("Data/XML/StaticMeshes.xml");
    //CORE->GetStaticMeshManager()->Load("Data/Levels/Hangar/XML/StaticMeshes.xml");
    //CORE->GetRenderableObjectsManager()->Load("Data/Levels/Hangar/XML/RenderableObjects.xml");
    //CORE->GetPortalManager()->Init("Data/Levels/Hangar/XML/Level.xml");
    //CORE->GetPhysicsManager()->Init(m_szPhysxFile);
    //if(CORE->GetPhysicsManager()->IsOk())
    //{
    //  CORE->GetPhysicsManager()->SetTriggerReport  (CORE->GetPhysicTriggerReport());
    //  CORE->GetPhysicsManager()->SetCollisionReport(CORE->GetPhysicCollisionReport());
    //}
    //CORE->GetIAManager()->Init();
    //CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Hangar/XML/GameEntities.xml");
    //
    //CORE->GetIAManager()->CompleteGraph();
    //
    //m_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
    //m_pCamera = m_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();
  }
  return false;
}

