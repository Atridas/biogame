#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysXProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "StaticMesh.h"
#include "StaticMeshManager.h"
#include "RenderableObjectsManager.h"
#include "AnimatedModelManager.h"
#include "ParticleManager.h"

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
#include "ActionManager.h"
#include "PhysxGrenade.h"
#include "ShoulderCamera.h"

#include "SpotLight.h"
#include "Camera.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include <cal3d/cal3d.h>
//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
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
#define ALTURA_TOTAL ((ALTURA_CONTROLLER+2*RADIUS_CONTROLLER)*0.5f + 0.01f)

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

//RAGDOLLS
CPhysxSkeleton* g_pRagdoll = 0;
float g_fRagdollInitTime = 0.0f;
float g_fRagdollAnimationDuration = 0.0f;
bool g_bRagdollInit = false;
unsigned int g_fRagdollFrames = 0;

CPhysxGrenade* g_pGrenade = 0;
vector<CPhysxGrenade*> g_vGrenadesVector;

//CAnimatedModelManager::CAnimatedModelManager()
//{
//  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
//}

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


  /*m_pObjectCamera = new CShoulderCamera(
      0.1f,
      100.0f,
      55.0f * FLOAT_PI_VALUE/180.0f,
      ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
      m_pObject,
      2.0f,0.6f,1.5f);*/
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

  //CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");

  /*m_pSpotlight = CORE->GetLightManager()->CreateSpotLight("FreeModeLight",
	                                                          Vect3f(-2.15715f,0.0f,-7.32758f),
	                                                          Vect3f(-5.4188f,0.0f,3.75613f),
	                                                          CColor(Vect3f(1.0f,1.0f,1.0f)),
	                                                          20.0f,
	                                                          80.0f,
	                                                          10.0f,
	                                                          45.0f,
	                                                          false );
	
	 m_pSpotlight->SetActive(false);*/
 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////CODI PER MOSTRAR LES ESFERES ALS BONES
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("ariggle");
  //CalCoreSkeleton* l_pSkeleton = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetCoreModel()->getCoreSkeleton();
  //vector<CalCoreBone*> l_vBones = l_pSkeleton->getVectorCoreBone();
  //CalVector l_vBonePos;

  //for (size_t i=0;i<l_vBones.size();++i)
  //{
  //  //l_vBones[0]->getBoundingData(plane,l_vBonePos);
  //  l_vBonePos = l_vBones[i]->getTranslationAbsolute();
  //  Vect3f l_vVector = Vect3f(l_vBonePos.x,l_vBonePos.y,l_vBonePos.z);
  //  SCollisionInfo l_cInfo;
  //  l_cInfo.m_CollisionPoint = l_vVector;
  //  l_cInfo.m_Normal = v3fZERO;
  //  g_vCollisions.push_back(l_cInfo);
  //}
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  g_pUserDataJoint1 = new CPhysicUserData("Objecte Joint 1");
  g_pUserDataJoint2 = new CPhysicUserData("Objecte Joint 2");
  g_pUserDataJoint3 = new CPhysicUserData("Objecte Joint 3");
  g_pUserDataJoint1->SetPaint(true);
  g_pUserDataJoint1->SetColor(colYELLOW);
  g_pUserDataJoint2->SetPaint(true);
  g_pUserDataJoint2->SetColor(colRED);
  g_pUserDataJoint3->SetPaint(true);
  g_pUserDataJoint3->SetColor(colGREEN);

  g_pUserData = new CPhysicUserData("Plane");
  g_pUserData->SetPaint(true);
  g_pUserData->SetColor(colWHITE);


  g_pJointActor1 = new CPhysicActor(g_pUserDataJoint1);
  g_pJointActor2 = new CPhysicActor(g_pUserDataJoint2);
  g_pJointActor3 = new CPhysicActor(g_pUserDataJoint3);


  g_pJointActor1->AddBoxSphape(Vect3f(0.5f,0.5f,0.5f),v3fZERO,NULL,GROUP_COLLIDABLE_PUSHABLE);
  g_pJointActor2->AddSphereShape(0.4f,v3fZERO,NULL,GROUP_COLLIDABLE_PUSHABLE);
  g_pJointActor3->AddBoxSphape(Vect3f(0.3f,0.3f,0.3f),v3fZERO,NULL,GROUP_COLLIDABLE_PUSHABLE);
  g_pJointActor1->SetGlobalPosition(Vect3f(-9.0f,1.0f,-3.0f));
  g_pJointActor2->SetGlobalPosition(Vect3f(-9.0f,3.0f,-3.0f));
  g_pJointActor3->SetGlobalPosition(Vect3f(-9.0f,5.0f,-3.0f));

  g_pJointActor1->CreateBody(1.0f);
  g_pJointActor2->CreateBody(5.0f);
  g_pJointActor3->CreateBody(10.0f);
  
  g_pFixedJoint = new CPhysicFixedJoint();
  g_pFixedJoint2 = new CPhysicFixedJoint();
  g_pRevoluteJoint = new CPhysicRevoluteJoint();
  g_pSphericalJoint = new CPhysicSphericalJoint();
  g_pSphericalJoint2 = new CPhysicSphericalJoint();
  //g_pSphericalJoint->GetPhXDescJoint()->

  l_pPhysManager->AddPhysicActor(g_pJointActor1);
  l_pPhysManager->AddPhysicActor(g_pJointActor2);
  l_pPhysManager->AddPhysicActor(g_pJointActor3);

  g_pJointActor3->GetPhXActor()->raiseBodyFlag(NX_BF_KINEMATIC);
  g_pJointActor3->GetPhXActor()->setLinearDamping(0.5);

  //g_pFixedJoint->SetInfo(g_pJointActor1,g_pJointActor2);
  g_pSphericalJoint->SetInfoComplete(Vect3f(-9.0f,5.0f,-3.0f),Vect3f(0.0f,-1.0f,0.0f),g_pJointActor2,g_pJointActor3);
  g_pSphericalJoint2->SetInfoComplete(Vect3f(-9.0f,3.0f,-3.0f),Vect3f(0.0f,-1.0f,0.0f),g_pJointActor1,g_pJointActor2);
  //g_pSphericalJoint2->SetInfo(Vect3f(-9.0f,2.0f,-3.0f),g_pJointActor1,g_pJointActor2);
  //g_pSphericalJoint->SetInfo(Vect3f(-9.0f,3.0f,-3.0f),g_pJointActor2,g_pJointActor3);
  
  
  
  //g_pFixedJoint2->SetInfo(g_pJointActor2,g_pJointActor3);
  //g_pRevoluteJoint->SetInfo(v3fZERO,Vect3f(0.1f,0.1f,0.1f),g_pJointActor1,g_pJointActor2);
  //g_pSphericalJoint->SetInfo(Vect3f(-6.0f,2.0f,-3.0f),g_pJointActor2);
  //g_pRevoluteJoint->SetMotor(1.0f,1.0f);

  //g_pFixedJoint->CreateJoint(NULL);

  //l_pPhysManager->AddPhysicFixedJoint(g_pFixedJoint);
  //l_pPhysManager->AddPhysicFixedJoint(g_pFixedJoint2);
  //l_pPhysManager->AddPhysicRevoluteJoint(g_pRevoluteJoint);
  l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint);
  l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint2);




  g_pObjectManager = new CGameObjectManager();
  g_pObjectManager->Load("Data/Levels/NivellProves/XML/GameObjects.xml",false);

  //CPhysicActor* l_pLampada = g_pObjectManager->GetResource("Lampada")->GetPhysXActor();
  //CPhysicActor* l_pCable = g_pObjectManager->GetResource("Cable_Lampada")->GetPhysXActor();
  //Vect3f l_vMax = g_pObjectManager->GetResource("Cable_Lampada")->GetRenderableObject()->GetBoundingBox()->GetDimension();
  //Vect3f l_vPos = g_pObjectManager->GetResource("Lampada")->GetRenderableObject()->GetPosition();
  //Vect3f l_vPos2 = g_pObjectManager->GetResource("Cable_Lampada")->GetRenderableObject()->GetPosition();
  //Vect3f l_vPos3 = l_vPos2;

  //l_vPos2.y = l_vPos2.y - (l_vMax.y*0.5f); 
  //l_vPos3.y = l_vPos3.y + (l_vMax.y*0.5f); 


  //SCollisionInfo l_SInfo;
  //l_SInfo.m_CollisionPoint = l_vPos2;
  //l_SInfo.m_Normal = Vect3f(0.0f,-1.0f,0.0f);
  //g_vCollisions.push_back(l_SInfo);

  //l_SInfo.m_CollisionPoint = l_vPos3;
  //g_vCollisions.push_back(l_SInfo);


  //g_pSphericalJoint->SetInfo(l_vPos2,l_pLampada,l_pCable);
  //g_pSphericalJoint2->SetInfo(l_vPos3,l_pCable);

  //l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint);
  //l_pPhysManager->AddPhysicSphericalJoint(g_pSphericalJoint2);
 
  
 


  //CHARACTER CONTROLLER
  g_pUserDataController = new CPhysicUserData("PhysX Controller");
  g_pUserDataController->SetPaint(true);
  g_pUserDataController->SetColor(colBLACK);

  g_pPhysXController = new CPhysicController(RADIUS_CONTROLLER,ALTURA_CONTROLLER,10.0,0.05f,0.01f,COLISIONABLE_MASK,g_pUserDataController,Vect3f(-7.0f,2.2f,-4.0f));
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

  
  /*if(l_Spot)
  {
    l_Spot->SetDirection(l_ControllerPos);
    l_Spot->SetActive(true);
  }*/
  m_bRenderLights = false;


  CORE->GetLightManager()->SetLightsEnabled(true);
  l_pPhysManager->SetDebugRenderMode(true);

  
  /////////////////////////////////////////////////////////////////////////////////
  //// RAGDOLL PROVES
  /////////////////////////////////////////////////////////////////////////////////

  //CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("rigglebot");
  //CalSkeleton* l_pSkeleton = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
  //l_pSkeleton->getCoreSkeleton()->calculateBoundingBoxes(l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getCoreModel());
  //l_pSkeleton->calculateBoundingBoxes();
  //if (g_pRagdoll == 0)
  //{
  //  g_pRagdoll = new CPhysxSkeleton();
  //  CalModel* l_pCalModel = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  //  g_pRagdoll->Init("Data/Animated Models/Riggle/Skeleton.xml",l_pCalModel,l_pAnim->GetMat44());
  //}

  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////


  CORE->GetParticleManager()->SetAllEmittersActive(false);


  SetOk(true);
  return IsOk();




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

  CHECKED_DELETE(g_pRagdoll);
  CHECKED_DELETE(g_pGrenade);

  for (size_t i=0;i<g_vGrenadesVector.size();++i)
  {
    CHECKED_DELETE(g_vGrenadesVector[i]);
  }

  g_vGrenadesVector.clear();
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


    if (m_pSpotlight)
    {
      m_pSpotlight->SetPosition(m_pObject->GetPosition());
      m_pSpotlight->SetDirection(m_pCamera->GetDirection());
    
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


  /*Mat44f l_vMat = g_pCharacter->GetMat44();
  Vect3f l_vVect();*/
  m_pObject->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y+0.5f,l_ControllerPos.z-1.0f));

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
       /* g_pCharacter->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(0);
        CalCoreAnimation* l_pAnimation;*/
	    }
    }
  }
  else
  /*if ((INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_W)) ||
      (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_S)) ||
      (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_A)) ||
      (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_D)))*/
  {
    if(g_pCharacter)
    {
      //if (CORE->GetActionManager()->IsActionActive("Run"))
      if (INPUT_MANAGER->IsDown(IDV_KEYBOARD,KEY_LSHIFT))
      {
	      if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 6)
	      {
		      g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
          g_pCharacter->GetAnimatedInstanceModel()->BlendCycle("run",0.3f);
	      }
      }
      else
      {
        if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	      {
          if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 5)
          {
		        g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
            g_pCharacter->GetAnimatedInstanceModel()->BlendCycle("walk",0.3f);
          }
	      }

        
      }
    }

      
  }

  CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("rigglebot");
  CalModel* l_pCalModel = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel();


  //RAGDOLLS
  if (g_pRagdoll != 0)
  {
    g_pRagdoll->Update();
    l_pAnim->SetMat44(g_pRagdoll->GetTransform());
    //CalSkeleton* l_pSkeleton = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
    //l_pSkeleton->calculateState();
    
  }

  //GRANADA
  for (size_t i=0;i<g_vGrenadesVector.size();++i)
  {
    g_vGrenadesVector[i]->Update(_fElapsedTime);
  }

  CORE->GetParticleManager()->Update(_fElapsedTime,m_pCamera);

}

