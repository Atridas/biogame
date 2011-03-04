#include "ParticleProcess.h"
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
#include <sstream>
#include "VertexsStructs.h"
#include "Console.h"

#include <LightManager.h>
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "PhysicController.h"
#include "SpotLight.h"
#include "InstanceMesh.h"
#include "GameObject.h"



bool CParticleProcess::Init()
{
  m_pObject = new CObject3D();
  m_fVelocity = 1;
  
  m_pObject->SetPosition(Vect3f(-6,1.7f,0));

  m_pObjectBot = CORE->GetRenderableObjectsManager()->GetResource("bot");

  m_iState = 0;

  m_bStateChanged = false;

  

  m_pObjectCamera = new CThPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pObject,
    4.5f);

  

  m_pCamera = m_pObjectCamera;

  ((CThPSCamera*)m_pObjectCamera)->SetZoom(10.0f);
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");
  //m_pRenderPhysX = CORE->GetRenderableObjectsManager()->GetResource("maquina exploradora");

  if(l_Spot)
  {
    l_Spot->SetDirection(m_pObjectBot->GetPosition());
    l_Spot->SetActive(true);
  }
  m_bRenderLights = false;

  //m_ParticleEmitter.Init(CORE->GetRenderManager(),"./Data/Textures/Ch19p1_ParticleTexture.png");
  m_ParticleEmitter.Init(CORE->GetRenderManager(),"./Data/Textures/gohan.png"); 
  
  SetOk(true);
  return IsOk();
}

void CParticleProcess::Release()
{
  
}

void CParticleProcess::Update(float _fElapsedTime)
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

    m_ParticleEmitter.Update(_fElapsedTime);
  }
}

void CParticleProcess::RenderScene(CRenderManager* _pRM)
{
  //CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
   //l_pPhysManager->DebugRender(_pRM);



   /*if (m_pRenderPhysX!=0)
   {
     Mat44f l_vMat;
     g_pPActorBall->GetMat44(l_vMat);
     RenderPhysX(_pRM,m_pRenderPhysX,l_vMat);
   }*/
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
   
   //_pRM->DrawPlane(10,Vect3f(0,1,0),0,colBLUE,10,10);

  m_ParticleEmitter.Render();
}

void CParticleProcess::RenderINFO(CRenderManager* _pRM)
{
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
 
  int l_iPosicio = 0;
  int l_iPosicio2 = 130;
  string l_szMsg("Sense Objecte");
  
  stringstream l_SStreamHelp;

 
  _pRM->DrawAxis();
}


bool CParticleProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  float l_pVelocity = 1/20;

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

    Vect3f l_vDir = m_pCamera->GetDirection();

    l_vDir.Normalize();
    //l_vDir.y = 0.0f;
   // g_pPhysXController->Move(Vect3f(l_vDir.x,0.0f,l_vDir.z)*0.05f*m_fVelocity,_fDeltaSeconds);
    //m_pObject->SetPosition(g_pPhysXController->GetPosition());
   

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

    Vect3f l_vDir = m_pCamera->GetDirection();

    l_vDir.Normalize();
    //l_vDir.y = 0.0f;
    //g_pPhysXController->Move(Vect3f(-l_vDir.x,0.0f,-l_vDir.z)*0.05f*m_fVelocity,_fDeltaSeconds);
    //m_pObject->SetPosition(g_pPhysXController->GetPosition());

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

  /*
  if(strcmp(_pcAction, "ShootBall") == 0)
  {
    
    CPhysicActor* l_pPActorShoot = new CPhysicActor(g_pUserData4);
    l_pPActorShoot->AddSphereShape(0.3f,m_pCamera->GetEye());
    l_pPActorShoot->CreateBody(1);
    CORE->GetPhysicsManager()->AddPhysicActor(l_pPActorShoot);
    //m_pCamera->GetDirection();
    l_pPActorShoot->SetLinearVelocity(m_pCamera->GetDirection()*m_fPhysxVelocity);
    
    CHECKED_DELETE(l_pPActorShoot)

  }

  if(strcmp(_pcAction, "Elevate") == 0)
  {
    //g_pPActorComposite->SetLinearVelocity(Vect3f(0.0f,1.0f,0.0f)*m_fPhysxVelocity);
      CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
      const Vect3f l_PosCamera = m_pCamera->GetEye();
      const Vect3f& l_DirCamera = m_pCamera->GetDirection().Normalize();
  
      SCollisionInfo l_CInfo;
      //CPhysicUserData* l_pUserData = 0;
      if (g_pUserDataSHOOT != 0)
      {
        g_pUserDataSHOOT->SetColor(colWHITE);
      }
      
      g_pUserDataSHOOT = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,1,g_pUserDataSHOOT,l_CInfo);

      if (g_pUserDataSHOOT != 0)
      {
        g_pUserDataSHOOT->SetColor(colRED);
      }
     
  }

  if(strcmp(_pcAction, "ShootBOX") == 0)
  {
    CPhysicActor* l_pPActorShoot = new CPhysicActor(g_pUserData5);
    l_pPActorShoot->AddBoxSphape(0.8f,m_pCamera->GetEye());
    l_pPActorShoot->CreateBody(3);
    CORE->GetPhysicsManager()->AddPhysicActor(l_pPActorShoot);
    //m_pCamera->GetDirection();
    l_pPActorShoot->SetLinearVelocity(m_pCamera->GetDirection()*m_fPhysxVelocity);
    CHECKED_DELETE(l_pPActorShoot)
  }

  if(strcmp(_pcAction, "VelocityChange") == 0)
  {
    Vect3i l_vDelta = INPUT_MANAGER->GetMouseDelta();

    if (l_vDelta.z < 0)
    {
      m_fPhysxVelocity = m_fPhysxVelocity - 2;
      if (m_fPhysxVelocity == 0)
        m_fPhysxVelocity = 2;
      
    }
    else
    {
       m_fPhysxVelocity = m_fPhysxVelocity + 2;
      if (m_fPhysxVelocity == 1000)
        m_fPhysxVelocity = 998;
      
    }
 
    
  }

  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    Vect3i l_vDelta = INPUT_MANAGER->GetMouseDelta();

    if (l_vDelta.z < 0)
    {
      ((CThPSCamera*)m_pObjectCamera)->AddZoom(0.3f);
    }else{
      ((CThPSCamera*)m_pObjectCamera)->AddZoom(-0.3f);
    }

    return true;
  }

  */

  return false;
}

