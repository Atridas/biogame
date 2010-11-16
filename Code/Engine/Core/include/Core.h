#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"
#include <base.h>

// -- forward declarations --
class CRenderManager;
// --------------------------


class CCore:
  public CSingleton<CCore>,
  public CBaseControl
{

public:

                      CCore               (void):m_pRenderManager(0)      {};
  virtual             ~CCore              (void)                          {Done();};

  bool                Init(HWND hWnd);

	void Update();
	void Render();

  CRenderManager*     GetRenderManager    () const                        {return m_pRenderManager;};
  void                SetRenderManager    (CRenderManager* _pRm)          {m_pRenderManager = _pRm;};

private:

  CCore               (const CCore& _Core)            {};

  CRenderManager*      m_pRenderManager;

  void                 Relase             ();

};