Mat44f CPhysXProcess::GetBoneLeftHandedAbsoluteTransformation(CalBone* _pBone)
{
  //rotacio i translacio del bone (absoluta)
  CalVector l_vTranslation = _pBone->getTranslationAbsolute();
  CalQuaternion l_RotationQuaternion = _pBone->getRotationAbsolute();

  //passem el quaternion a left handed
  l_RotationQuaternion.x = -l_RotationQuaternion.x;
  l_RotationQuaternion.y = -l_RotationQuaternion.y;
  l_RotationQuaternion.z = -l_RotationQuaternion.z;

  //creem la matriu de transformacio Cal3d (absolute) -> Mat44f
  CalMatrix l_RotationMatrix(l_RotationQuaternion);

  Mat33f l_Rotation = Mat33f( l_RotationMatrix.dxdx   ,l_RotationMatrix.dydx  ,l_RotationMatrix.dzdx,
                              l_RotationMatrix.dxdy   ,l_RotationMatrix.dydy  ,l_RotationMatrix.dzdy,
                              l_RotationMatrix.dxdz   ,l_RotationMatrix.dydz  ,l_RotationMatrix.dzdz);

  float l_fAngleX = FLOAT_PI_VALUE - l_Rotation.GetAngleX();
  float l_fAngleY = FLOAT_PI_VALUE - l_Rotation.GetAngleY();
  float l_fAngleZ = FLOAT_PI_VALUE - l_Rotation.GetAngleZ();

  Mat44f l_Transform;
  l_Transform.SetIdentity();
  l_Transform.RotByAnglesYXZ(l_fAngleY,l_fAngleX,l_fAngleZ);
  l_Transform.Translate(Vect3f(-l_vTranslation.x,l_vTranslation.y,l_vTranslation.z));

  return l_Transform;
  
}

