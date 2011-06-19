#include "GameProcess.h"
#include "RenderManager.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "RenderableObjectsManager.h"
#include "LightManager.h"
#include "EntityManager.h"
#include "Component3rdPSCamera.h"
#include "PhysicsManager.h"

#include "Core.h"

void CGameProcess::Update(float _fElapsedTime)
{
}

void CGameProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);
}

void CGameProcess::RenderINFO(CRenderManager* _pRM)
{
  CORE->GetPhysicsManager()->DebugRender(_pRM);
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

  
  CGameEntity *l_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
  m_pCamera = l_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();
  
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
