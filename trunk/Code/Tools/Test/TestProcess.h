#pragma once

#include "Process.h"
#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CRenderManager;
class CRenderableAnimatedInstanceModel;
class CGameEntity;
//-----------------------------------------

class CTestProcess:
  public CProcess
{

public:
                        CTestProcess            (): CProcess("CTestProcess")
                                                {};
  virtual               ~CTestProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  //void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:
  
  void                  Release                 ();


  CGameEntity*          m_pPlayerEntity;
};