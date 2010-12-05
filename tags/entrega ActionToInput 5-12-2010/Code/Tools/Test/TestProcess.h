#pragma once

#include "Process.h"
#include <base.h>

class CObject3D;

class CTestProcess:
  public CProcess
{

public:
	                      CTestProcess            ():CProcess("CTestProcess") {};
  virtual               ~CTestProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
	void                  Render                  ();

  virtual bool          ExecuteAction           (float _fDeltaSeconds, float _fDelta, const string& _szAction);

private:

	void                  Release                 ();

  CObject3D*            m_pObject;
  CObject3D*            m_pCube;
  Vect2f                m_vPos;
  CCamera*              m_pCubeCamera;
  CCamera*              m_pObjectCamera;
  //Vect3f                m_vCubePos;
  //Vect3f                m_vCubeRot;
};