void CPhysXProcess::ExportSkeletonInfo(CalSkeleton* _pSkeleton)
{
  CXMLTreeNode l_XMLDoc;
  CBoundingBox l_Box;
  Vect3f l_vPoints[8];
  CalVector l_vCalPoints[8];

  _pSkeleton->calculateBoundingBoxes();

  vector<CalBone*> l_vBones = _pSkeleton->getVectorBone();
  vector<CalBone*>::iterator l_itBone = l_vBones.begin();

  l_XMLDoc.StartNewFile("Skeleton.xml");
  l_XMLDoc.StartElement("skeleton");

  while(l_itBone != l_vBones.end())
  {
    CalBone* l_pBone = *l_itBone;
    CalCoreBone* l_pCoreBone = l_pBone->getCoreBone();

    l_pCoreBone->getBoundingBox().computePoints(l_vCalPoints);

    //rotacio i translacio del core bone (absoluta)
    CalVector l_vCoreBoneTranslation = l_pCoreBone->getTranslationAbsolute();
    CalQuaternion l_CoreBoneRotationQuaternion = l_pCoreBone->getRotationAbsolute();

    //rotem el core bone  (absoluta)
    CalQuaternion x_axis_90(-0.7071067811f,0.0f,0.0f,0.7071067811f);
    l_CoreBoneRotationQuaternion *= x_axis_90;
    l_vCoreBoneTranslation *= x_axis_90;

    for(int i = 0; i < 8; ++i)
    {
      //calculem les coordenades del vertex de la box relatives al bone
      CalVector l_vRelativeCoords = l_vCalPoints[i] - l_vCoreBoneTranslation;
      CalQuaternion l_CoreBoneRotationQuaternionInv = l_CoreBoneRotationQuaternion;
      l_CoreBoneRotationQuaternionInv.invert();
      l_vRelativeCoords *= l_CoreBoneRotationQuaternionInv;

      l_vPoints[i] = Vect3f(-l_vRelativeCoords.x,l_vRelativeCoords.y,l_vRelativeCoords.z);
    }

    l_Box.Init(l_vPoints);

    l_XMLDoc.StartElement("bone");

    l_XMLDoc.WritePszProperty("name",l_pCoreBone->getName().c_str());
    l_XMLDoc.WritePszProperty("type","box");
    l_XMLDoc.WriteFloatProperty("density",0.0f);
    l_XMLDoc.WriteVect3fProperty("bounding_box_size",l_Box.GetDimension());
    l_XMLDoc.WriteVect3fProperty("bounding_box_middle_point",l_Box.GetMiddlePoint());

    l_XMLDoc.EndElement();

    ++l_itBone;
  }

  l_XMLDoc.EndElement();

  l_XMLDoc.EndNewFile();
}

