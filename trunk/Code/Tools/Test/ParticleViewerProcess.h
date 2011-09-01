#pragma once

#include "Process.h"
#include "EntityManager.h"
#include <base.h>

//Forward declarations---------------------
class CEmiterInstance;
//-----------------------------------------

#define NUM_EMISORS 6

class CParticleViewerProcess:
  public CProcess
{

public:
                        CParticleViewerProcess       (): CProcess("CParticleViewerProcess"),m_pPlayerEntity(0)
                                                {};
  virtual               ~CParticleViewerProcess      ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

	void                  Release                 ();
  
  void                  NextEmiter              ();
  void                  PrevEmiter              ();


  CGameEntity*          m_pPlayerEntity;
  CEmiterInstance*      m_pEmiters[NUM_EMISORS];
  string                m_szEmiter;
};
