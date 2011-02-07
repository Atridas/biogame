#include "ViewerProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
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

#include <LightManager.h>
#include "SpotLight.h"


bool CViewerProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Init");
  
 
  m_pObject = new CObject3D();
  m_fVelocity = 5;
  
  m_pObject->SetPosition(Vect3f(-6,1.7f,0));

  m_pObjectBot = CORE->GetRenderableObjectsManager()->GetResource("bot");
  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");

  m_iState = 0;
  m_iMode = 0;

  m_bStateChanged = false;

  //m_pObjectCamera = new CFPSCamera(
  //  0.1f,
  //  100.0f,
  //  45.0f * FLOAT_PI_VALUE/180.0f,
  //  ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
  //  m_pObject);

  m_pObjectCamera = new CThPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pObject,
    2.5f);

  m_pCamera = m_pObjectCamera;
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  l_Spot->SetDirection(m_pObjectBot->GetPosition());

  if (m_iMode != 0)
  {
    m_pObject->SetPosition(Vect3f(0.0f,0.0f,0.0f));
  }

  m_bRenderLights = true;

  SetOk(true);
  return IsOk();
}

void CViewerProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Release");

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pObject)
}

void CViewerProcess::Update(float _fElapsedTime)
{
  //Actualitze el pitch i el yaw segons els delta del mouse
  float l_fPitch, l_fYaw;

  Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();
  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");

  l_fPitch = m_pObject->GetPitch();
  l_fYaw = m_pObject->GetYaw();
  
  m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
  
  l_fPitch -= l_vVec.y*_fElapsedTime;
  if(l_fPitch < - FLOAT_PI_VALUE/3) l_fPitch = - FLOAT_PI_VALUE/3;
  if(l_fPitch >   FLOAT_PI_VALUE/3) l_fPitch =   FLOAT_PI_VALUE/3;
  m_pObject->SetPitch(l_fPitch);

  if (m_iMode == 0)
  {
    m_pObjectBot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObjectBot->GetPosition().y, m_pObject->GetPosition().z));
    m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);
    l_fPitch = m_pObjectBot->GetPitch();
    l_fYaw = m_pObjectBot->GetYaw();
    l_fYaw = l_fYaw+FLOAT_PI_VALUE/2;
    if (l_Spot != 0)
    l_Spot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObjectBot->GetPosition().y, m_pObject->GetPosition().z));

    Vect3f l_vec(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch),sin(l_fYaw) * cos(l_fPitch) );
    l_Spot->SetDirection(Vect3f(l_vec.x,l_vec.y,l_vec.z));
  }
  else
  {
    
    
  }
  
  

  

  

  if(m_bStateChanged)
  {
    ((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->BlendCycle(m_iState,0);

    m_bStateChanged = false;
  }

}

void CViewerProcess::RenderScene(CRenderManager* _pRM)
{
  //Render Objects
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  //Render Lights
  if(m_bRenderLights)
    CORE->GetLightManager()->Render(_pRM);

  //Matrix for testing
  Mat44f r,r2, t, s, identity, total;

  identity.SetIdentity();
  r.SetIdentity();
  r2.SetIdentity();
  t.SetIdentity();
  s.SetIdentity();

  //Draw Grid and Axis
  _pRM->SetTransform(identity);
  _pRM->DrawGrid(30.0f,colCYAN,30,30);
  _pRM->DrawAxis();


  //text
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  string l_szMsg("Mode Animats");
  string l_szMsg2("Mode Meshes");
  string l_szMsg3("Mode Escena");

  switch (m_iMode)
  {
    case MODE_ESCENA:
      FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontType,l_szMsg3.c_str());
      break;

    case MODE_MESH:
      FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontType,l_szMsg2.c_str());
      break;

    case MODE_ANIMATS:
      FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontType,l_szMsg.c_str());
      break; 
  }
}

bool CViewerProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "Run") == 0)
  {
    if (m_iMode == 0)
    {
      m_fVelocity = 10;
    }
    
    return true;
  }

  if(strcmp(_pcAction, "Walk") == 0)
  {
    if (m_iMode == 0)
    {
      m_fVelocity = 5;
    }
    return true;
  }

  if(strcmp(_pcAction, "MoveFwd") == 0)
  {

    Vect3f l_vPos = m_pObject->GetPosition();

    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }
    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

 if(strcmp(_pcAction, "MoveBack") == 0)
  {

    Vect3f l_vPos = m_pObject->GetPosition();
    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x - cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z - sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }
    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveLeft") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();

    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveRight") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();

    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }


  if(strcmp(_pcAction, "ChangeMode") == 0)
  {
    m_iMode = m_iMode + 1;
    if (m_iMode == 3)
    {
      m_iMode = 0;
      CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
      l_pCam->SetZoom(2.5f);
    }
    else 
      m_pObject->SetPosition(Vect3f(0.0f,0.0f,0.0f));

    
    return true;
  }


  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    if (m_iMode != 0)
    {
      CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
      //l_pCam->AddZoom(-0.1f);
      Vect3i l_vDelta = INPUT_MANAGER->GetMouseDelta();

      if (l_vDelta.z < 0)
      {
        l_pCam->AddZoom(0.2f);
      }
      else
      {
        l_pCam->AddZoom(-0.2f);
      }

    }
    return true;
  }

  return false;
}

