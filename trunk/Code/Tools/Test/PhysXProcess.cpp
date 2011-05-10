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
#include "PhysicFixedJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicSphericalJoint.h"
#include "SpotLight.h"
#include "InstanceMesh.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "PhysicCookingMesh.h"
#include <cal3d/cal3d.h>
//---PhysX Includes---//
#undef min
#undef max
//---------------------//

//typedef enum ETypeFunction { 
//      COLISION_STATIC_MASK = 0,
//      COLISION_SOLID_MASK = 1, 
//      COLISION_SCENE_MASK = 2
//  };

#define COLISION_STATIC_MASK 0
#define COLISION_SOLID_MASK 1
#define COLISION_SCENE_MASK 2
#define ALTURA_CONTROLLER 1.5f
#define RADIUS_CONTROLLER 0.3f
#define ALTURA_TOTAL ((ALTURA_CONTROLLER+2*RADIUS_CONTROLLER)*0.5f + 0.1f)

#define COLISIONABLE_MASK ((1<<GROUP_COLLIDABLE_NON_PUSHABLE))


vector<SCollisionInfo> g_vCollisions;
CPhysicUserData* g_pUserData;
CPhysicUserData* g_pUserDataController;
//CPhysicUserData* g_pUserDataEscala;
//CPhysicActor* g_pPActorPlane;
//CPhysicActor* g_pPEscala;


CPhysicUserData* g_pUserDataSHOOT = 0;
CGameObject* g_pGameObject = 0;
CPhysicController* g_pPhysXController = 0;
CRenderableAnimatedInstanceModel* g_pCharacter = 0;


//PROVES DE JOINTS
CPhysicActor* g_pJointActor1;
CPhysicActor* g_pJointActor2;
CPhysicActor* g_pJointActor3;

CPhysicUserData* g_pUserDataJoint1;
CPhysicUserData* g_pUserDataJoint2;
CPhysicUserData* g_pUserDataJoint3;

CPhysicFixedJoint* g_pFixedJoint;
CPhysicFixedJoint* g_pFixedJoint2;
CPhysicRevoluteJoint* g_pRevoluteJoint;
CPhysicSphericalJoint* g_pSphericalJoint;
CPhysicSphericalJoint* g_pSphericalJoint2;

//MANAGER DE GAMEOBJECTS
CGameObjectManager* g_pObjectManager = 0;