void CPhysXProcess::RenderScene(CRenderManager* _pRM)
{  
  CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
  l_pPhysManager->DebugRender(_pRM);

  //CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("ariggle");
  //CalSkeleton* l_pSkeleton = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();

  //l_pSkeleton->calculateBoundingBoxes();

  //vector<CalBone*> l_vBones = l_pSkeleton->getVectorBone();

  //g_vCollisions.clear();

  //for (size_t j=0;j<l_vBones.size();++j)
  //{
  //  CalBone* l_pBone = l_vBones[j];
  //  CalCoreBone* l_pCoreBone = l_pBone->getCoreBone();

  //  Mat44f l_mWorld = GetBoneLeftHandedAbsoluteTransformation(l_pBone);

  //  _pRM->SetTransform(/*g_pCharacter->GetMat44() */ l_mWorld);

  //  /***************CORE BOUNDING BOX****************/

  //  //agafem els punts de la bounding de la core
  //  CalVector l_vPoints[8];
  //  l_pCoreBone->getBoundingBox().computePoints(l_vPoints);

  //  CBoundingBox l_Box;
  //  Vect3f l_vect[8];

  //  //rotacio i translacio del core bone (absoluta)
  //  CalVector l_TranslationC=l_pCoreBone->getTranslationAbsolute();
  //  CalQuaternion l_QuatC=l_pCoreBone->getRotationAbsolute();

  //  //rotem el core bone  (absoluta)
  //  CalQuaternion x_axis_90(-0.7071067811f,0.0f,0.0f,0.7071067811f);
  //  l_QuatC *= x_axis_90;
  //  l_TranslationC *= x_axis_90;

  //  for(int j=0;j<8;++j)
  //  {
  //    //calculem les coordenades del vertex de la box relatives al bone
  //    CalVector l_PosBB=(l_vPoints[j]-l_TranslationC);
  //    CalQuaternion l_Quat2=l_QuatC;
  //    l_Quat2.invert();
  //    l_PosBB*=l_Quat2;

  //    l_vect[j] = Vect3f(-l_PosBB.x,l_PosBB.y,l_PosBB.z);
  //  }

  //  //creem la box
  //  l_Box.Init(l_vect);

  //  /************************************************/

  //  //_pRM->RenderBoundingBox(&l_Box);

  //  //renderitzem els joints
  //  _pRM->DrawSphere(0.01f,colYELLOW,5);
  //}

  CORE->GetRenderableObjectsManager()->Render(_pRM);
  _pRM->DrawGrid(0.1f,colCYAN,1,1);
  CORE->GetParticleManager()->Render(_pRM);

  if(m_bRenderLights)
    CORE->GetLightManager()->Render(_pRM);

  _pRM->SetTransform(Mat44f().SetIdentity());
  RenderLaserPoint(_pRM);
  //_pRM->DrawAxis();
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


 
  //_pRM->DrawAxis();
  
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
      /*if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }*/
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
      /*if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }*/
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
      /*if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }*/
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
      /*if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	    }*/
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }


  if(strcmp(_pcAction, "Elevate") == 0)
  {
    
    ///////////////////////////////////////////////////////////////////////////////
    // RAGDOLL PROVES
    ///////////////////////////////////////////////////////////////////////////////

    CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("rigglebot");
    CalSkeleton* l_pSkeleton = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
    l_pSkeleton->getCoreSkeleton()->calculateBoundingBoxes(l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getCoreModel());
    l_pSkeleton->calculateBoundingBoxes();
    if (g_pRagdoll == 0)
    {
      g_pRagdoll = new CPhysxSkeleton(false);
      CalModel* l_pCalModel = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel();
      g_pRagdoll->Init("Data/Animated Models/Riggle/Skeleton.xml",l_pCalModel,l_pAnim->GetMat44());
    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

  }

  if(strcmp(_pcAction, "ShootBall") == 0)
  {
      if (g_pCharacter)
      {
        g_pCharacter->GetAnimatedInstanceModel()->ExecuteAction(3,0.0f);
      }

      
      
      //g_pPActorComposite->SetLinearVelocity(Vect3f(0.0f,1.0f,0.0f)*m_fPhysxVelocity);
      CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
      const Vect3f l_PosCamera = m_pCamera->GetEye();
      const Vect3f& l_DirCamera = m_pCamera->GetDirection().Normalize();

       CalSkeleton* l_pSkeleton = g_pCharacter->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
      int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Hand");
      CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
      CalVector l_vPos = l_pBone->getTranslationAbsolute();
      Mat44f l_vMat44 = g_pCharacter->GetMat44();
      Vect3f l_vVect(-l_vPos.x, l_vPos.y, l_vPos.z);
      l_vVect = l_vMat44*l_vVect;
  
      SCollisionInfo l_CInfo;
      if (g_pUserDataSHOOT != 0)
      {
        g_pUserDataSHOOT->SetColor(colWHITE);
      }
      
      g_pUserDataSHOOT = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,4,g_pUserDataSHOOT,l_CInfo);
      Vect3f l_vDirection(l_CInfo.m_CollisionPoint.x - l_vVect.x, l_CInfo.m_CollisionPoint.y - l_vVect.y, l_CInfo.m_CollisionPoint.z - l_vVect.z);
      l_vDirection.Normalize();


      if (g_pUserDataSHOOT != 0)
      {
        if (g_pRagdoll != 0)
        {
          if (g_pRagdoll->IsRagdollPhysXActor(g_pUserDataSHOOT->GetName()))
          {
            if (!g_pRagdoll->IsRagdollActive())
            {
              g_pRagdoll->SetRagdollActive(true);
            }
          }

        }
        g_pUserDataSHOOT->SetColor(colRED);
      }


      g_pUserDataSHOOT = l_pPhysManager->RaycastClosestActorShoot(l_vVect,l_vDirection,4,g_pUserDataSHOOT,l_CInfo, 20.0f);
      g_vCollisions.push_back(l_CInfo);

      

  }

  if(strcmp(_pcAction, "ShootBOX") == 0)
  {
    /*CPhysicActor* l_pPActorShoot = new CPhysicActor(g_pUserData);
    l_pPActorShoot->AddBoxSphape(0.2f,m_pCamera->GetEye());
    l_pPActorShoot->CreateBody(3);
    CORE->GetPhysicsManager()->AddPhysicActor(l_pPActorShoot);
    l_pPActorShoot->SetLinearVelocity(m_pCamera->GetDirection()*m_fPhysxVelocity);
    CHECKED_DELETE(l_pPActorShoot)*/
  
    ///////////////////////////////////////////////////////////////////////////////
    // RAGDOLL PROVES
    ///////////////////////////////////////////////////////////////////////////////
  
    //CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetResource("rigglebot");
    //CalSkeleton* l_pSkeleton = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
    //l_pSkeleton->getCoreSkeleton()->calculateBoundingBoxes(l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getCoreModel());
    //l_pSkeleton->calculateBoundingBoxes();
  
    //if (g_pRagdoll == 0)
    //{
  
    //  int l_iId = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimationId("dead");
    //  g_fRagdollFrames = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(l_iId)->getTotalNumberOfKeyframes();
    //  g_fRagdollAnimationDuration = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(l_iId)->getDuration();
    //  //g_fRagdollAnimationDuration = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(l_iId)->
  
    //  l_pAnim->GetAnimatedInstanceModel()->ClearCycle(0.5f);
    //  l_pAnim->GetAnimatedInstanceModel()->BlendCycle("dead",0.5f);
    //  g_pRagdoll = new CPhysxSkeleton();
    //  //RENDER_MANAGER->SetTransform(l_pAnim->GetMat44());
    //  CalModel* l_pCalModel = l_pAnim->GetAnimatedInstanceModel()->GetAnimatedCalModel();
    //  
    //  
    //  //l_pAnim->SetVisible(false);
    //  
  
    // /* g_pRagdoll = new CPhysxRagdoll("Ragdoll Prova");
    //  g_pRagdoll->Load("Data/Animated Models/Riggle/Ragdoll.xml",false);
    //  g_pRagdoll->InitSkeleton(l_pSkeleton);*/
  
    //}

    if (g_pRagdoll != 0)
    {
      if(!g_pRagdoll->IsRagdollActive())
      {
        g_pRagdoll->SetRagdollActive(true);
      }else{
        Mat44f l_mTransform = g_pRagdoll->GetTransform();

        Mat44f l_mNewTransform;
        Mat44f l_mNewTransform2;
        l_mNewTransform.SetIdentity();
        l_mNewTransform2.SetIdentity();
        l_mNewTransform.RotByAngleY(l_mTransform.GetYaw());
        l_mNewTransform2.Translate(Vect3f(l_mTransform.GetTranslationVector().x,0.0f,l_mTransform.GetTranslationVector().z));

        g_pRagdoll->SetRagdollActive(false);

        g_pRagdoll->SetTransform(l_mNewTransform2*l_mNewTransform);
      }
    }

    if (g_pCharacter)
    {
      //g_pCharacter->SetPosition(Vect3f(l_ControllerPos.x,l_ControllerPos.y-ALTURA_TOTAL,l_ControllerPos.z));
      if(g_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 5)
	    {
		    g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(5,0.3f);
	    }
      else
      {
        g_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		    g_pCharacter->GetAnimatedInstanceModel()->BlendCycle(0,0.3f);
      }
    }


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
    //g_pCharacter->SetVisible(!g_pCharacter->GetVisible());
    //ExportSkeletonInfo(g_pCharacter->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton());
  }

  if(strcmp(_pcAction, "ShootGrenade") == 0)
  {
     if (g_pCharacter)
      {
        g_pCharacter->GetAnimatedInstanceModel()->ExecuteAction(3,0.0f);
      }
      
      //g_pPActorComposite->SetLinearVelocity(Vect3f(0.0f,1.0f,0.0f)*m_fPhysxVelocity);
      CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
      const Vect3f l_PosCamera = m_pCamera->GetEye();
      const Vect3f& l_DirCamera = m_pCamera->GetDirection().Normalize();

      CalSkeleton* l_pSkeleton = g_pCharacter->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
      int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Hand");
      CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
      CalVector l_vPos = l_pBone->getTranslationAbsolute();
      Mat44f l_vMat44 = g_pCharacter->GetMat44();
      //l_vMat44.Translate(Vect3f(0.1f,0.0f,0.0f));
      Vect3f l_vVect(-l_vPos.x, l_vPos.y, l_vPos.z);
      l_vVect = l_vMat44*l_vVect;
  
      SCollisionInfo l_CInfo;
      if (g_pUserDataSHOOT != 0)
      {
        g_pUserDataSHOOT->SetColor(colWHITE);
      }
      
      g_pUserDataSHOOT = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,6,g_pUserDataSHOOT,l_CInfo);
      Vect3f l_vDirection(l_CInfo.m_CollisionPoint.x - l_vVect.x, l_CInfo.m_CollisionPoint.y - l_vVect.y, l_CInfo.m_CollisionPoint.z - l_vVect.z);
      l_vDirection.Normalize();

      CPhysxGrenade* l_pGrenade = new CPhysxGrenade("Granada" + g_vGrenadesVector.size(),7.0f,3.0f,25.0f);
      if (g_pUserDataSHOOT != 0)
      {
        l_pGrenade->Init(0.1f,1.0f,GROUP_COLLIDABLE_PUSHABLE,l_vVect,l_vDirection,20.0f,"fum_proveta"); 
      }
      else
      {
        Vect3f l_vDirectionAir = l_DirCamera*10.0f;
        l_vDirectionAir = l_vDirectionAir-l_vVect;
        l_vDirectionAir.Normalize();
        l_pGrenade->Init(0.1f,1.0f,GROUP_COLLIDABLE_PUSHABLE,l_vVect,l_vDirectionAir,20.0f,"fum_proveta");
      }
      g_vGrenadesVector.push_back(l_pGrenade);

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

