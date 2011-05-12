#include "EntityProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "RenderableObjectsManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"

#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRenderableObject.h"
#include "LightManager.h"

#include "PhysicActor.h"
#include <PhysicsManager.h>

bool CEntityProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();
  CORE->GetLightManager()->SetLightsEnabled(true);


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

  CComponentPlayerController *l_pComponentPlayerController = new CComponentPlayerController();
  l_pComponentPlayerController->Init(m_pPlayerEntity,
                                      //Actions
                                     "MoveFwd",
                                     "MoveBack",
                                     "MoveLeft",
                                     "MoveRight",
                                     "Walk",
                                     "Run",
                                      //Animations
                                     "idle",
                                     "walk",
                                     "walk",
                                     "walk",
                                     "walk",
                                      //Speed
                                     4, 10, 1, 1,
                                      FLOAT_PI_VALUE/3,
                                     -FLOAT_PI_VALUE/3);

  CComponent3rdPSCamera *l_pComponent3rdPSCamera = new CComponent3rdPSCamera();
  l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 1, 0.5f);

  m_pCamera = l_pComponent3rdPSCamera->GetCamera();

  CComponentPhysXController *l_pComponentPhysXController = new CComponentPhysXController();
  l_pComponentPhysXController->Init(m_pPlayerEntity, 0.3f, 1.5f, 10.0f, 0.1f, 0.5f, 1);



  //Carregar entitats de l'escenari
  LoadEntitiesFromXML("Data/Levels/NivellProves/XML/GameEntities.xml");

  /*
  // Entitat de l'escenari ----------------------------------------------------------------------
  CGameEntity* l_peEscenari = CORE->GetEntityManager()->CreateEntity();
  (new CComponentObject3D())->Init(l_peEscenari);
  (new CComponentRenderableObject())->Init(l_peEscenari, "Escenari");
  (new CComponentPhysXMesh())->Init(l_peEscenari, 0, 0);

  //Una paret -------------------------------------------------------------------------
  CGameEntity* l_peWall = CORE->GetEntityManager()->CreateEntity();
  (new CComponentObject3D())->Init(l_peWall);

  CComponentRenderableObject* l_pComponentRenderableObject = new CComponentRenderableObject();
  l_pComponentRenderableObject->Init(l_peWall, "Wall");

  (new CComponentPhysXBox())->Init(l_peWall ,
                                    1, 0
                                  );

  */

  // un enemic ------------------------------------------------------------------------
  CGameEntity* l_peEnemy = CORE->GetEntityManager()->CreateEntity();
  l_pComponentObject3D = new CComponentObject3D();
  l_pComponentObject3D->Init(l_peEnemy);
  l_pComponentObject3D->SetPosition(Vect3f(8.0f,2.0f,4.0f));
  (new CComponentMovement)->Init(l_peEnemy);

  (new CComponentPhysXController())->Init(l_peEnemy, 0.7f, 1.5f, 10.0f, 0.1f, 0.5f, 1);

  l_pComponentRenderableObject = new CComponentRenderableObject();
  l_pComponentRenderableObject->InitAnimatedModel(l_peEnemy, "Bot Character 1", "miner");
  l_pComponentRenderableObject->m_bBlockPitchRoll = true;
  l_pComponentRenderableObject->m_fHeightAdjustment = -1.f;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;

  (new CComponentIAWalkToPlayer())->Init(l_peEnemy,"Player",2,"walk");

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

}

void CEntityProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  
  Mat44f mat;
  mat.SetIdentity();
  _pRM->SetTransform(mat);
  _pRM->DrawGrid(20,colWHITE,20,20);
}

void CEntityProcess::RenderINFO(CRenderManager* _pRM)
{
  CORE->GetPhysicsManager()->DebugRender(_pRM);

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