bool CPhysXProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CPhysXProcess::Init");

  CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
  
  m_pObject = new CObject3D();
  m_fVelocity = 2;
  
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

  ((CThPSCamera*)m_pObjectCamera)->SetZoom(5.0f);
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");
 

  CalCoreSkeleton* l_pSkeleton = CORE->GetAnimatedModelManager()->GetResource("riggle")->GetCoreModel()->getCoreSkeleton();
  

  g_pUserDataJoint1 = new CPhysicUserData("Objecte Joint 1");
  g_pUserDataJoint2 = new CPhysicUserData("Objecte Joint 2");
  g_pUserDataJoint3 = new CPhysicUserData("Objecte Joint 3");
  g_pUserDataJoint1->SetPaint(true);
  g_pUserDataJoint1->SetColor(colYELLOW);
  g_pUserDataJoint2->SetPaint(true);
  g_pUserDataJoint2->SetColor(colRED);
  g_pUserDataJoint3->SetPaint(true);
  g_pUserDataJoint3->SetColor(colGREEN);


  g_pJointActor1 = new CPhysicActor(g_pUserDataJoint1);
  g_pJointActor2 = new CPhysicActor(g_pUserDataJoint2);
  g_pJointActor3 = new CPhysicActor(g_pUserDataJoint3);


  g_pJointActor1->AddBoxSphape(Vect3f(0.5f,0.5f,0.5f),v3fZERO,NULL,GROUP_COLLIDABLE_PUSHABLE);
  g_pJointActor2->AddSphereShape(0.4f,v3fZERO,NULL,GROUP_COLLIDABLE_PUSHABLE);
  g_pJointActor1->SetGlobalPosition(Vect3f(-6.0f,1.0f,-3.0f));
  g_pJointActor2->SetGlobalPosition(Vect3f(-7.0f,1.0f,-3.0f));

  //g_pJointActor1->CreateBody(1.0f);
  g_pJointActor2->CreateBody(1.0f);
  
  g_pFixedJoint = new CPhysicFixedJoint();
  g_pFixedJoint2 = new CPhysicFixedJoint();
  g_pRevoluteJoint = new CPhysicRevoluteJoint();
  g_pSphericalJoint = new CPhysicSphericalJoint();
  g_pSphericalJoint2 = new CPhysicSphericalJoint();
  //g_pSphericalJoint->GetPhXDescJoint()->

  //l_pPhysManager->AddPhysicActor(g_pJointActor1);
  //l_pPhysManager->AddPhysicActor(g_pJointActor2);

  //g_pFixedJoint->SetInfo(g_pJointActor1,g_pJointActor2);
  //g_pRevoluteJoint->SetInfo(v3fZERO,Vect3f(0.1f,0.1f,0.1f),g_pJointActor1,g_pJointActor2);
  //g_pSphericalJoint->SetInfo(Vect3f(-6.0f,2.0f,-3.0f),g_pJointActor2);
  //g_pRevoluteJoint->SetMotor(1.0f,1.0f);

  //g_pFixedJoint->CreateJoint(NULL);

  //l_pPhysManager->AddPhysicFixedJoint(g_pFixedJoint);
  //l_pPhysManager->AddPhysicRevoluteJoint(g_pRevoluteJoint);
  //l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint);




  g_pObjectManager = new CGameObjectManager();
  g_pObjectManager->Load("Data/Levels/PhysX/XML/GameObjects.xml",false);
  CPhysicActor* l_pLampada = g_pObjectManager->GetResource("Lampada")->GetPhysXActor();
  CPhysicActor* l_pCable = g_pObjectManager->GetResource("Cable_Lampada")->GetPhysXActor();
  Vect3f l_vMax = g_pObjectManager->GetResource("Cable_Lampada")->GetRenderableObject()->GetBoundingBox()->GetDimension();
  Vect3f l_vPos = g_pObjectManager->GetResource("Lampada")->GetRenderableObject()->GetPosition();
  Vect3f l_vPos2 = g_pObjectManager->GetResource("Cable_Lampada")->GetRenderableObject()->GetPosition();
  Vect3f l_vPos3 = l_vPos2;

  l_vPos2.y = l_vPos2.y - (l_vMax.y*0.5f); 
  l_vPos3.y = l_vPos3.y + (l_vMax.y*0.5f); 


  SCollisionInfo l_SInfo;
  l_SInfo.m_CollisionPoint = l_vPos2;
  l_SInfo.m_Normal = Vect3f(0.0f,-1.0f,0.0f);
  g_vCollisions.push_back(l_SInfo);

  l_SInfo.m_CollisionPoint = l_vPos3;
  g_vCollisions.push_back(l_SInfo);


  g_pSphericalJoint->SetInfo(l_vPos2,l_pLampada,l_pCable);
  g_pSphericalJoint2->SetInfo(l_vPos3,l_pCable);
  //g_pSphericalJoint2->SetInfo(l_vPos2,l_pLampada);
  //g_pFixedJoint->SetInfo(l_pCable,l_pLampada);


  l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint);
  l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint2);
  //l_pPhysManager->AddPhysicFixedJoint(g_pFixedJoint);
  //l_pPhysManager->AddPhysicRevoluteJoint(g_pRevoluteJoint);
  
  
  
 


  //CHARACTER CONTROLLER
  g_pUserDataController = new CPhysicUserData("PhysX Controller");
  g_pUserDataController->SetPaint(true);
  g_pUserDataController->SetColor(colBLACK);

  g_pPhysXController = new CPhysicController(RADIUS_CONTROLLER,ALTURA_CONTROLLER,10.0f,0.1f,0.5f,COLISIONABLE_MASK,g_pUserDataController,Vect3f(-7.0f,2.2f,-4.0f));
  g_pPhysXController->SetYaw(-90);
  l_pPhysManager->AddPhysicController(g_pPhysXController);

  m_pObject->SetPosition(g_pPhysXController->GetPosition());

  //g_pObjectManager = new CGameObjectManager();
  //g_pObjectManager->Load("Data/Levels/PhysX/XML/GameObjects.xml",false);


  //Init del character de Riggle
  g_pCharacter = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("ariggle");

  if (g_pCharacter != 0)
  {
    Vect3f l_vPos = g_pPhysXController->GetPosition();
    l_vPos.y = 0.0f;
    g_pCharacter->SetPosition(l_vPos);
    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle("idle",0);
  }

  Vect3f l_ControllerPos = g_pPhysXController->GetPosition();
  m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));

  
  if(l_Spot)
  {
    l_Spot->SetDirection(l_ControllerPos);
    l_Spot->SetActive(true);
  }
  m_bRenderLights = false;


  CORE->GetLightManager()->SetLightsEnabled(true);
  l_pPhysManager->SetDebugRenderMode(true);

  SetOk(true);
  return IsOk();





  


  //PHYSICS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  /*g_pUserData = new CPhysicUserData("Plane");
  g_pUserDataEscala = new CPhysicUserData("Escala");*/
  /*g_pUserData->SetPaint(false);
  g_pUserData->SetColor(colWHITE);*/
  /*g_pUserDataEscala->SetPaint(true);
  g_pUserDataEscala->SetColor(colYELLOW);*/
  //ESCALA
  /*g_pPEscala->AddBoxSphape(Vect3f(0.5f,0.3f,2.0f),Vect3f(-10.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,0.6f,2.0f),Vect3f(-11.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,0.9f,2.0f),Vect3f(-12.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,1.2f,2.0f),Vect3f(-13.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,1.5f,2.0f),Vect3f(-14.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,1.8f,2.0f),Vect3f(-15.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,2.1f,2.0f),Vect3f(-16.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,2.4f,2.0f),Vect3f(-17.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,2.7f,2.0f),Vect3f(-18.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,3.0f,2.0f),Vect3f(-19.0f,0.0f,0.0f));
  g_pPEscala->SetGlobalPosition(Vect3f(5.0f,0.0f,5.0f));*/

  
  //g_pPActorPlane->AddBoxSphape(Vect3f(500.f,1.0f,500.f),Vect3f(0.0f,-1.0f,0.0f),NULL,GROUP_COLLIDABLE_NON_PUSHABLE);
  //l_pPhysManager->AddPhysicActor(g_pPActorPlane);

  //g_pPActorPlane = new CPhysicActor(g_pUserData);
  //g_pPActorComposite = new CPhysicActor(g_pUserData3);
  //g_pPEscala = new CPhysicActor(g_pUserDataEscala);
  /////////////////////////////////////////////////////////////////////////////////



}

void CPhysXProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Release");

  //DELETES DE OBJECTS
  g_pObjectManager->CleanUp();

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pObject)
  CHECKED_DELETE(g_pUserData)
  CHECKED_DELETE(g_pGameObject)
  CHECKED_DELETE(g_pPhysXController)
  CHECKED_DELETE(g_pUserDataController)
  CHECKED_DELETE(g_pObjectManager)


  CHECKED_DELETE(g_pUserDataJoint1)
  CHECKED_DELETE(g_pUserDataJoint2)
  CHECKED_DELETE(g_pUserDataJoint3)
  CHECKED_DELETE(g_pJointActor1)
  CHECKED_DELETE(g_pJointActor2)
  CHECKED_DELETE(g_pJointActor3)
  CHECKED_DELETE(g_pFixedJoint)
  CHECKED_DELETE(g_pFixedJoint2)
  CHECKED_DELETE(g_pRevoluteJoint);
  CHECKED_DELETE(g_pSphericalJoint);
  CHECKED_DELETE(g_pSphericalJoint2);

  g_vCollisions.clear();

  
}

void CPhysXProcess::Update(float _fElapsedTime)
{
  if(m_pObject) 
  {
    //Actualitze el pitch i el yaw segons els delta del mouse
    float l_fPitch, l_fYaw;

    Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

    l_fPitch = m_pObject->GetPitch();
    l_fYaw = m_pObject->GetYaw();
  
    m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
    //m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);

    l_fPitch -= l_vVec.y*_fElapsedTime;
    if(l_fPitch < - FLOAT_PI_VALUE/3) l_fPitch = - FLOAT_PI_VALUE/3;
    if(l_fPitch >   FLOAT_PI_VALUE/3) l_fPitch =   FLOAT_PI_VALUE/3;
    m_pObject->SetPitch(l_fPitch);

    //m_pObjectBot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObjectBot->GetPosition().y, m_pObject->GetPosition().z));
  
    l_fPitch = m_pObject->GetPitch();
    //l_fPitch = l_fPitch+FLOAT_PI_VALUE/2;
    l_fYaw = m_pObject->GetYaw();
    l_fYaw = l_fYaw-FLOAT_PI_VALUE/2;
    //l_fRoll = m_pObjectBot->GetRoll();

    if (g_pCharacter)
    g_pCharacter->SetYaw(l_fYaw);

    CSpotLight* l_pSpot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");
    if(l_pSpot)
    {
      l_pSpot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObject->GetPosition().y, m_pObject->GetPosition().z));

      //CDirectionalLight* l_dir = (CDirectionalLight*)CORE->GetLightManager()->GetResource("Direct01");
      Vect3f l_vec(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch),sin(l_fYaw) * cos(l_fPitch) );
      l_pSpot->SetDirection(Vect3f(l_vec.x,l_vec.y,l_vec.z));
    }

   /* if(m_bStateChanged)
    {
      ((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->BlendCycle(m_iState,0);

      m_bStateChanged = false;
    }*/

    
  }

  /*CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
  const Vect3f l_PosCamera = m_pCamera->GetEye();
  const Vect3f& l_DirCamera = m_pCamera->GetDirection().Normalize();
  
  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = 0;
  l_pUserData = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,1,l_pUserData,l_CInfo);
  
  if (l_pUserData != 0)
  {
    
  }*/

  /*if (m_pRenderPhysX != 0)
  {
    g_pGameObject->Update(_fElapsedTime);
  }*/
  g_pPhysXController->Move(Vect3f(0.0f,0.0f,0.0f),_fElapsedTime);
  Vect3f l_ControllerPos = g_pPhysXController->GetPosition();
  m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));

  /*g_pGameBox02->Update(_fElapsedTime);
  g_pGameBox03->Update(_fElapsedTime);
  g_pGameBox04->Update(_fElapsedTime);
  g_pGameBox05->Update(_fElapsedTime);
  g_pGameBox06->Update(_fElapsedTime);
  g_pGameBox07->Update(_fElapsedTime);*/



  g_pObjectManager->Update(_fElapsedTime);
  //g_pGameObject->Update(_fElapsedTime);


  if ((!INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_W)) &&
      (!INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_S)) &&
      (!INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_A)) &&
      (!INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_D)))
  {
    if(g_pCharacter)
    {
	    if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() == 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
        g_pCharacter->GetAnimatedInstanceModel()->BlendCycle("idle",0.3f);
	    }
    }
  }

}

