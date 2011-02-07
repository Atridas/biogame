#pragma once

#include "Process.h"
#include <base.h>

#define MODE_ESCENA 0
#define MODE_MESH 1
#define MODE_ANIMATS 2


//Forward declarations---------------------
class CObject3D;
class CRenderManager;
//-----------------------------------------

class CViewerProcess:
  public CProcess
{

public:
                        CViewerProcess            (): CProcess("Viewer Process"), m_pObject(0), m_pObjectCamera(0),
                                                    m_fVelocity(0.0f),m_bRenderLights(false),m_iMode(0)
                                                {};
  virtual               ~CViewerProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

	void                  Release                 ();

  int                   m_iState;
  bool                  m_bStateChanged;
  CObject3D*            m_pObject;
  CObject3D*            m_pObjectBot;
  CCamera*              m_pObjectCamera;
  float                 m_fVelocity;
  bool                  m_bRenderLights;
  int                   m_iMode;
};