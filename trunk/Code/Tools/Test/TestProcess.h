#pragma once

#include "Process.h"

class CTestProcess:
  public CProcess
{

public:
	                      CTestProcess            ()          {};
  virtual               ~CTestProcess           ()          {Done();};

  virtual bool          Init();

	void                  Update                  ();
	void                  Render                  ();

private:

	void                  Relase                  ();

};

