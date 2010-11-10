#pragma once

#include "Process.h"

class CTestProcess: public CProcess
{
	void                  Relase();
  
	bool                  m_bIsOk;
public:
	CTestProcess(void):m_bIsOk(false){};
  virtual ~CTestProcess(void) {Done();};

  bool                  Init();

  void                  Done() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};

	void Update();
	void Render();
};

