#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "InputManager.h"

bool CTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");
  // ---

  m_vPos = Vect2f(-150,0);
  m_vCubePos = Vect3f(0,0,0);
  m_vCubeRot = Vect3f(0,0,0);

  m_pObject = new CObject3D();
  m_pObject->SetPosition(Vect3f(-3,0,0));
  m_pCamera = new CFPSCamera(
    0.1f,
    100.0f,
    45.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pObject);



  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Release");
  CHECKED_DELETE(m_pCamera);
  CHECKED_DELETE(m_pObject);
	// ----
}

void CTestProcess::Update(float _fElapsedTime)
{
  float l_fVelX = 400;
  float l_fCubeVelRotY = 0.5;
  float l_fCubeVelRotZ = 1;

  m_vPos.x += l_fVelX*_fElapsedTime;

  //Codi que fa rotar el CUB!!!!!
  /*m_vCubeRot.y += l_fCubeVelRotY*_fElapsedTime;
  m_vCubeRot.z += l_fCubeVelRotZ*_fElapsedTime;*/

  if(m_vPos.x > RENDER_MANAGER->GetScreenWidth())
    m_vPos.x = -150;

  Vect3i vec = INPUT_MANAGER->GetMouseDelta();
  

  //Actualitze el pitch i el yaw segons els delta del mouse
  float pitch, yaw;

  pitch = m_pObject->GetPitch();
  yaw = m_pObject->GetYaw();
  
  m_pObject->SetYaw(yaw-vec.x*_fElapsedTime);
  m_pObject->SetPitch(pitch-vec.y*_fElapsedTime);


  //Movem el objecte per l'escenari segons les tecles WSAD.
  Vect3f pos = m_pObject->GetPosition();

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_W))
  {
    pos.x = pos.x + cos(m_pObject->GetYaw())*_fElapsedTime;
    pos.z = pos.z + sin(m_pObject->GetYaw())*_fElapsedTime;
    m_pObject->SetPosition(pos);
  }
  
  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_S))
  {
    pos.x = pos.x - cos(m_pObject->GetYaw())*_fElapsedTime;
    pos.z = pos.z - sin(m_pObject->GetYaw())*_fElapsedTime;
    m_pObject->SetPosition(pos);
  }

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_A))
  {
    pos.x = pos.x + cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fElapsedTime;
    pos.z = pos.z + sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fElapsedTime;
    m_pObject->SetPosition(pos);
  }

  if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_D))
  {
    pos.x = pos.x + cos(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fElapsedTime;
    pos.z = pos.z + sin(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fElapsedTime;
    m_pObject->SetPosition(pos);
  }
  

}

void CTestProcess::Render()
{
  CColor col = colBLUE;
  CColor l_CubeCol = colWHITE;

  CRenderManager* pRM = RENDER_MANAGER;

  Mat44f r, t, s, identity, total;

  identity.SetIdentity();
  r.SetIdentity();
  t.SetIdentity();
  s.SetIdentity();

  t.Translate(Vect3f(m_vCubePos.x,m_vCubePos.y,m_vCubePos.z));
  r.RotByAnglesYXZ(m_vCubeRot.x,m_vCubeRot.y,m_vCubeRot.z);
  s.Scale(1.5f,1.5f,1.5f);

  total = t*r*s;

  pRM->SetTransform(identity);

  pRM->DrawAxis();

  pRM->SetTransform(total);

  pRM->DrawCube(Vect3f(0.0f,0.0f,0.0f),1.0f,l_CubeCol);
  
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  string l_szMsg("Biogame");

  FONT_MANAGER->DrawText((uint32)m_vPos.x,(uint32)m_vPos.y,col,l_uiFontType,l_szMsg.c_str());
  
}



