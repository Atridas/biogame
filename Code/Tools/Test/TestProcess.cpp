#include "TestProcess.h"
#include "Core.h"
#include "ActionManager.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "EntityManager.h"
#include "ShoulderCamera.h"
#include "InputManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "StaticMesh.h"
#include "StaticMeshManager.h"
#include "RenderableObjectsManager.h"
#include "AnimatedModelManager.h"
#include "AnimatedInstanceModel.h"
#include "RenderableAnimatedInstanceModel.h"
#include <IndexedVertexs.h>
#include "VertexsStructs.h"
#include "Console.h"
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRenderableObject.h"
#include "ComponentAnimation.h"
#include "ComponentVida.h"
#include "ComponentStateMachine.h"
#include "ComponentAnimation.h"
#include <LightManager.h>
#include "SpotLight.h"

#define ALTURA_CONTROLLER 1.5f
#define RADIUS_CONTROLLER 0.3f
#define COLISIONABLE_MASK ((1<<GROUP_COLLIDABLE_NON_PUSHABLE))



bool CTestProcess::Init()
{
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

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

  l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 0.55f, 0.85f, 1.8f);

  m_pCamera = l_pComponent3rdPSCamera->GetCamera();

  CComponentPhysXController *l_pComponentPhysXController = new CComponentPhysXController();
  l_pComponentPhysXController->Init(m_pPlayerEntity, 0.3f, 1.5f, 10.0f, 0.1f, 0.5f, ECG_PERSONATGE );

  (new CComponentStateMachine())->Init(m_pPlayerEntity, "State_Player_Neutre");


  CLightManager* l_pLM = CORE->GetLightManager();
  l_pLM->SetLightsEnabled(true);

  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/NivellProves/XML/GameEntities.xml");


  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{

}

void CTestProcess::Update(float _fElapsedTime)
{

}

void CTestProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  //Mat44f m;
  //_pRM->SetTransform(m.SetIdentity());
  //_pRM->DrawAxis();
  //_pRM->DrawLine(m_pCamera->GetLookAt(),m_pCamera->GetEye()-Vect3f(0.0f,0.5f,0.0f)+m_pCamera->GetDirection().Normalize()*5.0f,colRED);
  //_pRM->DrawLine(m_pCamera->GetEye(),Vect3f(0.0f),colBLUE);
}

void CTestProcess::RenderINFO(CRenderManager* _pRM)
{
 
}

bool CTestProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  
  return false;
}
