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
#include "ComponentObject3D.h"

#include <LightManager.h>
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "PhysicController.h"
#include "SpotLight.h"
#include "InstanceMesh.h"
#include "GameObject.h"
#include "ParticleManager.h"
#include "BillBoard.h"

#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRenderableObject.h"



bool CParticleProcess::Init()
{

  m_pBillBoard.SetPos( Vect3f(0.332061f, 2.5184f, 8.16266f));
  m_pBillBoard.SetSize( 1.79023f, 1.61947f);
  
  m_pObject = new CObject3D();
  m_fVelocity = 1;
  angle=0;
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
  
  if(l_Spot)
  {
    //l_Spot->SetDirection(m_pObjectBot->GetPosition());
    l_Spot->SetActive(true);
  }
  m_bRenderLights = false;

 /* // Creem la entitat del jugador ----------------------------------------------------------------

  m_pPlayerEntity = CORE->GetEntityManager()->CreateEntity();
  CORE->GetEntityManager()->SetName("Player", m_pPlayerEntity);

  CComponentObject3D *l_pComponentObject3D = new CComponentObject3D();
  l_pComponentObject3D->Init(m_pPlayerEntity);
  l_pComponentObject3D->SetPosition(Vect3f(3.1f,1.0f,-8.56f));
  

  (new CComponentMovement)->Init(m_pPlayerEntity);

  CComponentRenderableObject * l_pComponentRenderableObject = new CComponentRenderableObject();
  l_pComponentRenderableObject->InitAnimatedModel(m_pPlayerEntity, "Player Character", "riggle");
  l_pComponentRenderableObject->m_bBlockPitchRoll = true;
  l_pComponentRenderableObject->m_fHeightAdjustment = -1.f;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;

  CComponentPlayerController *l_pComponentPlayerController = new CComponentPlayerController();
  l_pComponentPlayerController->Init(m_pPlayerEntity,
                                      //Actions
                                     "MoveFwd",
                                     "MoveBack",
                                     "MoveLeft",
                                     "MoveRight",
                                     "Walk",
                                     "Run",
                                      //Animations
                                     "idle",
                                     "walk",
                                     "walk",
                                     "walk",
                                     "walk",
                                      //Speed
                                     4, 10, 1, 1,
                                      FLOAT_PI_VALUE/3,
                                     -FLOAT_PI_VALUE/3);

  CComponent3rdPSCamera *l_pComponent3rdPSCamera = new CComponent3rdPSCamera();
  //l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 0, 0);
  //((CThPSCamera*)l_pComponent3rdPSCamera->GetCamera())->SetZoom(0);
  l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 1, 0.5f);

  m_pCamera = l_pComponent3rdPSCamera->GetCamera();

  CComponentPhysXController *l_pComponentPhysXController = new CComponentPhysXController();
  l_pComponentPhysXController->Init(m_pPlayerEntity, 0.3f, 1.5f, 10.0f, 0.1f, 0.5f, 1);
  */

  SetOk(true);
  return IsOk();
}

void CParticleProcess::Release()
{
	CHECKED_DELETE(m_pCamera);
	CHECKED_DELETE(m_pObject);
}

void CParticleProcess::Update(float _fElapsedTime)
{
   if(m_pObject)// && m_pObjectBot) 
  {

    m_pBillBoard.Update(_fElapsedTime,m_pObjectCamera);
    //update del player
    //CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);

    CORE->GetParticleManager()->Update(_fElapsedTime,m_pObjectCamera);
    //Actualitze el pitch i el yaw segons els delta del mouse
    float l_fPitch, l_fYaw;

    Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

    l_fPitch = m_pObject->GetPitch();
    l_fYaw   = m_pObject->GetYaw();
  
    m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
    //m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);

    l_fPitch -= l_vVec.y*_fElapsedTime;
    if(l_fPitch < - FLOAT_PI_VALUE/3) l_fPitch = - FLOAT_PI_VALUE/3;
    if(l_fPitch >   FLOAT_PI_VALUE/3) l_fPitch =   FLOAT_PI_VALUE/3;
    m_pObject->SetPitch(l_fPitch);


    //m_pObjectBot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObjectBot->GetPosition().y, m_pObject->GetPosition().z));
  
    l_fPitch = m_pObject->GetPitch();
    //l_fPitch = l_fPitch+FLOAT_PI_VALUE/2;
    //l_fYaw = m_pObjectBot->GetYaw();
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
      //((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->BlendCycle(m_iState,0);

      m_bStateChanged = false;
    }
  }
}

void CParticleProcess::RenderScene(CRenderManager* _pRM)
{





  //*******************************************************
  
  //Render Objects
  // Ensenya tot el Hangar i els seus objectes
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  _pRM->DrawGrid(0.1f,colCYAN,1,1);
  //_pRM->DrawPlane(10,Vect3f(0,1,0),0,colBLUE,10,10);
  CORE->GetParticleManager()->Render(_pRM);
  CTexture*  texture = CORE->GetTextureManager()->GetResource("Data/Textures/texturas particulas/cartaAjuste.png");
  m_pBillBoard.Render(_pRM->GetDevice(), texture->GetD3DTexture() );
  
   
  
}

void CParticleProcess::RenderINFO(CRenderManager* _pRM)
{
  /*uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
 
  int l_iPosicio = 0;
  int l_iPosicio2 = 130;
  string l_szMsg("Sense Objecte");
  
  stringstream l_SStreamHelp;

 
  _pRM->DrawAxis();  */
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