void CPhysXProcess::RenderImpacts(CRenderManager* _pRM)
{
  Mat44f t;  
  for (int i=0;i<(int)g_vCollisions.size();++i)
  {

      t.SetIdentity();
      t.Translate(g_vCollisions[i].m_CollisionPoint);
      _pRM->SetTransform(t);
      if (i==8)
      {
        _pRM->DrawSphere(0.015f,colRED,5);
      }
      else
      {
        _pRM->DrawSphere(0.01f,colYELLOW,5);
      }
      _pRM->DrawLine(v3fZERO,g_vCollisions[i].m_Normal*0.5f,colGREEN);
  }


}

void CPhysXProcess::RenderLaserPoint(CRenderManager* _pRM)
{
  CalSkeleton* l_pSkeleton = g_pCharacter->GetAnimatedInstanceModel()->GetAnimatedCalModel()->getSkeleton();
  l_pSkeleton->calculateBoundingBoxes();
  Vect3f l_vAnimPos = g_pCharacter->GetPosition();
  int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Hand");
  CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
  CalVector l_vPos = l_pBone->getTranslationAbsolute();
  Mat44f l_vMat44 = g_pCharacter->GetMat44();
  Vect3f l_vVect(-l_vPos.x, l_vPos.y, l_vPos.z);
  l_vVect = l_vMat44*l_vVect;


  CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();
  const Vect3f l_PosCamera = m_pCamera->GetEye();
  const Vect3f& l_DirCamera = m_pCamera->GetDirection().Normalize();
  
  SCollisionInfo l_CInfo;
  g_pUserDataSHOOT = l_pPhysManager->RaycastClosestActor(l_PosCamera,l_DirCamera,6,g_pUserDataSHOOT,l_CInfo);
  //vCollisions.push_back(l_CInfo);

  /*Mat44f t;
  t.SetIdentity();
  t.Translate(l_vAnimPos);
  _pRM->SetTransform(t);*/

  Vect3f l_vAnimPos2 = m_pObject->GetPosition();
  //Vect3f l_vDirectionAir(l_DirCamera.x+10.0f,l_DirCamera.y+10.0f,l_DirCamera.z+10.0f);
  Vect3f l_vDirectionAir = l_DirCamera*10.0f;
  //_pRM->DrawLine(Vect3f(l_vPos.x+l_vAnimPos.x,l_vPos.y+l_vAnimPos.y,l_vPos.z+l_vAnimPos.z),l_CInfo.m_CollisionPoint,colRED);
  if (g_pUserDataSHOOT != 0)
  {
    _pRM->DrawLine(l_CInfo.m_CollisionPoint,l_vVect,colRED);
  }
  else
  {
    _pRM->DrawLine(l_vVect+l_vDirectionAir,l_vVect,colRED);
  }

  Mat44f t;
  t.SetIdentity();
  t.Translate(l_CInfo.m_CollisionPoint);
  _pRM->SetTransform(t);
  _pRM->DrawSphere(0.1f,colRED,5);


}

  



