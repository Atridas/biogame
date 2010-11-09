#pragma once

#include <vector>

//---forward-declarations---
class CProcess;
class CCore;
//--------------------------


using namespace std;

typedef vector<CProcess*> VectorProcessPtr;

class CEngine
{
public:
	CEngine(void);
	~CEngine(void);

	void	Update();
	void	Render();
	void	SetProcess(CProcess* _pProcess);

private:

	CProcess*			m_pProcess; //Temporal
	CCore*				m_pCore;
	VectorProcessPtr	m_vProcesses;
};

