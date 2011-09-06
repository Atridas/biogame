#include "TestProcess.h"
#include "Core.h"
#include "ActionManager.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "EntityManager.h"
#include "ShoulderCamera.h"
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
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentRenderableObject.h"
#include "ComponentAnimation.h"
#include "ComponentVida.h"
#include "ComponentStateMachine.h"
#include "ComponentAnimation.h"
#include <LightManager.h>
#include "SpotLight.h"

#define ALTURA_CONTROLLER 1.5f
#define RADIUS_CONTROLLER 0.3f
#define COLISIONABLE_MASK ((1<<GROUP_COLLIDABLE_NON_PUSHABLE))



bool CTestProcess::Init()
{
  //m_pSceneEffectManager = CORE->GetSceneEffectManager();

  CORE->GetLightManager()->SetLightsEnabled(true);

  CORE->GetEntityManager()->LoadEntitiesFromXML("Data/Levels/NivellProves/XML/GameEntities.xml");

  m_pPlayerEntity = CORE->GetEntityManager()->GetEntity("Player");
  m_pCamera = m_pPlayerEntity->GetComponent<CComponent3rdPSCamera>()->GetCamera();

  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{

}

void CTestProcess::Update(float _fElapsedTime)
{

}

void CTestProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  //Mat44f m;
	//_pRM->SetTransform(m.SetIdentity().Translate(m_pCamera->GetLookAt()));
  //_pRM->DrawAxis();
  //_pRM->DrawLine(m_pCamera->GetLookAt(),m_pCamera->GetEye()-Vect3f(0.0f,0.5f,0.0f)+m_pCamera->GetDirection().Normalize()*5.0f,colRED);
  //_pRM->DrawLine(m_pCamera->GetEye(),Vect3f(0.0f),colBLUE);
}

void CTestProcess::RenderINFO(CRenderManager* _pRM)
{
  if(CORE->GetActionManager()->IsActionActive("ToggleDebug"))
  {
    CORE->GetPhysicsManager()->DebugRender(_pRM);
    //m_PortalManager.DebugRender(_pRM);

    //CFrustum l_Frustum;
    //l_Frustum.Update(m_pCamera);
  
    //Mat44f i;
    //i.SetIdentity();
    //_pRM->SetTransform(i);
  
    //_pRM->DrawFrustum(&l_Frustum, colBLACK);
  }
}

bool CTestProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  
  return false;
}
