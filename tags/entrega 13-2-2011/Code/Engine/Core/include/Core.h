#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"
#include <base.h>

// -- forward declarations --
struct SInitParams;
class CRenderManager;
class CLanguageManager;
class CFontManager;
class CXMLTreeNode;
class CTimer;
class CInputManager;
class CActionToInput;
class CRenderableObjectsManager;
class CAnimatedModelManager;
class CLightManager;
class CSceneEffectManager;
class CScriptManager;
// --------------------------

/**
 * Classe Core.
 * Core de l'engine. �s el nucli de l'engine que cont� els managers principals.
**/
class CCore:
  public CSingleton<CCore>,
  public CBaseControl
{

public:
  /**
   * Constructor per defecte.
   * Core est� implementat segons el patr� de Singleton.
  **/
                      CCore               (void): m_pRenderManager(0),m_pLanguageManager(0),m_pFontManager(0),m_pInputManager(0),
                                                  m_pActionToInput(0),m_pRenderableObjectsManager(0),m_pLightManager(0),
                                                  m_pSceneEffectManager(0), m_pTimer(0), m_pScriptManager(0)
                                                  {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  virtual             ~CCore              (void)                          {Done();};

  /**
   * M�tode d'inicialitzaci� del Core.
   * Aquest m�tode inicialitzar� el Core i tots els seus m�nagers segons els par�metres facilitats.
   * @param _InitParams Par�metres d'inicialitzaci�.
   * @return True si s'ha iniciat correctament. False si no.
   * @see SInitParams
  **/
  bool                Init(HWND hWnd, const SInitParams& _InitParams);

	/**
   * M�tode d'update.
   * Aquest m�tode s'executa a cada iteraci�. Durant aquest es calcular� i realitzar� la l�gica d'inputs desde l'�ltima iteraci�.
  **/
  void Update();
 /**
   * M�tode de render.
   * Aquest m�tode s'executa a cada iteraci�. Durant aquest es far� render de tots els RenderableObjects.
 **/
 //void Render();
  
  /**
   * Getter del RenderManager.
   * @return El RenderManager.
   * @see RENDER_MANAGER
  **/
  CTimer*             GetTimer              () const                        {return m_pTimer;};
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
  //CAnimatedModelManager*  GetAnimatedModelManager      () const             {return m_pAnimatedModelManager;}

  CLightManager*      GetLightManager       () const                        {return m_pLightManager;}
  CRenderableObjectsManager*  GetRenderableObjectsManager      () const                        {return m_pRenderableObjectsManager;}

  CSceneEffectManager*GetSceneEffectManager () const                        {return m_pSceneEffectManager;};

  CScriptManager*     GetScriptManager      () const                        {return m_pScriptManager;};

private:

  /**
   * Constructor per c�pia.
   * Core est� implementat segons el patr� Singleton.
   * @param _Core Core.
  **/
  CCore               (const CCore& _Core)            {};
  /**
   * M�tode d'alliberaci� de recursos.
   * Aquest m�tode alliberar� tots els recursos del Core.
  **/
  void                 Release             ();

  /**
   * Timer.
  **/
  CTimer*                     m_pTimer;
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
  /**
   * LightManager
  **/
  CLightManager*              m_pLightManager;

  CSceneEffectManager*        m_pSceneEffectManager;

  CScriptManager*             m_pScriptManager;

  /**
   * AnimatedModelManager.
  **/
  //CAnimatedModelManager*  m_pAnimatedModelManager;
};
