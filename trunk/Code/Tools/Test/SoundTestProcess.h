#pragma once

#include "Process.h"
#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CRenderManager;
//-----------------------------------------

class CSoundTestProcess:
  public CProcess
{

public:
                        CSoundTestProcess       (): CProcess("CSoundTestProcess"), m_pTargetObject(0), m_pObjectCamera(0)
                                                {};
  virtual               ~CSoundTestProcess      ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  //void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

  void UpdateCamera(float _fDeltaPitch, float _fDeltaYaw);
  void UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw);

	void                  Release                 ();

  CObject3D*            m_pTargetObject;

  CCamera*              m_pObjectCamera;
  
};