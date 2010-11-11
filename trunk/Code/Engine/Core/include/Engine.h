#pragma once

#include <vector>
#include <string>

#include <Windows.h>

//---forward-declarations---
class CProcess;
class CCore;
class CRenderManager;
//--------------------------


using namespace std;

typedef vector<CProcess*> VectorProcessPtr;

class CEngine
{
  void                  Relase();
  
	bool                  m_bIsOk;
public:
	CEngine(void):m_bIsOk(false),m_pProcess(0),m_pCore(0){};
  virtual ~CEngine(void) {Done();};

  bool        Init					(const string& _PathXML, HWND hWnd);
  void        Done					() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool        IsOk					() const {return m_bIsOk;};
	void				Update				();
	void				Render				();
	void				RenderScene		(CRenderManager* rm);
	void				SetProcess		(CProcess* _pProcess);

private:

	CProcess*			m_pProcess; //Temporal
	CCore*				m_pCore;
	VectorProcessPtr	m_vProcesses;
};

