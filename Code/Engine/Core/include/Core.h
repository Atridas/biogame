#pragma once

#include <Windows.h>
// -- forward declarations --
class CRenderManager;
// --------------------------


class CCore
{
  void                  Relase();
  
	bool                  m_bIsOk;
	CCore(const CCore& _Core) {;};
	CCore(void):m_bIsOk(false),m_pRenderManager(0){};
  virtual ~CCore(void) {Done();};
public:

  bool                  Init(HWND hWnd);

  void                  Done() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};

  static CCore* GetInstance() { static CCore ls_Core; return &ls_Core;};

	void Update();
	void Render();

  CRenderManager*     GetRenderManager    () const                 {return m_pRenderManager;};
  void                SetRenderManager    (CRenderManager* _pRm)   {m_pRenderManager = _pRm;};

private:

  CRenderManager* m_pRenderManager;
};

