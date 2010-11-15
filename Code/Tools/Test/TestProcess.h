#pragma once

#include "Process.h"

class CTestProcess: public CProcess
{

public:
	CTestProcess(void){};
  virtual ~CTestProcess(void) {Done();};

  virtual bool      Init();

  void              Done() {if(IsOk()) Relase(); m_bIsOk=false;};

	void Update();
	void Render();

private:

	virtual void          Relase();


};

