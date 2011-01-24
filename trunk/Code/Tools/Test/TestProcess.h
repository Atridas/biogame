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
	                      CTestProcess            ():CProcess("CTestProcess") {};
  virtual               ~CTestProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  //void                  PreRender               (CRenderManager* _pRM);
  //void                  Render                  (CRenderManager* _pRM);
  void                  RenderScene             (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

	void                  Release                 ();

  CObject3D*            m_pObject;
  CObject3D*            m_pCube;
  Vect2f                m_vPos;
  CCamera*              m_pCubeCamera;
  CCamera*              m_pObjectCamera;
  //Vect3f                m_vCubePos;
  //Vect3f                m_vCubeRot;
  float                 m_fVelocity;
};

