#pragma once

#include "Process.h"
#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CRenderManager;
//-----------------------------------------

class CTestProcess:
  public CProcess
{

public:
                        CTestProcess            (): CProcess("CTestProcess"), m_pObject(0), m_pObjectCamera(0),
                                                    m_fVelocity(0.0f),m_bRenderLights(false)
                                                {};
  virtual               ~CTestProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

	void                  Release                 ();

  CObject3D*            m_pObject;
  CCamera*              m_pObjectCamera;
  float                 m_fVelocity;
  bool                  m_bRenderLights;
};