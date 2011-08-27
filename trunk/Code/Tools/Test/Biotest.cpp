#include "Biotest.h"
#include "RenderManager.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "RenderableObjectsManager.h"
#include "LightManager.h"
#include "EntityManager.h"
#include "Component3rdPSCamera.h"
#include "PhysicsManager.h"
#include "ComponentObject3D.h"
#include "OmniLight.h"
#include "ParticleManager.h"
#include "BillBoardManager.h"
#include "IAManager.h"
#include "PortalManager.h"
#include "GraphDefines.h"
#include "SphereCamera.h"
#include "ActionManager.h"

#include "Core.h"

void CBiotestProcess::Update(float _fElapsedTime)
{
  //CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);


  //Vect3f l_vPlayerPos = m_pPlayerPos->GetPosition();
  //Vect3f l_vDirCampera = m_pCamera->GetDirection();

  //Vect3f l_vLightMod(l_vDirCampera.x, 0, l_vDirCampera.z);
  //l_vLightMod.Normalize();
  //l_vLightMod = Vect3f(0,1,0) - l_vLightMod;

  //m_pOmniLight->SetPosition(l_vPlayerPos + l_vLightMod);
  //CORE->GetParticleManager()->Update(_fElapsedTime,m_pCamera);
  CORE->GetBillBoardManager()->Update(_fElapsedTime,m_pCamera);


}

void CBiotestProcess::RenderScene(CRenderManager* _pRM)
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

  //CORE->GetRenderableObjectsManager()->Render(_pRM);
  CORE->GetPortalManager()->Render(_pRM);
  CORE->GetParticleManager()->Render(_pRM);
  CORE->GetBillBoardManager()->Render(_pRM);
}

void CBiotestProcess::RenderINFO(CRenderManager* _pRM)
{
  CORE->GetPhysicsManager()->DebugRender(_pRM);
  CORE->GetPortalManager()->DebugRender(_pRM);
  CORE->GetIAManager()->GetGraph()->DebugRender(_pRM);
}

bool CBiotestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CGameProcess::Init");

  m_pSceneEffectManager = CORE->GetSceneEffectManager();
  CORE->GetLightManager()->SetLightsEnabled(true);

  
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - Laboratori.xml");
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - Menjador.xml");
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - Passadis.xml");
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - video.xml");

  
  CGameEntity* l_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
  m_pCamera = l_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();
  
  /*Vect3f l_vOmniColor = Vect3f(.5f,.5f,.5f);
  m_pOmniLight = CORE->GetLightManager()->CreateOmniLight("OmniViewerLight",Vect3f(0.0f),CColor(l_vOmniColor),0.1f,17.0f);
  m_pOmniLight->SetActive(true);*/

  
  CORE->GetIAManager()->CompleteGraph();
  CORE->GetParticleManager()->SetAllEmittersActive(true);

  SetOk(true);
  return IsOk();
}

void CBiotestProcess::Release()
{
	// ----
}

bool CBiotestProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _szAction)
{
	return false;
}
