#include "GameProcess.h"
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
//#include "ParticleManager.h"

#include "Core.h"

void CGameProcess::Update(float _fElapsedTime)
{
  CObject3D* m_pPlayerPos = CORE->GetEntityManager()->GetEntity("Player")->GetComponent<CComponentObject3D>(CBaseComponent::ECT_OBJECT_3D);

  m_pOmniLight->SetPosition(m_pPlayerPos->GetPosition());
}

void CGameProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);
}

void CGameProcess::RenderINFO(CRenderManager* _pRM)
{
  //CORE->GetPhysicsManager()->DebugRender(_pRM);
}

bool CGameProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CGameProcess::Init");
  //LOGGER->SaveLogsInFile();

  m_pSceneEffectManager = CORE->GetSceneEffectManager();
  CORE->GetLightManager()->SetLightsEnabled(true);

  
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - Laboratori.xml");
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - Menjador.xml");
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - Passadis.xml");
  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/Level -2/XML/GameEntities - video.xml");

  
  CGameEntity* l_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
  m_pCamera = l_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();
  
  Vect3f l_vOmniColor = Vect3f(.5f,.5f,.5f);
  m_pOmniLight = CORE->GetLightManager()->CreateOmniLight("OmniViewerLight",Vect3f(0.0f),CColor(l_vOmniColor),0.1f,17.0f);
  m_pOmniLight->SetActive(true);


  //CORE->GetParticleManager()->SetAllEmittersActive(true);

  SetOk(true);
  return IsOk();
}

void CGameProcess::Release()
{
	// ----
}

bool CGameProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _szAction)
{
	return false;
}
