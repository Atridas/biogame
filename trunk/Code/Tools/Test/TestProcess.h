#pragma once

#include "Process.h"
#include <base.h>

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

  Vect2f                m_vPos;
  float                 m_fRotation;
};

