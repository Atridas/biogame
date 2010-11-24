#pragma once

#include "Process.h"
#include <base.h>

class CObject3D;

class CTestProcess:
  public CProcess
{

public:
	                      CTestProcess            ()                      {};
  virtual               ~CTestProcess           ()                      {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
	void                  Render                  ();

private:

	void                  Release                 ();

  CObject3D*            m_pObject;
  CObject3D*            m_pCube;
  Vect2f                m_vPos;
  CCamera*              m_pCubeCamera;
  //Vect3f                m_vCubePos;
  //Vect3f                m_vCubeRot;
};

