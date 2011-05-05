#pragma once

#include "Process.h"
#include "EntityManager.h"
#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CRenderManager;
class CPhysicUserData;
class CPhysicActor;
//-----------------------------------------

class CEntityProcess:
  public CProcess
{

public:
                        CEntityProcess       (): CProcess("CEntityProcess"),m_pPlayerEntity(0),m_pUserData(0),m_pPActorPlane(0)
                                                {};
  virtual               ~CEntityProcess      ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

	void                  Release                 ();

  CGameEntity*          m_pPlayerEntity;
  CPhysicUserData*      m_pUserData;
  CPhysicActor*         m_pPActorPlane;
};
