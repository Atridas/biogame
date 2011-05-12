#include "SoundTestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "RenderableObjectsManager.h"
#include "LightManager.h"
#include "SoundManager.h"
#include "ActionManager.h"

vector<CRenderableObject*>::iterator g_itCurrentMesh;
vector<CRenderableObject*> g_vMeshes;

Vect3i g_vMouseDelta;

bool CSoundTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CSoundTestProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  g_vMeshes = CORE->GetRenderableObjectsManager()->GetMeshes();

  if(g_vMeshes.size() == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSoundTestProcess::Init FATAL ERROR: no renderable meshes.");
    SetOk(false);
    return IsOk();
  }

  if(!CORE->GetActionManager()->Load("Data/XML/actions_soundtest.xml"))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSoundTestProcess::Init Error loading actions.");
  }

  m_pTargetObject = new CObject3D();
  
  m_pTargetObject->SetPosition(Vect3f(-8.0f,2.0f,-4.0f));

  g_itCurrentMesh = g_vMeshes.begin();
  (*g_itCurrentMesh)->SetRenderBoundingBox(true);
  
  m_pObjectCamera = new CFPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pTargetObject);

  m_pCamera = m_pObjectCamera;

  //SOUND_MANAGER->PlayMusic("bgm",true);
  SOUND_MANAGER->SetMusic3DPosition("music3d",(*g_itCurrentMesh)->GetPosition());
  SOUND_MANAGER->PlayMusic("music3d");
  SOUND_MANAGER->PlayMusic("bgm");

  SetOk(true);
  return IsOk();
}

void CSoundTestProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CSoundTestProcess::Release");

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pTargetObject)
}

void CSoundTestProcess::Update(float _fElapsedTime)
{
  float l_fVelocity = 3.0f;
  Vect3f l_vPosDelta = Vect3f(0.0f);
  float l_fDeltaYaw = g_vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = g_vMouseDelta.y * _fElapsedTime;

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

  float l_fDistance = m_pTargetObject->GetPosition().Distance((*g_itCurrentMesh)->GetPosition());

  if(l_fDistance < 5.0f)
  {
    SOUND_MANAGER->FadeMusicVolume("bgm",(l_fDistance)/20.0f,500);
  }else{
    SOUND_MANAGER->FadeMusicVolume("bgm",0.5f,1000);
  }

  l_vFrontDirection.x = 1.0f;
  l_vFrontDirection.RotateY(-m_pTargetObject->GetYaw());

  SOUND_MANAGER->UpdateSound3DSystem(m_pTargetObject->GetPosition(),l_vFrontDirection);

  g_vMouseDelta = 0;
}

void CSoundTestProcess::UpdateCamera(float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fTargetPitch = m_pTargetObject->GetPitch() - _fDeltaPitch;
  float l_fTargetYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;

  if(l_fTargetPitch < -FLOAT_PI_VALUE/3) l_fTargetPitch = -FLOAT_PI_VALUE/3;
  if(l_fTargetPitch > FLOAT_PI_VALUE/3) l_fTargetPitch = FLOAT_PI_VALUE/3;
 
  m_pTargetObject->SetYaw(l_fTargetYaw);
  m_pTargetObject->SetPitch(l_fTargetPitch);
}

void CSoundTestProcess::UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fYaw = 0.0f;

  l_fYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;
  _PosDelta.RotateY(-l_fYaw);
  m_pTargetObject->SetPosition(m_pTargetObject->GetPosition()+_PosDelta);

}

void CSoundTestProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);
  CORE->GetLightManager()->Render(_pRM);
}

void CSoundTestProcess::RenderINFO(CRenderManager* _pRM)
{

}

bool CSoundTestProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "Yaw") == 0)
  {
    g_vMouseDelta.x = (int)_fDelta;
    return true;
  }

  if(strcmp(_pcAction, "Pitch") == 0)
  {
    g_vMouseDelta.y = (int)_fDelta;
    return true;
  }

  if(strcmp(_pcAction, "next") == 0)
  {
    (*g_itCurrentMesh)->SetRenderBoundingBox(false);

    ++g_itCurrentMesh;
    
    if(g_itCurrentMesh == g_vMeshes.end())
    {
      g_itCurrentMesh = g_vMeshes.begin();
    }

    (*g_itCurrentMesh)->SetRenderBoundingBox(true);

    SOUND_MANAGER->SetMusic3DPosition("music3d",(*g_itCurrentMesh)->GetPosition());

    return true;
  }

  if(strcmp(_pcAction, "previous") == 0)
  {
    (*g_itCurrentMesh)->SetRenderBoundingBox(false);

    if(g_itCurrentMesh == g_vMeshes.begin())
    {
      g_itCurrentMesh = g_vMeshes.end();
    }

    --g_itCurrentMesh;

    (*g_itCurrentMesh)->SetRenderBoundingBox(true);

    SOUND_MANAGER->SetMusic3DPosition("music3d",(*g_itCurrentMesh)->GetPosition());

    return true;
  }
  

  return false;
}

