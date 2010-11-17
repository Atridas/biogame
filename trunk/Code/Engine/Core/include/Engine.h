#pragma once

#include <vector>
#include <string>
#include <base.h>

#include <Windows.h>

//---forward-declarations---
class CProcess;
class CCore;
class CRenderManager;
class CTimer;
//--------------------------


using namespace std;

typedef vector<CProcess*> VectorProcessPtr;

class CEngine:
  public CBaseControl
{
  
public:
	                          CEngine(void):m_pProcess(0),m_pCore(0)                      {};
  virtual					          ~CEngine(void)                                              {Done();};

  bool                      Init					      (const string& _PathXML, HWND hWnd);
	void				              Update				      ();
	void				              Render				      ();
	void				              RenderScene		      ();
	void				              SetProcess		      (CProcess* _pProcess);

private:

  void                      Release              ();

	CProcess*			            m_pProcess; //Temporal
	CCore*				            m_pCore;
	CTimer*							m_pTimer;
	VectorProcessPtr	        m_vProcesses;
};

