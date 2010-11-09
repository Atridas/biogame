#pragma once

#include <Process.h>

class CGameProcess :
	public CProcess
{
public:
	CGameProcess(void);
	~CGameProcess(void);

	void Update();
	void Render();
};

