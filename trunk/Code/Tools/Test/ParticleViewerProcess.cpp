#include "ParticleViewerProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "RenderableObjectsManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"
#include "ThPSCamera.h"

#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRenderableObject.h"
#include "ComponentVida.h"
#include "ComponentRagdoll.h"
#include "ComponentMirilla.h"
#include "LightManager.h"
#include "SpotLight.h"
#include "Camera.h"
#include "ComponentStateMachine.h"
#include "ComponentAnimation.h"
#include "ComponentIABrain.h"
#include "IAManager.h"
#include "GraphDefines.h"
#include "EmiterInstance.h"
#include "CoreEmiterManager.h"
#include "EffectManager.h"
#include "EmiterManager.h"

#include "PhysicActor.h"
#include <PhysicsManager.h>

#include "PortalManager.h"

#include "SphereCamera.h"

bool CParticleViewerProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CParticleViewerProcess::Init");
  
  CORE->GetRenderManager()->SetClearColor(colWHITE);

  m_pSceneEffectManager = CORE->GetSceneEffectManager();
  CORE->GetLightManager()->SetLightsEnabled(true);

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();


  //Carregar entitats de l'escenari ----------------------------------------------------------------------------------------
  CEntityManager* l_pEM = CORE->GetEntityManager();
  CGameEntity* l_pFloor = l_pEM->CreateEntity();
  CComponentObject3D::AddToEntity(l_pFloor)->SetPosition(Vect3f( 0, -50, 0));
  CComponentPhysXBox::AddToEntity(l_pFloor, 100, 100, 100, 0, 0, 0, 0, ECG_ESCENARI);


  m_pPlayerEntity = l_pEM->InitPlayer("Player", Vect3f(-6,1,-3.6f), -12.3f);
  m_pCamera = m_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();

  /// -----------------------------------------------------------------------------------------------------------------------
  //for(int i = 0; i < NUM_EMISORS; ++i)
  //  m_pEmiters[i]  = new CEmiterInstance();

  CCoreEmiterManager* l_pCoreEmiterManager = CORE->GetCoreEmiterManager();
  const set<string>& l_szEmiterNames = l_pCoreEmiterManager->GetCoreNames();
  set<string>::const_iterator l_it = l_szEmiterNames.begin();
  if(l_it != l_szEmiterNames.end())
  {
    CEmiterManager* l_pEM = CORE->GetEmiterManager();
    //
    m_szEmiter = *l_it;
    m_pEmiters[0] = l_pEM->CreateEmiter("emiter 1", m_szEmiter, CObject3D(Vect3f(0, 0, 0),0,0), Vect3f(1, 1, 1));
    m_pEmiters[1] = l_pEM->CreateEmiter("emiter 2", m_szEmiter, CObject3D(Vect3f(5, 0, 0),0,0), Vect3f(.1f, .1f, .1f));
    m_pEmiters[2] = l_pEM->CreateEmiter("emiter 3", m_szEmiter, CObject3D(Vect3f(-5, 0, 5),0,0), Vect3f(.5f, .5f, .5f));
    m_pEmiters[3] = l_pEM->CreateEmiter("emiter 4", m_szEmiter, CObject3D(Vect3f(10, 0, -10),0,0), Vect3f(10, 1, 10));
    m_pEmiters[4] = l_pEM->CreateEmiter("emiter 5", m_szEmiter, CObject3D(Vect3f(10, 0, 10),0,0), Vect3f(10, 10, 10));
    m_pEmiters[5] = l_pEM->CreateEmiter("emiter 6", m_szEmiter, CObject3D(Vect3f(-5, 0, -5),0,0), Vect3f(.25f, .5f, .25f));
    //m_pEmiters[0]->Init(m_szEmiter, CObject3D(Vect3f(0, 0, 0),0,0), Vect3f(1, 1, 1));
    //m_pEmiters[1]->Init(m_szEmiter, CObject3D(Vect3f(5, 0, 0),0,0), Vect3f(.1f, .1f, .1f));
    //m_pEmiters[2]->Init(m_szEmiter, CObject3D(Vect3f(-5, 0, 5),0,0), Vect3f(.5f, .5f, .5f));
    //m_pEmiters[3]->Init(m_szEmiter, CObject3D(Vect3f(10, 0, -10),0,0), Vect3f(10, 1, 10));
    //m_pEmiters[4]->Init(m_szEmiter, CObject3D(Vect3f(10, 0, 10),0,0), Vect3f(10, 10, 10));
  }
  //m_pEmiter->Init("bubble", CObject3D(Vect3f(-14.6275f, 0.833153f, -4.08485f),0,0), Vect3f(0.391403f, 0.702762f, 0.194437f));



  SetOk(true);
  return IsOk();
}

void CParticleViewerProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CEntityProcess::Release");

  //for(int i = 0; i < NUM_EMISORS; ++i)
  //{
  //  CHECKED_DELETE( m_pEmiters[i] );
  //}
}

void CParticleViewerProcess::Update(float _fElapsedTime)
{
  //for(int i = 0; i < NUM_EMISORS; ++i)
  //  m_pEmiters[i]->Update(_fElapsedTime);
}

void CParticleViewerProcess::RenderScene(CRenderManager* _pRM)
{
  if(CORE->GetActionManager()->IsActionActive("ToggleDebug"))
  {
    CObject3D l_Poslluny;
    l_Poslluny.SetPosition(Vect3f(0,25,0));
    CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);
  
  
    CSphereCamera l_SphereCamera( 1, 700, 55.0f * FLOAT_PI_VALUE/180.0f,
                                  ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                                  &l_Poslluny,m_pPlayerPos);
  
  
    _pRM->SetupMatrices(&l_SphereCamera,false,false);
  }
  //CORE->GetPortalManager()->Render(_pRM);





  
  //----------------------------------------------------------------------------------------
  CORE->GetEmiterManager()->Render(_pRM);
  //----------------------------------------------------------------------------------------
}

void CParticleViewerProcess::RenderINFO(CRenderManager* _pRM)
{

  FONT_MANAGER->DrawText(0,30,colBLUE,FONT_MANAGER->GetTTF_Id("Titania"),m_szEmiter.c_str());
  if(CORE->GetActionManager()->IsActionActive("ToggleDebug"))
  {
    CORE->GetPhysicsManager()->DebugRender(_pRM);
    CORE->GetPortalManager()->DebugRender(_pRM);
    CORE->GetEntityManager()->DebugRender(_pRM);

    CFrustum l_Frustum;
    l_Frustum.Update(m_pCamera);
  
    Mat44f i;
    i.SetIdentity();
    _pRM->SetTransform(i);
  
    _pRM->DrawFrustum(&l_Frustum, colBLACK);

  }
  CORE->GetEmiterManager()->DebugRender(_pRM);


}

bool CParticleViewerProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp("NextEmiter",_pcAction) == 0)
  {
    NextEmiter();
  }
  if(strcmp("PrevEmiter",_pcAction) == 0)
  {
    PrevEmiter();
  }
  if(strcmp("ReloadParticles",_pcAction) == 0)
  {
    CORE->GetCoreEmiterManager()->Reload();
  }
  return false;
}


void CParticleViewerProcess::NextEmiter()
{
  CCoreEmiterManager* l_pCoreEmiterManager = CORE->GetCoreEmiterManager();
  const set<string>& l_szEmiterNames = l_pCoreEmiterManager->GetCoreNames();
  set<string>::const_iterator l_it = l_szEmiterNames.find(m_szEmiter);
  if(l_it == l_szEmiterNames.end())
  {
    l_it = l_szEmiterNames.begin();
  }
  
  if(l_it == l_szEmiterNames.end())
  {
    return;
  }

  ++l_it;
  if(l_it == l_szEmiterNames.end())
    l_it = l_szEmiterNames.begin();

  
  m_szEmiter = *l_it;
  for(int i = 0; i < NUM_EMISORS; ++i)
    m_pEmiters[i]->Reset(m_szEmiter);
}

void CParticleViewerProcess::PrevEmiter()
{
  CCoreEmiterManager* l_pCoreEmiterManager = CORE->GetCoreEmiterManager();
  const set<string>& l_szEmiterNames = l_pCoreEmiterManager->GetCoreNames();
  set<string>::const_iterator l_it = l_szEmiterNames.find(m_szEmiter);
  if(l_it == l_szEmiterNames.end())
  {
    l_it = l_szEmiterNames.begin();
  }
  
  if(l_it == l_szEmiterNames.end())
  {
    return;
  }

  if(l_it == l_szEmiterNames.begin())
    l_it = l_szEmiterNames.end();
  --l_it;

  
  m_szEmiter = *l_it;
  for(int i = 0; i < NUM_EMISORS; ++i)
    m_pEmiters[i]->Reset(m_szEmiter);
}