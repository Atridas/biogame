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
#include "SpotLight.h"
#include "InstanceMesh.h"
#include "GameObject.h"
#include "GameObjectManager.h"

//typedef enum ETypeFunction { 
//      COLISION_STATIC_MASK = 0,
//      COLISION_SOLID_MASK = 1, 
//      COLISION_SCENE_MASK = 2
//  };

#define COLISION_STATIC_MASK 0
#define COLISION_SOLID_MASK 1
#define COLISION_SCENE_MASK 2

#define COLISIONABLE_MASK ((1<<COLISION_SCENE_MASK) || (1<<COLISION_STATIC_MASK) || (1<<COLISION_SOLID_MASK))


CPhysicUserData* g_pUserData;
CPhysicUserData* g_pUserData2;
CPhysicUserData* g_pUserData3;
CPhysicUserData* g_pUserData4;
CPhysicUserData* g_pUserData5;
CPhysicUserData* g_pUserDataController;
CPhysicUserData* g_pUserDataEscala;
CPhysicActor* g_pPActorPlane;
CPhysicActor* g_pPActorBall;
CPhysicActor* g_pPActorComposite;
CPhysicActor* g_pPEscala;
CPhysicUserData* g_pBoxes;


CPhysicActor* g_Box02;
CPhysicActor* g_Box03;
CPhysicActor* g_Box04;
CPhysicActor* g_Box05;
CPhysicActor* g_Box06;
CPhysicActor* g_Box07;

CGameObject* g_pGameBox02;
CGameObject* g_pGameBox03;
CGameObject* g_pGameBox04;
CGameObject* g_pGameBox05;
CGameObject* g_pGameBox06;
CGameObject* g_pGameBox07;


CPhysicFixedJoint* g_pJoint;
CPhysicFixedJoint* g_pJoint2;
CPhysicFixedJoint* g_pJoint3;
CPhysicFixedJoint* g_pJoint4;
CPhysicFixedJoint* g_pJoint5;

CPhysicUserData* g_pUserDataSHOOT = 0;
CGameObject* g_pGameObject = 0;
CPhysicController* g_pPhysXController = 0;


