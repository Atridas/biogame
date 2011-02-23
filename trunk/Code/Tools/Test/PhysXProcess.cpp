#include "PhysXProcess.h"
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
#include "Console.h"

#include <LightManager.h>
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "SpotLight.h"

CPhysicUserData* g_pUserData;
CPhysicUserData* g_pUserData2;
CPhysicActor* g_pPActorPlane;
CPhysicActor* g_pPActorBall;

bool CPhysXProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");

  CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
  
  m_pObject = new CObject3D();
  m_fVelocity = 1;
  
  m_pObject->SetPosition(Vect3f(-6,1.7f,0));

  m_pObjectBot = CORE->GetRenderableObjectsManager()->GetResource("bot");

  m_iState = 0;

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
    4.5f);

  m_pCamera = m_pObjectCamera;
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");

  if(l_Spot)
  {
    l_Spot->SetDirection(m_pObjectBot->GetPosition());
    l_Spot->SetActive(true);
  }
  m_bRenderLights = false;


  //PHYSICS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  g_pUserData = new CPhysicUserData();
  g_pUserData2 = new CPhysicUserData();
  g_pUserData->SetPaint(true);
  g_pUserData->SetColor(colWHITE);
  g_pUserData2->SetPaint(true);
  g_pUserData2->SetColor(colGREEN);


  g_pPActorBall = new CPhysicActor(g_pUserData2);
  g_pPActorPlane = new CPhysicActor(g_pUserData);

  //Plane
  g_pPActorPlane->AddPlaneShape(Vect3f(0,1,0),0);

  //Ball
  g_pPActorBall->AddSphereShape(1);
  g_pPActorBall->SetGlobalPosition(Vect3f(0,6,0));
  g_pPActorBall->CreateBody(0.1f);

  l_pPhysManager->AddPhysicActor(g_pPActorPlane);
  l_pPhysManager->AddPhysicActor(g_pPActorBall);
  

  //CORE->GetRenderableObjectsManager()->SetAllVisibility(false);

  

  SetOk(true);
  return IsOk();
}

void CPhysXProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Release");

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pObject)
  CHECKED_DELETE(g_pPActorPlane)
  CHECKED_DELETE(g_pUserData)
  CHECKED_DELETE(g_pUserData2)
  CHECKED_DELETE(g_pPActorBall)

  
  
}

void CPhysXProcess::Update(float _fElapsedTime)
{
  if(m_pObject && m_pObjectBot) 
  {
    //Actualitze el pitch i el yaw segons els delta del mouse
    float l_fPitch, l_fYaw;

    Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

    l_fPitch = m_pObject->GetPitch();
    l_fYaw = m_pObject->GetYaw();
  
    m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
    m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);

    l_fPitch -= l_vVec.y*_fElapsedTime;
    if(l_fPitch < - FLOAT_PI_VALUE/3) l_fPitch = - FLOAT_PI_VALUE/3;
    if(l_fPitch >   FLOAT_PI_VALUE/3) l_fPitch =   FLOAT_PI_VALUE/3;
    m_pObject->SetPitch(l_fPitch);

    m_pObjectBot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObjectBot->GetPosition().y, m_pObject->GetPosition().z));
  
    l_fPitch = m_pObject->GetPitch();
    //l_fPitch = l_fPitch+FLOAT_PI_VALUE/2;
    l_fYaw = m_pObjectBot->GetYaw();
    l_fYaw = l_fYaw+FLOAT_PI_VALUE/2;
    //l_fRoll = m_pObjectBot->GetRoll();

    CSpotLight* l_pSpot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");
    if(l_pSpot)
    {
      l_pSpot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObject->GetPosition().y, m_pObject->GetPosition().z));

      //CDirectionalLight* l_dir = (CDirectionalLight*)CORE->GetLightManager()->GetResource("Direct01");
      Vect3f l_vec(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch),sin(l_fYaw) * cos(l_fPitch) );
      l_pSpot->SetDirection(Vect3f(l_vec.x,l_vec.y,l_vec.z));
    }

    if(m_bStateChanged)
    {
      ((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->BlendCycle(m_iState,0);

      m_bStateChanged = false;
    }
  }
}

void CPhysXProcess::RenderScene(CRenderManager* _pRM)
{

    CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
   l_pPhysManager->DebugRender(_pRM);
  //Render Objects
  //CORE->GetRenderableObjectsManager()->Render(_pRM);

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
  //_pRM->DrawGrid(30.0f,colCYAN,30,30);
   
   //_pRM->DrawPlane(10,Vect3f(0,1,0),0,colBLUE,10,10);

}

void CPhysXProcess::RenderINFO(CRenderManager* _pRM)
{
  /*
  D3DXVECTOR3 vecPos = D3DXVECTOR3(0,0,0);
  g_pD3DXSprite->Begin(0);
  g_pD3DXSprite->Draw(_pRM->GetTextureManager()->GetResource("ZBlurTexture")->GetD3DTexture(), NULL, NULL, &vecPos, 0xffffffff);
  g_pD3DXSprite->End();
  */
  /*
  vecPos = D3DXVECTOR3(0,382,0);
  g_pD3DXSprite2->Begin(0);
  g_pD3DXSprite2->Draw(_pRM->GetTextureManager()->GetResource("DownSampledGlow2")->GetD3DTexture(), NULL, NULL, &vecPos, 0xffffffff);
	g_pD3DXSprite2->End();
  */
  

  //_pRM->DrawQuad2D(pos, 100, 100, UPPER_LEFT, colGREEN);

  _pRM->EnableAlphaBlend();

  Vect2i pos(40, 40);

  
  CTexture* l_pTexture = CORE->GetTextureManager()->GetResource("Data/Textures/gohan.png");
  CTexture* l_pTexture2 = CORE->GetTextureManager()->GetResource("Data/Textures/gohan.png");
 // _pRM->EnableAlphaBlend();
  if(l_pTexture)
  {
    assert(l_pTexture->IsOk());
    l_pTexture->Activate(0);
    _pRM->DrawTexturedQuad2D(pos, l_pTexture->GetWidth(), l_pTexture->GetHeight(), UPPER_LEFT);
  }

  _pRM->DisableAlphaBlend();
   _pRM->DrawAxis();
}

bool CPhysXProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
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

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

 if(strcmp(_pcAction, "MoveBack") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x - cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z - sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveLeft") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveRight") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();
    l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
    m_pObject->SetPosition(l_vPos);

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  return false;
}

