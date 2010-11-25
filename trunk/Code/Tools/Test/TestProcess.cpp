#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
#include "InputManager.h"

bool CTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");
  // ---

  m_vPos = Vect2f(-150,0);

  //m_vCubePos = Vect3f(0,0,0);
  //m_vCubeRot = Vect3f(0,0,0);
  
  m_pCube = new CObject3D();
  m_pCube->SetPosition(Vect3f(0,0.5,0));

  //Init CubeCamera with the same m_pCamera parameters
  m_pCubeCamera = new CThPSCamera(
                            0.1f,
                            100.0f,
                            45.0f * FLOAT_PI_VALUE/180.0f,
                            ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                            m_pCube,
                            6.0f);

    //m_pCubeCamera = new CFPSCamera(
    //                        0.1f,
    //                        100.0f,
    //                        45.0f * FLOAT_PI_VALUE/180.0f,
    //                        ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    //                        m_pCube);

  m_pObject = new CObject3D();
  m_pObject->SetPosition(Vect3f(-6,2,0));
  m_pObjectCamera = new CFPSCamera(
    0.1f,
    100.0f,
    45.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pObject);

  //m_pObjectCamera = new CThPSCamera(
  //  0.1f,
  //  100.0f,
  //  45.0f * FLOAT_PI_VALUE/180.0f,
  //  ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
  //  m_pObject,6.0f);

  m_pCamera = m_pObjectCamera;

  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Release");
  CHECKED_DELETE(m_pObjectCamera);
  CHECKED_DELETE(m_pObject);
  CHECKED_DELETE(m_pCube);
  CHECKED_DELETE(m_pCubeCamera);
	// ----
}

void CTestProcess::Update(float _fElapsedTime)
{
  float l_fVelX = 400;
  float l_fCubeVelRotY = 0.5;
  float l_fCubeVelRotZ = 1;

  m_vPos.x += l_fVelX*_fElapsedTime;

  //Codi que fa rotar el CUB!!!!!
  float l_fCubeRotY = m_pCube->GetYaw();
  float l_fCubeRotZ = m_pCube->GetPitch();
  m_pCube->SetPitch(l_fCubeRotZ + l_fCubeVelRotZ*_fElapsedTime);
  m_pCube->SetYaw(l_fCubeRotY + l_fCubeVelRotY*_fElapsedTime);


  if(m_vPos.x > RENDER_MANAGER->GetScreenWidth())
    m_vPos.x = -150;

  
  

  //Actualitze el pitch i el yaw segons els delta del mouse
  float l_fPitch, l_fYaw, l_fVelocity;

  l_fVelocity = 1;
  Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

  l_fPitch = m_pObject->GetPitch();
  l_fYaw = m_pObject->GetYaw();
  
  m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
  m_pObject->SetPitch(l_fPitch-l_vVec.y*_fElapsedTime);


  //Movem el objecte per l'escenari segons les tecles WSAD.
  Vect3f l_vPos = m_pObject->GetPosition();

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_LSHIFT))
  {
    l_fVelocity = 10;
  }

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_W))
  {
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw())*_fElapsedTime*l_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw())*_fElapsedTime*l_fVelocity;
    m_pObject->SetPosition(l_vPos);
  }
  
  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_S))
  {
    l_vPos.x = l_vPos.x - cos(m_pObject->GetYaw())*_fElapsedTime*l_fVelocity;
    l_vPos.z = l_vPos.z - sin(m_pObject->GetYaw())*_fElapsedTime*l_fVelocity;
    m_pObject->SetPosition(l_vPos);
  }

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_A))
  {
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fElapsedTime*l_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fElapsedTime*l_fVelocity;
    m_pObject->SetPosition(l_vPos);
  }

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_D))
  {
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fElapsedTime*l_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fElapsedTime*l_fVelocity;
    m_pObject->SetPosition(l_vPos);
  }

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_Z))
  {
    m_pCamera = m_pCubeCamera;
  }else{
    m_pCamera = m_pObjectCamera;
  }

}

void CTestProcess::Render()
{
  CColor col = colBLUE;
  CColor l_CubeCol = colWHITE;

  CRenderManager* pRM = RENDER_MANAGER;

  Mat44f r,r2, t, s, identity, total;

  identity.SetIdentity();
  r.SetIdentity();
  r2.SetIdentity();
  t.SetIdentity();
  s.SetIdentity();

  pRM->SetTransform(identity);

  t.Translate(m_pCube->GetPosition());
  r.SetFromAngleY(-m_pCube->GetYaw());
  r2.SetFromAngleZ(m_pCube->GetPitch());
  s.Scale(1.5f,1.5f,1.5f);

  total = t*r*r2*s;

  pRM->DrawAxis();

  pRM->DrawGrid(30.0f,colCYAN,30,30);

  pRM->DrawCube(Vect3f(2.0f,0.5f,0.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,0.5f,2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,0.5f,-2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,2.5f,0.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,2.5f,2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,2.5f,-2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,4.5f,0.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,4.5f,2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,4.5f,-2.0f),1.0f,l_CubeCol);

  pRM->SetTransform(total);

  pRM->DrawCube(1.0f,l_CubeCol);
 
  pRM->DrawCamera(m_pCubeCamera);
  
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  string l_szMsg("Biogame");

  FONT_MANAGER->DrawText((uint32)m_vPos.x,(uint32)m_vPos.y,col,l_uiFontType,l_szMsg.c_str());
  
}



