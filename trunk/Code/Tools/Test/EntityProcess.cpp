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
#include "IAManager.h"
#include "GraphDefines.h"
#include "ParticleManager.h"
#include "BillboardManager.h"

#include "PhysicActor.h"
#include <PhysicsManager.h>

#include "PortalManager.h"

#include "SphereCamera.h"

bool CEntityProcess::Init()
{
  //m_PortalManager.Init("./Data/Levels/NivellProves/XML/Level.xml");


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
  CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);

  m_pSpotLight->SetPosition(m_pPlayerPos->GetPosition());
  m_pSpotLight->SetDirection(m_pCamera->GetDirection());

  CORE->GetParticleManager()->Update(_fElapsedTime, m_pCamera);
  CORE->GetBillBoardManager()->Update(_fElapsedTime, m_pCamera);
}

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
  
  //CORE->GetRenderableObjectsManager()->Render(_pRM);
  CORE->GetPortalManager()->Render(_pRM);
  CORE->GetParticleManager()->Render(_pRM);
  CORE->GetBillBoardManager()->Render(_pRM);
  //m_PortalManager.Render(_pRM);

  //CORE->GetEntityManager()->DebugRender(_pRM);
}

void CEntityProcess::RenderINFO(CRenderManager* _pRM)
{
 
  if(CORE->GetActionManager()->IsActionActive("ToggleDebug"))
  {
    CORE->GetPhysicsManager()->DebugRender(_pRM);
    CORE->GetPortalManager()->DebugRender(_pRM);
    CORE->GetEntityManager()->DebugRender(_pRM);

    CFrustum l_Frustum;
    l_Frustum.Update(m_pCamera);
  
    Mat44f i;
    i.SetIdentity();
    _pRM->SetTransform(i);
  
    _pRM->DrawFrustum(&l_Frustum, colBLACK);

  }
  CORE->GetIAManager()->GetGraph()->DebugRender(_pRM);
}

bool CEntityProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  return false;
}

