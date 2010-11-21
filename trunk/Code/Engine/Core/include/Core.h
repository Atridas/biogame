#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"
#include <base.h>

// -- forward declarations --
class CRenderManager;
class CLanguageManager;
class CFontManager;
class CXMLTreeNode;
class CInputManager;
struct SInitParams;
// --------------------------


class CCore:
  public CSingleton<CCore>,
  public CBaseControl
{

public:

                      CCore               (void): m_pRenderManager(0),m_pLanguageManager(0),m_pFontManager(0),m_pInputManager(0)
                                                  {};
  virtual             ~CCore              (void)                          {Done();};

  bool                Init(HWND hWnd, const SInitParams& _InitParams);

	void Update(float elapsedTime);
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
  CInputManager*        m_pInputManager;

};