//MANAGER DE GAMEOBJECTS
CGameObjectManager* g_pObjectManager = 0;

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

  
  CORE->GetLightManager()->CreateDirectionalLight("ObjectModeLight",
                                                  Vect3f(0.0f),
                                                  Vect3f(1.0f,1.0f,1.0f),
                                                  CColor(Vect3f(1.0f,1.0f,1.0f)),
                                                  50.0f,
                                                  80.0f,
                                                  false);

  CORE->GetLightManager()->CreateSpotLight("FreeModeLight",
                                          Vect3f(-2.15715f,0.0f,-7.32758f),
                                          Vect3f(-5.4188f,0.0f,3.75613f),
                                          CColor(Vect3f(1.0f,1.0f,1.0f)),
                                          20.0f,
                                          80.0f,
                                          10.0f,
                                          45.0f,
                                          true);

  m_pCamera = m_pObjectCamera;

  ((CThPSCamera*)m_pObjectCamera)->SetZoom(10.0f);
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");
 

  if(l_Spot)
  {
    l_Spot->SetDirection(m_pObjectBot->GetPosition());
    l_Spot->SetActive(true);
  }
  m_bRenderLights = false;


  //PHYSICS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  g_pUserData = new CPhysicUserData("Plane");
  g_pUserData2 = new CPhysicUserData("CaixaNormalMap");
  g_pUserData3 = new CPhysicUserData("Composite");
  g_pUserData4 = new CPhysicUserData("Pilota");
  g_pUserData5 = new CPhysicUserData("Cub");
  g_pUserDataEscala = new CPhysicUserData("Escala");
  g_pUserDataController = new CPhysicUserData("PhysX Controller");
  g_pUserData->SetPaint(false);
  g_pUserData->SetColor(colWHITE);
  g_pUserData2->SetPaint(true);
  g_pUserData2->SetColor(colWHITE);
  g_pUserData3->SetPaint(true);
  g_pUserData3->SetColor(colWHITE);
  g_pUserData4->SetPaint(true);
  g_pUserData4->SetColor(colYELLOW);
  g_pUserData5->SetPaint(true);
  g_pUserData5->SetColor(colCYAN);
  g_pUserDataController->SetPaint(true);
  g_pUserDataController->SetColor(colBLACK);
  g_pUserDataEscala->SetPaint(true);
  g_pUserDataEscala->SetColor(colYELLOW);
  


  g_pPActorPlane = new CPhysicActor(g_pUserData);
  g_pPActorComposite = new CPhysicActor(g_pUserData3);
  g_pPEscala = new CPhysicActor(g_pUserDataEscala);

  //ESCALA
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,0.3f,2.0f),Vect3f(-10.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,0.6f,2.0f),Vect3f(-11.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,0.9f,2.0f),Vect3f(-12.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,1.2f,2.0f),Vect3f(-13.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,1.5f,2.0f),Vect3f(-14.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,1.8f,2.0f),Vect3f(-15.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,2.1f,2.0f),Vect3f(-16.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,2.4f,2.0f),Vect3f(-17.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,2.7f,2.0f),Vect3f(-18.0f,0.0f,0.0f));
  g_pPEscala->AddBoxSphape(Vect3f(0.5f,3.0f,2.0f),Vect3f(-19.0f,0.0f,0.0f));


  //Actor estatic en forma de pla per fer probes amb el charactercontroller:
  //CPhysicActor* plaXung = new CPhysicActor(g_pUserData2);
  g_pPActorPlane->AddBoxSphape(Vect3f(100.f,1.0f,100.f),Vect3f(0.0f,-1.0f,0.0f));
  //g_pPActorPlane->CreateBody(100.0f);


  //Composite
  g_pPActorComposite->AddBoxSphape(2,Vect3f(0,7,0));
  g_pPActorComposite->AddSphereShape(1,Vect3f(2,4,2));
  g_pPActorComposite->AddSphereShape(1,Vect3f(2,4,-2));
  g_pPActorComposite->AddSphereShape(1,Vect3f(-2,4,-2));
  g_pPActorComposite->AddSphereShape(1,Vect3f(-2,4,2));
  

  g_pPActorComposite->CreateBody(0.6f);
  g_pPActorComposite->SetGlobalPosition(Vect3f(10,4,0));
  

  //l_pPhysManager->AddPhysicActor(g_pPActorPlane);
  
  l_pPhysManager->AddPhysicActor(g_pPActorComposite);
  l_pPhysManager->AddPhysicActor(g_pPActorPlane);
  l_pPhysManager->AddPhysicActor(g_pPEscala);
  

  g_pPhysXController = new CPhysicController(0.5f,3.0f,10.0f,0.1f,0.5f,COLISION_SOLID_MASK,g_pUserDataController,Vect3f(5.0f,2.2f,0.0f));
  g_pPhysXController->SetYaw(-90);
  l_pPhysManager->AddPhysicController(g_pPhysXController);

  m_pObject->SetPosition(g_pPhysXController->GetPosition());

  //Vect3f l_vBoxDim2(1.0f,1.0f,1.0f);
  //Vect3f l_vBoxDim3(1.0f,1.0f,1.0f);
  //Vect3f l_vBoxDim4(1.0f,1.0f,1.0f);
  //Vect3f l_vBoxDim5(1.0f,1.0f,1.0f);
  //Vect3f l_vBoxDim6(1.0f,1.0f,1.0f);
  //Vect3f l_vBoxDim7(1.0f,1.0f,1.0f);

  //l_vBoxDim2 = CORE->GetRenderableObjectsManager()->GetResource("Box02")->GetBoundingBox()->GetDimension();
  //l_vBoxDim3 = CORE->GetRenderableObjectsManager()->GetResource("Box03")->GetBoundingBox()->GetDimension();
  //l_vBoxDim4 = CORE->GetRenderableObjectsManager()->GetResource("Box04")->GetBoundingBox()->GetDimension();
  //l_vBoxDim5 = CORE->GetRenderableObjectsManager()->GetResource("Box05")->GetBoundingBox()->GetDimension();
  //l_vBoxDim6 = CORE->GetRenderableObjectsManager()->GetResource("Box06")->GetBoundingBox()->GetDimension();
  //l_vBoxDim7 = CORE->GetRenderableObjectsManager()->GetResource("Box07")->GetBoundingBox()->GetDimension();

  //

  //g_pBoxes = new CPhysicUserData("Boxes Fisiques");
  //g_pBoxes->SetPaint(false);
  //g_pBoxes->SetColor(colWHITE);
  //g_Box02 = new CPhysicActor(g_pBoxes);
  //g_Box03 = new CPhysicActor(g_pBoxes);
  //g_Box04 = new CPhysicActor(g_pBoxes);
  //g_Box05 = new CPhysicActor(g_pBoxes);
  //g_Box06 = new CPhysicActor(g_pBoxes);
  //g_Box07 = new CPhysicActor(g_pBoxes);


  //g_Box02->AddBoxSphape(l_vBoxDim2/2);
  //g_Box03->AddBoxSphape(l_vBoxDim3/2);
  //g_Box04->AddBoxSphape(l_vBoxDim4/2);
  //g_Box05->AddBoxSphape(l_vBoxDim5/2);
  //g_Box06->AddBoxSphape(l_vBoxDim6/2);
  //g_Box07->AddBoxSphape(l_vBoxDim7/2);

  ///*l_Box02->SetMat44(CORE->GetRenderableObjectsManager()->GetResource("Box02")->GetMat44());
  //l_Box03->SetMat44(CORE->GetRenderableObjectsManager()->GetResource("Box03")->GetMat44());
  //l_Box04->SetMat44(CORE->GetRenderableObjectsManager()->GetResource("Box04")->GetMat44());
  //l_Box05->SetMat44(CORE->GetRenderableObjectsManager()->GetResource("Box05")->GetMat44());
  //l_Box06->SetMat44(CORE->GetRenderableObjectsManager()->GetResource("Box06")->GetMat44());
  //l_Box07->SetMat44(CORE->GetRenderableObjectsManager()->GetResource("Box07")->GetMat44());*/
  //g_Box02->SetGlobalPosition(CORE->GetRenderableObjectsManager()->GetResource("Box02")->GetPosition());
  //g_Box03->SetGlobalPosition(CORE->GetRenderableObjectsManager()->GetResource("Box03")->GetPosition());
  //g_Box04->SetGlobalPosition(CORE->GetRenderableObjectsManager()->GetResource("Box04")->GetPosition());
  //g_Box05->SetGlobalPosition(CORE->GetRenderableObjectsManager()->GetResource("Box05")->GetPosition());
  //g_Box06->SetGlobalPosition(CORE->GetRenderableObjectsManager()->GetResource("Box06")->GetPosition());
  //g_Box07->SetGlobalPosition(CORE->GetRenderableObjectsManager()->GetResource("Box07")->GetPosition());

  //g_Box02->CreateBody(1.1f);
  //g_Box03->CreateBody(1.1f);
  //g_Box04->CreateBody(1.1f);
  //g_Box05->CreateBody(1.1f);
  //g_Box06->CreateBody(1.1f);
  //g_Box07->CreateBody(1.1f);

  //g_pJoint = new CPhysicFixedJoint();
  //g_pJoint2 = new CPhysicFixedJoint();
  //g_pJoint3 = new CPhysicFixedJoint();
  //g_pJoint4 = new CPhysicFixedJoint();
  //g_pJoint5 = new CPhysicFixedJoint();
  ////CPhysicFixedJoint* l_pJointP = new CPhysicFixedJoint();

  ////l_pJointP->SetInfo(g_Box02,g_pPActorBall);


  //g_pJoint->SetInfo(g_Box02,g_Box03);
  //g_pJoint2->SetInfo(g_Box03,g_Box04);
  //g_pJoint3->SetInfo(g_Box04,g_Box05);
  //g_pJoint4->SetInfo(g_Box05,g_Box06);
  //g_pJoint5->SetInfo(g_Box06,g_Box07);


  //l_pPhysManager->AddPhysicActor(g_Box02);
  //l_pPhysManager->AddPhysicActor(g_Box03);
  //l_pPhysManager->AddPhysicActor(g_Box04);
  //l_pPhysManager->AddPhysicActor(g_Box05);
  //l_pPhysManager->AddPhysicActor(g_Box06);
  //l_pPhysManager->AddPhysicActor(g_Box07);

  //l_pPhysManager->AddPhysicFixedJoint(g_pJoint);
  //l_pPhysManager->AddPhysicFixedJoint(g_pJoint2);
  //l_pPhysManager->AddPhysicFixedJoint(g_pJoint3);
  //l_pPhysManager->AddPhysicFixedJoint(g_pJoint4);
  //l_pPhysManager->AddPhysicFixedJoint(g_pJoint5);
  ////l_pPhysManager->AddPhysicFixedJoint(l_pJointP);

  //



  //g_pGameBox02 = new CGameObject("Box02");
  //g_pGameBox03 = new CGameObject("Box03");
  //g_pGameBox04 = new CGameObject("Box04");
  //g_pGameBox05 = new CGameObject("Box05");
  //g_pGameBox06 = new CGameObject("Box06");
  //g_pGameBox07 = new CGameObject("Box07");

  //g_pGameBox02->Init(CORE->GetRenderableObjectsManager()->GetResource("Box02"),g_Box02);
  //g_pGameBox03->Init(CORE->GetRenderableObjectsManager()->GetResource("Box03"),g_Box03);
  //g_pGameBox04->Init(CORE->GetRenderableObjectsManager()->GetResource("Box04"),g_Box04);
  //g_pGameBox05->Init(CORE->GetRenderableObjectsManager()->GetResource("Box05"),g_Box05);
  //g_pGameBox06->Init(CORE->GetRenderableObjectsManager()->GetResource("Box06"),g_Box06);
  //g_pGameBox07->Init(CORE->GetRenderableObjectsManager()->GetResource("Box07"),g_Box07);

  

  /*CHECKED_DELETE(l_pJoint)
  CHECKED_DELETE(l_pJoint2);
  CHECKED_DELETE(l_pJoint3);
  CHECKED_DELETE(l_pJoint4);
  CHECKED_DELETE(l_pJoint5);
  CHECKED_DELETE(l_pBoxes);*/
  
  //CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();
  
  //m_pRenderPhysX->SetVisible(true);
  

  //CORE->GetRenderableObjectsManager()->SetAllVisibility(false);

  
  g_pObjectManager = new CGameObjectManager();
  g_pObjectManager->Load("Data/XML/GameObjects.xml",false);

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
  CHECKED_DELETE(g_pUserData3)
  CHECKED_DELETE(g_pUserData4)
  CHECKED_DELETE(g_pUserData5)
  CHECKED_DELETE(g_pPActorBall)
  CHECKED_DELETE(g_pPActorComposite)
  CHECKED_DELETE(g_pGameObject)
  CHECKED_DELETE(g_pPhysXController)
  CHECKED_DELETE(g_pPEscala)
  CHECKED_DELETE(g_pUserDataEscala)
  CHECKED_DELETE(g_pUserDataController)
  CHECKED_DELETE(g_pGameBox02)
  CHECKED_DELETE(g_pGameBox03)
  CHECKED_DELETE(g_pGameBox04)
  CHECKED_DELETE(g_pGameBox05)
  CHECKED_DELETE(g_pGameBox06)
  CHECKED_DELETE(g_pGameBox07)

  CHECKED_DELETE(g_Box02)
  CHECKED_DELETE(g_Box03)
  CHECKED_DELETE(g_Box04)
  CHECKED_DELETE(g_Box05)
  CHECKED_DELETE(g_Box06)
  CHECKED_DELETE(g_Box07)


  CHECKED_DELETE(g_pJoint)
  CHECKED_DELETE(g_pJoint2)
  CHECKED_DELETE(g_pJoint3)
  CHECKED_DELETE(g_pJoint4)
  CHECKED_DELETE(g_pJoint5)
  CHECKED_DELETE(g_pBoxes)  
  CHECKED_DELETE(g_pObjectManager);
  //CHECKED_DELETE(plaXung)

  //CHECKED_DELETE(m_pRenderPhysX);
  

  
  
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
    //l_fYaw = l_fYaw+FLOAT_PI_VALUE/2;
    //l_fRoll = m_pObjectBot->GetRoll();

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

  /*g_pGameBox02->Update(_fElapsedTime);
  g_pGameBox03->Update(_fElapsedTime);
  g_pGameBox04->Update(_fElapsedTime);
  g_pGameBox05->Update(_fElapsedTime);
  g_pGameBox06->Update(_fElapsedTime);
  g_pGameBox07->Update(_fElapsedTime);*/



  g_pObjectManager->Update(_fElapsedTime);

}

