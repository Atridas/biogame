#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"

// -- forward declarations --
class CRenderManager;
// --------------------------


class CCore: public CSingleton<CCore>
{

public:

                      CCore               (void):m_bIsOk(false),
                                          m_pRenderManager(0)       {};
                      CCore               (const CCore& _Core)      {;};
                      virtual ~CCore(void)                          {Done();};

  bool                Init(HWND hWnd);

  void                Done                ()                        {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                IsOk                () const                  {return m_bIsOk;};

	void Update();
	void Render();

  CRenderManager*     GetRenderManager    () const                 {return m_pRenderManager;};
  void                SetRenderManager    (CRenderManager* _pRm)   {m_pRenderManager = _pRm;};

private:

  CRenderManager* m_pRenderManager;

  void                  Relase();
  
  bool                  m_bIsOk;

};

