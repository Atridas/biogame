#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "StaticMesh.h"

#include <IndexedVertexs.h>
#include "VertexsStructs.h"

STEXTUREDVERTEX g_vertex[4] = {
	{-1.5f,3.0f,0.0f,0.0f,0.0f},
	{1.5f,3.0f,0.0f,1.0f,0.0f},
	{-1.5f,0.0f,0.0f,0.0f,1.0f},
	{1.5f,0.0f,0.0f,1.0f,1.0f}
};

uint16 g_index[6] = {0,1,2,2,1,3};

CTexture* g_tex = 0;

CIndexedVertexs<STEXTUREDVERTEX>* g_pIndexedVertexs = 0;

CStaticMesh* g_pMesh = 0;

bool CTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");
  // ---

  m_vPos = Vect2f(-150,0);
  m_fVelocity = 1;

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

  //g_tex = new CTexture();
  //g_tex->Load("Data/Assets/Textures/gohan.png");
  g_tex = CORE->GetRenderManager()->GetTextureManager()->GetResource("Data/Assets/Textures/gohan.png");

  g_pIndexedVertexs = new CIndexedVertexs<STEXTUREDVERTEX>(RENDER_MANAGER, (char*)g_vertex, g_index, 4, 6);

  g_pMesh = new CStaticMesh();
  //g_pMesh->LoadSergi("D:/a.mesh");
  g_pMesh->Load("Data/Assets/Meshes/bmulti.mesh");

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
  //CHECKED_DELETE(g_tex); ja ho fa el texture manager
  
  CHECKED_DELETE(g_pIndexedVertexs);
  CHECKED_DELETE(g_pMesh);
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
  float l_fPitch, l_fYaw;

  Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

  l_fPitch = m_pObject->GetPitch();
  l_fYaw = m_pObject->GetYaw();
  
  m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
  m_pObject->SetPitch(l_fPitch-l_vVec.y*_fElapsedTime);

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

  /*pRM->DrawCube(Vect3f(2.0f,0.5f,0.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,0.5f,2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,0.5f,-2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,2.5f,0.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,2.5f,2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,2.5f,-2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,4.5f,0.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,4.5f,2.0f),1.0f,l_CubeCol);
  pRM->DrawCube(Vect3f(2.0f,4.5f,-2.0f),1.0f,l_CubeCol);*/

  t.Translate(m_pCube->GetPosition());
  r.SetFromAngleY(-m_pCube->GetYaw());
  r2.SetFromAngleZ(m_pCube->GetPitch());
  s.Scale(1.5f,1.5f,1.5f);

  total = t*r*r2*s;
  
  pRM->DrawGrid(30.0f,colCYAN,30,30);

  pRM->DrawAxis();

  pRM->SetTransform(total);

  pRM->DrawCube(1.0f,l_CubeCol);
 
  pRM->SetTransform(identity);
  pRM->DrawCamera(m_pCubeCamera);

  pRM->SetTransform(t.Translate(Vect3f(-2.0f,0.0f,0.0f)) * r.SetFromAngleY(FLOAT_PI_VALUE/2.0f));
  g_tex->Activate(0);
  pRM->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  g_pMesh->Render(pRM);

 
  pRM->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  pRM->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
  pRM->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  pRM->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  //g_pIndexedVertexs->Render(pRM);

  t.SetIdentity();
  r.SetIdentity();
  pRM->SetTransform(t.Translate(Vect3f(-2.0f,0.0f,3.0f)) * r.SetFromAngleY(FLOAT_PI_VALUE/2.0f));

  //g_pIndexedVertexs->Render(pRM);
  
  //pRM->SetTransform(s.Scale(0.1f,0.1f,0.1f));

  //g_pMesh->Render(pRM);

  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  string l_szMsg("Biogame");

  FONT_MANAGER->DrawText((uint32)m_vPos.x,(uint32)m_vPos.y,col,l_uiFontType,l_szMsg.c_str());
}

bool CTestProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "Reload Test Cube") == 0 )
  {
    g_pMesh->ReLoad();
    return true;
  }

  if(strcmp(_pcAction, "Run") == 0)
  {
    m_fVelocity = 10;
    return true;
  }

  if(strcmp(_pcAction, "Walk") == 0)
  {
    m_fVelocity = 1;
    return true;
  }

  if(strcmp(_pcAction, "MoveFwd") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);
    return true;
  }

 if(strcmp(_pcAction, "MoveBack") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x - cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z - sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);
    return true;
  }

  if(strcmp(_pcAction, "MoveLeft") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);
    return true;
  }

  if(strcmp(_pcAction, "MoveRight") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);
    return true;
  }

  if(strcmp(_pcAction, "SetCameraCube") == 0)
  {
    m_pCamera = m_pCubeCamera;
    return true;
  }
  
  if(strcmp(_pcAction, "SetCameraFPS") == 0)
  {
    m_pCamera = m_pObjectCamera;
    return true;
  }

  return false;
}

