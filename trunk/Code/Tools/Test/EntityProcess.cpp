#include "EntityProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "RenderableObjectsManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"

#include "ComponentObject3D.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"


#include "PhysicActor.h"
#include <PhysicsManager.h>

bool CEntityProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();



  // Creem la entitat del jugador ----------------------------------------------------------------

  m_pPlayerEntity = CORE->GetEntityManager()->CreateEntity();

  CComponentObject3D *m_pComponentObject3D = new CComponentObject3D();
  m_pComponentObject3D->Init(m_pPlayerEntity);
  m_pComponentObject3D->SetPosition(Vect3f(-8.0f,2.0f,-4.0f));

  CComponentPlayerController *m_pComponentPlayerController = new CComponentPlayerController();
  m_pComponentPlayerController->Init(m_pPlayerEntity,
                                     "MoveFwd",
                                     "MoveBack",
                                     "MoveLeft",
                                     "MoveRight",
                                     "Walk",
                                     "Run",
                                     4, 10, 1, 1,
                                      FLOAT_PI_VALUE/3,
                                     -FLOAT_PI_VALUE/3);

  CComponent3rdPSCamera *m_pComponent3rdPSCamera = new CComponent3rdPSCamera();
  m_pComponent3rdPSCamera->Init(m_pPlayerEntity, 1);

  m_pCamera = m_pComponent3rdPSCamera->GetCamera();

  CComponentPhysXController *m_pComponentPhysXController = new CComponentPhysXController();
  m_pComponentPhysXController->Init(m_pPlayerEntity, 0.3f, 1.5f, 10.0f, 0.1f, 0.5f, 1);


  CGameEntity* l_peFloor = CORE->GetEntityManager()->CreateEntity();
  (new CComponentObject3D())->Init(l_peFloor);
  (new CComponentPhysXBox())->Init(l_peFloor ,
                                    50.f, 1.f, 50.f, 
                                    0.f, -1.f,  0.f,
                                    0, 0
                                  );
  

  /*
  CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
  l_pPhysManager->SetDebugRenderMode(true);

  m_pUserData = new CPhysicUserData("Plane");
  m_pUserData->SetPaint(true);
  m_pUserData->SetColor(colBLUE);
  m_pPActorPlane = new CPhysicActor(m_pUserData);
  m_pPActorPlane->AddBoxSphape(Vect3f(50.f,1.0f,50.f),Vect3f(0.0f,-1.0f,0.0f),NULL,0);
  l_pPhysManager->AddPhysicActor(m_pPActorPlane);
  */

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

