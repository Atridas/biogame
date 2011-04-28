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

bool CEntityProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  m_pPlayerEntity = new CGameEntity();
  m_pPlayerEntity->Init();

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
                                      -FLOAT_PI_VALUE/3,
                                      1);

  m_pCamera = m_pComponentPlayerController->GetCamera();

  SetOk(true);
  return IsOk();
}

void CEntityProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Release");

  CHECKED_DELETE(m_pPlayerEntity);
}

void CEntityProcess::Update(float _fElapsedTime)
{
  m_pPlayerEntity->Update(_fElapsedTime);
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

