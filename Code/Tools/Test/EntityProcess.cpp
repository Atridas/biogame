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
#include "OmniLight.h"
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


#include "SphereCamera.h"

//#define FES_UN_COLLO_DE_LLUMS

bool CEntityProcess::Init()
{
  //Mr.Merda Portàtil.
  //CORE->GetRenderer()->SetUniqueRenderPath("forward");
  //m_PortalManager.Init("./Data/Levels/NivellProves/XML/Level.xml");


  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Init");
  
  //m_pSceneEffectManager = CORE->GetSceneEffectManager();
  CORE->GetLightManager()->SetLightsEnabled(true);

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();

  //Carregar entitats de l'escenari ----------------------------------------------------------------------------------------
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/NivellProves/XML/GameEntities.xml");
  CORE->GetEntityManager()->CreateLevelControllerEntity(false);
  
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
  
#ifdef FES_UN_COLLO_DE_LLUMS

  for(int i = -10; i <= 10; i++ )
  {
    for(int j = -10; j <= 10; j++ )
    {
      COmniLight* l_mOL = CORE->GetLightManager()->CreateOmniLight(string("omni").append(i+10,'i').append(j+10,'j'),
                                                                      Vect3f(i*4.0f,1.5f,j*4.0f),
                                                                      CColor(Vect3f(rand()/(float)RAND_MAX,rand()/(float)RAND_MAX,rand()/(float)RAND_MAX)),
                                                                      2.0f,
                                                                      3.0f);
      l_mOL->SetActive(true);
    }
  }

#endif

  //CSpotLight* l_pSpotLight = CORE->GetLightManager()->CreateSpotLight("FreeModeLight",
  //                                                                    Vect3f(0.0f,15.0f,0.0f),
  //                                                                    Vect3f(0.3f,-1.0f,0.0f),
  //                                                                    CColor(Vect3f(1.0f,1.0f,1.0f)),
  //                                                                    20.0f,
  //                                                                    80.0f,
  //                                                                    10.0f,
  //                                                                    45.0f,
  //                                                                    true );
  //l_pSpotLight->SetPosition(Vect3f(0.0f,15.0f,0.0f));
  //l_pSpotLight->SetDirection(Vect3f(0.3f,-1.0f,0.0f).Normalize());
  //l_pSpotLight->SetActive(true);
  
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //CGameEntity* l_pBichu = CORE->GetEntityManager()->CreateEntity();
  //CComponentObject3D::AddToEntity(l_pBichu);
  //CComponentRenderableObject *l_pCRO = CComponentRenderableObject::AddToEntityWithAnimatedModel(l_pBichu, "Capsula", "Capsula");
  //CComponentAnimation *l_pCA = l_pBichu->GetComponent<CComponentAnimation>();
  //
  //l_pCA->PlayCycle("idle",1,1);

  //l_pCRO->m_bBlockPitchRoll = true;
  //l_pCRO->m_fPitchAdjustment = 90.f;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




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

  //m_pSpotLight->SetPosition(m_pCamera->GetEye());
  //m_pSpotLight->SetDirection(m_pCamera->GetDirection());

  //CORE->GetSoundManager()->UpdateSound3DSystem(m_pPlayerPos->GetPosition(),m_pCamera->GetDirection());

  CSpotLight* l_pSpotLight = (CSpotLight*)CORE->GetLightManager()->GetResource("FreeModeLight");
  
  if(!l_pSpotLight)
  {
    l_pSpotLight = CORE->GetLightManager()->CreateSpotLight("FreeModeLight",
                                                                        Vect3f(0.0f,15.0f,0.0f),
                                                                        Vect3f(0.3f,-1.0f,0.0f),
                                                                        CColor(Vect3f(1.0f,1.0f,1.0f)),
                                                                        20.0f,
                                                                        80.0f,
                                                                        10.0f,
                                                                        45.0f,
                                                                        true );
    l_pSpotLight->SetActive(true);
  }
  l_pSpotLight->SetPosition(m_pCamera->GetEye());
  l_pSpotLight->SetDirection(m_pCamera->GetDirection());
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
  //CORE->GetRenderableObjectsManager()->DebugRender(_pRM);
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
  if(strcmp(_pcAction, "Howto") == 0)
  {
    if(CORE->GetRenderer()->GetActiveCamera())
    {
      CORE->GetScriptManager()->RunCode("deactivate_cynematic_camera()");
    }
    else
    {
      CORE->GetScriptManager()->RunCode("activate_cynematic_camera('CameraDeProves')");
    }
    return true;
  }
  return false;
}