void CPhysXProcess::RenderScene(CRenderManager* _pRM)
{

   CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
   l_pPhysManager->DebugRender(_pRM);



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

  

}

void CPhysXProcess::RenderINFO(CRenderManager* _pRM)
{

  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
 
  int l_iPosicio = 0;
  int l_iPosicio2 = 130;
  string l_szMsg("Sense Objecte");
  //stringstream l_SStream;
  stringstream l_SStreamHelp;


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
 
  _pRM->DrawAxis();
}

bool CPhysXProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
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
    g_pPhysXController->Move(Vect3f(l_vDir.x,0.0f,l_vDir.z)*0.05f*m_fVelocity,_fDeltaSeconds);
    m_pObject->SetPosition(g_pPhysXController->GetPosition());
   

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
    g_pPhysXController->Move(Vect3f(-l_vDir.x,0.0f,-l_vDir.z)*0.05f*m_fVelocity,_fDeltaSeconds);
    m_pObject->SetPosition(g_pPhysXController->GetPosition());

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

     /* g_Box02->CreateBody(0.1f);
      g_Box03->CreateBody(0.1f);
      g_Box04->CreateBody(0.1f);
      g_Box05->CreateBody(0.1f);
      g_Box06->CreateBody(0.1f);
      g_Box07->CreateBody(0.1f);*/
      
     
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


  



