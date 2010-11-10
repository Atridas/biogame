#pragma once

#include <Process.h>

class CGameProcess :
	public CProcess
{
public:
	CGameProcess(void);
	virtual ~CGameProcess(void);

	void Update();
	void Render();
};

