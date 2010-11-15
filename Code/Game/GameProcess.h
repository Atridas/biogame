#pragma once

#include <Process.h>

class CGameProcess :
	public CProcess
{
public:
	CGameProcess(void);
	virtual ~CGameProcess(void);

  virtual bool          Init();
	void Update();
	void Render();
private:
  virtual void          Relase();
};

