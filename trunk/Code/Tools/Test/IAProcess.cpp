#include "IAProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "RenderableObjectsManager.h"
#include "LightManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"
#include "GraphDefines.h"

bool CIAProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CIAProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  CORE->GetScriptManager()->RunCode("load_level('ia')");

  if(!CORE->GetActionManager()->Load("Data/XML/actions_soundtest.xml"))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSoundTestProcess::Init Error loading actions.");
  }

  m_pTargetObject = new CObject3D();
  
  m_pTargetObject->SetPosition(Vect3f(-8.0f,2.0f,-4.0f));

  m_pObjectCamera = new CFPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pTargetObject);

  m_pCamera = m_pObjectCamera;

  m_pGraph = new CSparseGraph(false);

  GraphHelper_CreateGrid(*m_pGraph, 20, 20, 20, 20);

  SetOk(true);
  return IsOk();
}

void CIAProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CSoundTestProcess::Release");

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pTargetObject)
  CHECKED_DELETE(m_pGraph)
}

void CIAProcess::Update(float _fElapsedTime)
{
  float l_fVelocity = 3.0f;
  Vect3f l_vPosDelta = Vect3f(0.0f);
  float l_fDeltaYaw = m_vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = m_vMouseDelta.y * _fElapsedTime;

  if(CORE->GetActionManager()->IsActionActive("Run"))
  {
    l_fVelocity = 5.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveFwd"))
  {
    l_vPosDelta.x += 1.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveBack"))
  {
    l_vPosDelta.x -= 1.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveLeft"))
  {
    l_vPosDelta.z += 1.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveRight"))
  {
    l_vPosDelta.z -= 1.0f;
  }

  if(l_vPosDelta != Vect3f(0.0f))
    l_vPosDelta.Normalize();

  l_vPosDelta *= l_fVelocity * _fElapsedTime;

  UpdatePosition(l_vPosDelta,l_fDeltaPitch,l_fDeltaYaw);
  UpdateCamera(l_fDeltaPitch, l_fDeltaYaw);

  Vect3f l_vFrontDirection(0.0f);

  l_vFrontDirection.x = 1.0f;
  l_vFrontDirection.RotateY(-m_pTargetObject->GetYaw());

  m_vMouseDelta = 0;
}

void CIAProcess::UpdateCamera(float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fTargetPitch = m_pTargetObject->GetPitch() - _fDeltaPitch;
  float l_fTargetYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;

  if(l_fTargetPitch < -FLOAT_PI_VALUE/3) l_fTargetPitch = -FLOAT_PI_VALUE/3;
  if(l_fTargetPitch > FLOAT_PI_VALUE/3) l_fTargetPitch = FLOAT_PI_VALUE/3;
 
  m_pTargetObject->SetYaw(l_fTargetYaw);
  m_pTargetObject->SetPitch(l_fTargetPitch);
}

void CIAProcess::UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fYaw = 0.0f;

  l_fYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;
  _PosDelta.RotateY(-l_fYaw);
  m_pTargetObject->SetPosition(m_pTargetObject->GetPosition()+_PosDelta);

}

void CIAProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);
}

void CIAProcess::RenderINFO(CRenderManager* _pRM)
{
  _pRM->DrawGrid(20,colWHITE,20,20);
}

bool CIAProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "Yaw") == 0)
  {
    m_vMouseDelta.x = (int)_fDelta;
    return true;
  }

  if(strcmp(_pcAction, "Pitch") == 0)
  {
    m_vMouseDelta.y = (int)_fDelta;
    return true;
  }

  return false;
}