void CPhysXProcess::RenderScene(CRenderManager* _pRM)
{

   CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
   l_pPhysManager->DebugRender(_pRM);


   RenderImpacts(_pRM);



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
  //_pRM->DrawGrid(30.0f,colCYAN,30,30);
   
   //_pRM->DrawPlane(10,Vect3f(0,1,0),0,colBLUE,10,10);

  

}


void CPhysXProcess::RenderImpacts(CRenderManager* _pRM)
{
  Mat44f t;  
  for (int i=0;i<(int)g_vCollisions.size();++i)
  {

      t.SetIdentity();
      t.Translate(g_vCollisions[i].m_CollisionPoint);
      _pRM->SetTransform(t);
      _pRM->DrawSphere(0.1f,colYELLOW,5);
      _pRM->DrawLine(v3fZERO,g_vCollisions[i].m_Normal*0.5f,colGREEN);
  }


}


void CPhysXProcess::RenderINFO(CRenderManager* _pRM)
{

  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  uint32 l_uiFontType2 = FONT_MANAGER->GetTTF_Id("arial");
 
  int l_iPosicio = 0;
  int l_iPosicio2 = 130;
  string l_szMsg("Sense Objecte");
  //stringstream l_SStream;
  stringstream l_SStreamHelp,l_SStreamInfo;


  //

  if (g_pUserDataSHOOT != 0)
  {
    l_SStreamHelp << "Objecte Tocat: " << endl << g_pUserDataSHOOT->GetName() << endl;
    FONT_MANAGER->DrawText(l_iPosicio,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
  }
  else
  {
    l_SStreamHelp << "Objecte Tocat: " << endl << l_szMsg << endl;
    FONT_MANAGER->DrawText(l_iPosicio,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
  }

  l_SStreamInfo << "Mostrar Malles del PhysX:" << endl << "Tecla R" << endl;
  FONT_MANAGER->DrawText(l_iPosicio,l_iPosicio2-60,colYELLOW,l_uiFontType,l_SStreamInfo.str().c_str());


 
  _pRM->DrawAxis();
}

bool CPhysXProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  float l_pVelocity = 1/20;

  if(strcmp(_pcAction, "Run") == 0)
  {
    m_fVelocity = 7;
    
    return true;
  }

  if(strcmp(_pcAction, "Walk") == 0)
  {
    m_fVelocity = 2;
    
    return true;
  }

  if(strcmp(_pcAction, "MoveFwd") == 0)
  {
    Vect3f l_vDir = m_pCamera->GetDirection();

    
    l_vDir.Normalize();
    g_pPhysXController->Move(Vect3f(l_vDir.x,0.0f,l_vDir.z)*_fDeltaSeconds*m_fVelocity,_fDeltaSeconds);
    Vect3f l_ControllerPos = g_pPhysXController->GetPosition();
    m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));
    if (g_pCharacter)
    {
      g_pCharacter->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y-ALTURA_TOTAL,l_ControllerPos.z));
      if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

 if(strcmp(_pcAction, "MoveBack") == 0)
  {
    Vect3f l_vDir = m_pCamera->GetDirection();

    
    l_vDir.Normalize();
    g_pPhysXController->Move(Vect3f(-l_vDir.x,0.0f,-l_vDir.z)*_fDeltaSeconds*m_fVelocity,_fDeltaSeconds);
    Vect3f l_ControllerPos = g_pPhysXController->GetPosition();
    m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));
    if (g_pCharacter)
    {
      g_pCharacter->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y-ALTURA_TOTAL,l_ControllerPos.z));
      if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveLeft") == 0)
  {
    Vect3f l_vDir = m_pObject->GetPosition();
    l_vDir.x = cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2);
    l_vDir.z = sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2);
    l_vDir.y = 0.0f;
 
    l_vDir.Normalize();
    g_pPhysXController->Move(Vect3f(l_vDir.x,0.0f,l_vDir.z)*_fDeltaSeconds*m_fVelocity,_fDeltaSeconds);
    Vect3f l_ControllerPos = g_pPhysXController->GetPosition();
    m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));
    if (g_pCharacter)
    {
      g_pCharacter->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y-ALTURA_TOTAL,l_ControllerPos.z));
      if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }
    }


    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveRight") == 0)
  {
    Vect3f l_vDir = m_pObject->GetPosition();
    l_vDir.x = cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2);
    l_vDir.z = sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2);
    l_vDir.y = 0.0f;

    l_vDir.Normalize();
    g_pPhysXController->Move(Vect3f(-l_vDir.x,0.0f,-l_vDir.z)*_fDeltaSeconds*m_fVelocity,_fDeltaSeconds);
    Vect3f l_ControllerPos = g_pPhysXController->GetPosition();
    m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y,l_ControllerPos.z));
    if (g_pCharacter)
    {
      g_pCharacter->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y-ALTURA_TOTAL,l_ControllerPos.z));
      if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }


  if(strcmp(_pcAction, "Elevate") == 0)
  {
    
    //CPhysicActor* l_pPActorShoot = new CPhysicActor(g_pUserData);
    //l_pPActorShoot->AddSphereShape(0.3f,m_pCamera->GetEye());
    //l_pPActorShoot->CreateBody(1);
    //CORE->GetPhysicsManager()->AddPhysicActor(l_pPActorShoot);
    ////m_pCamera->GetDirection();
    //l_pPActorShoot->SetLinearVelocity(m_pCamera->GetDirection()*m_fPhysxVelocity);
    //
    //CHECKED_DELETE(l_pPActorShoot)

  }

  if(strcmp(_pcAction, "ShootBall") == 0)
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
      
      g_pUserDataSHOOT = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,2,g_pUserDataSHOOT,l_CInfo);
      g_vCollisions.push_back(l_CInfo);


     /* CORE->GetPhysicsManager()->RelasePhysicSphericalJoint(g_pSphericalJoint);
      CORE->GetPhysicsManager()->RelasePhysicSphericalJoint(g_pSphericalJoint2);*/

      if (g_pUserDataSHOOT != 0)
      {
        g_pUserDataSHOOT->SetColor(colRED);
      }

     /* g_Box02->CreateBody(0.1f);
      g_Box03->CreateBody(0.1f);
      g_Box04->CreateBody(0.1f);
      g_Box05->CreateBody(0.1f);
      g_Box06->CreateBody(0.1f);
      g_Box07->CreateBody(0.1f);*/
      
     
  }

  if(strcmp(_pcAction, "ShootBOX") == 0)
  {
    //CPhysicActor* l_pPActorShoot = new CPhysicActor(g_pUserData);
    //l_pPActorShoot->AddBoxSphape(0.8f,m_pCamera->GetEye());
    //l_pPActorShoot->CreateBody(3);
    //CORE->GetPhysicsManager()->AddPhysicActor(l_pPActorShoot);
    ////m_pCamera->GetDirection();
    //l_pPActorShoot->SetLinearVelocity(m_pCamera->GetDirection()*m_fPhysxVelocity);
    //CHECKED_DELETE(l_pPActorShoot)
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

  if(strcmp(_pcAction, "RenderPhysX") == 0)
  {
    bool l_bRender = CORE->GetPhysicsManager()->GetDebugRenderMode();
    CORE->GetPhysicsManager()->SetDebugRenderMode(!l_bRender);
  }
  
  return false;
}


void CPhysXProcess::RenderPhysX(CRenderManager* _pRM, CRenderableObject* _pRO, Mat44f _mMatTransf)
{
    //_pRO->GetBoundingBox()->GetMiddlePoint();
   Mat44f t;
   t.SetIdentity();

   t.Translate(Vect3f(0.0f,-_pRO->GetBoundingBox()->GetMiddlePoint().y,0.0f));
   _pRM->SetTransform(_mMatTransf*t);
   //(StaticMesh*)
   

   CInstanceMesh* l_pInstanceMesh = (CInstanceMesh*)_pRO;
   l_pInstanceMesh->RenderRenderableObject(_pRM);
  // _pRM->RenderBoundingBox(l_pInstanceMesh->GetBoundingBox());
}


  



