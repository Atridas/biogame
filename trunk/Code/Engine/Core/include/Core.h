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
class CActionToInput;
class CRenderableObjectsManager;
// --------------------------

/**
 * Classe Core.
 * Core de l'engine. És el nucli de l'engine que conté els managers principals.
**/
class CCore:
  public CSingleton<CCore>,
  public CBaseControl
{

public:
  /**
   * Constructor per defecte.
   * Core està implementat segons el patró de Singleton.
  **/
                      CCore               (void): m_pRenderManager(0),m_pLanguageManager(0),m_pFontManager(0),m_pInputManager(0),
                                                  m_pActionToInput(0),m_pRenderableObjectsManager(0)
                                                  {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  virtual             ~CCore              (void)                          {Done();};

  /**
   * Mètode d'inicialització del Core.
   * Aquest mètode inicialitzarà el Core i tots els seus mànagers segons els paràmetres facilitats.
   * @param _InitParams Paràmetres d'inicialització.
   * @return True si s'ha iniciat correctament. False si no.
   * @see SInitParams
  **/
  bool                Init(HWND hWnd, const SInitParams& _InitParams);

	/**
   * Mètode d'update.
   * Aquest mètode s'executa a cada iteració. Durant aquest es calcularà i realitzarà la lògica d'inputs desde l'última iteració.
   * @params _fElapsedTime temps que ha transcorregut desde l'última execuciód el mètode.
  **/
  void Update(float _fElapsedTime);
	/**
   * Mètode de render.
   * Aquest mètode s'executa a cada iteració. Durant aquest es farà render de tots els RenderableObjects.
  **/
  void Render();
  
  /**
   * Getter del RenderManager.
   * @return El RenderManager.
   * @see RENDER_MANAGER
  **/
  CRenderManager*     GetRenderManager      () const                        {return m_pRenderManager;};
  //void                SetRenderManager      (CRenderManager* _pRm)          {m_pRenderManager = _pRm;};

  /**
   * Getter del LanguageManager.
   * @return El LanguageManager.
  **/
  CLanguageManager*   GetLanguageManager    () const                        {return m_pLanguageManager;};
  //void                SetLanguageManager    (CLanguageManager* _pLm)        {m_pLanguageManager = _pLm;};

  /**
   * Getter del FontManager.
   * @return El FontManager.
   * @see FONT_MANAGER
  **/
  CFontManager*       GetFontManager        () const                        {return m_pFontManager;}
  /**
   * Getter de l' InputManager.
   * @return L'InputManager.
   * @see INPUT_MANAGER
  **/
  CInputManager*		  GetInputManager       () const                        {return m_pInputManager;}
  /**
   * Getter de l'ActionToInput.
   * @return L'ActionToInput.
  **/
  CActionToInput*     GetActionToInput      () const                        {return m_pActionToInput;}

private:

  /**
   * Constructor per còpia.
   * Core està implementat segons el patró Singleton.
   * @param _Core Core.
  **/
  CCore               (const CCore& _Core)            {};
  /**
   * Mètode d'alliberació de recursos.
   * Aquest mètode alliberarà tots els recursos del Core.
  **/
  void                 Release             ();

  /**
   * RenderManager.
  **/
  CRenderManager*             m_pRenderManager;
  /**
   * LanguageManager.
  **/
  CLanguageManager*           m_pLanguageManager;
  /**
   * FontManager.
  **/
  CFontManager*               m_pFontManager;
  /**
   * InputManager.
  **/
  CInputManager*              m_pInputManager;
  /**
   * ActionToInput.
  **/
  CActionToInput*             m_pActionToInput;
  /**
   * RenderableObjectsManager.
  **/
  CRenderableObjectsManager*  m_pRenderableObjectsManager;
};

