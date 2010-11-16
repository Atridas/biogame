#pragma once

#include <vector>
#include <string>
#include <base.h>

#include <Windows.h>

//---forward-declarations---
class CProcess;
class CCore;
class CRenderManager;
//--------------------------


using namespace std;

typedef vector<CProcess*> VectorProcessPtr;

class CEngine:
  public CBaseControl
{
  
public:
	                          CEngine(void):m_pProcess(0),m_pCore(0)                      {};
  virtual                   ~CEngine(void)                                              {Done();};

  bool                      Init					      (const string& _PathXML, HWND hWnd);
	void				              Update				      ();
	void				              Render				      ();
	void				              RenderScene		      ();
	void				              SetProcess		      (CProcess* _pProcess);

private:

  void                      Relase              ();

	CProcess*			            m_pProcess; //Temporal
	CCore*				            m_pCore;
	VectorProcessPtr	        m_vProcesses;
};

