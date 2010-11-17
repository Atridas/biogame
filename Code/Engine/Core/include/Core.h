#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"
#include <base.h>

// -- forward declarations --
class CRenderManager;
class CLanguageManager;
class CFontManager;
// --------------------------


class CCore:
  public CSingleton<CCore>,
  public CBaseControl
{

public:

                      CCore               (void): m_pRenderManager(0),m_pLanguageManager(0),m_pFontManager(0)
                                                  {};
  virtual             ~CCore              (void)                          {Done();};

  bool                Init(HWND hWnd);

	void Update();
	void Render();
  
  CRenderManager*     GetRenderManager      () const                        {return m_pRenderManager;};
  //void                SetRenderManager      (CRenderManager* _pRm)          {m_pRenderManager = _pRm;};
  CLanguageManager*   GetLanguageManager    () const                        {return m_pLanguageManager;};
  //void                SetLanguageManager    (CLanguageManager* _pLm)        {m_pLanguageManager = _pLm;};
  CFontManager*       GetFontManager        () const                        {return m_pFontManager;}

private:

  CCore               (const CCore& _Core)            {};
  void                 Release             ();




  CRenderManager*       m_pRenderManager;
  CLanguageManager*     m_pLanguageManager;
  CFontManager*         m_pFontManager;

};
