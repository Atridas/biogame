#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"
#include <base.h>

// -- forward declarations --
struct SInitParams;
class CRenderManager;
class CTextureManager;
class CStaticMeshManager;
class CAnimatedModelManager;
class CEffectManager;
class CLanguageManager;
class CFontManager;
class CXMLTreeNode;
class CTimer;
class CInputManager;
//class CActionToInput;
class CActionManager;
class CRenderableObjectsManager;
class CAnimatedModelManager;
class CLightManager;
class CSceneEffectManager;
class CScriptManager;
class CLogRender;
class CConsole;
class CGUIManager;
class CPhysicsManager;
class CParticleManager;
class CSoundManager;
class CEngine;
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
                                                  m_pActionManager(0),m_pRenderableObjectsManager(0),m_pLightManager(0),
                                                  m_pSceneEffectManager(0), m_pTimer(0), m_pScriptManager(0),m_pTextureManager(0),
                                                  m_pStaticMeshManager(0),m_pAnimatedModelManager(0),m_pEffectManager(0),
                                                  m_pGUIManager(0),m_pPhysicsManager(0),m_pParticleManager(0),m_pSoundManager(0),
                                                  m_pEngine(0)
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
  bool                Init(HWND hWnd, const SInitParams& _InitParams, CEngine* _pEngine);

	/**
   * Mètode d'update.
   * Aquest mètode s'executa a cada iteració. Durant aquest es calcularà i realitzarà la lògica d'inputs desde l'última iteració.
  **/
  void Update();
 /**
   * Mètode de render.
   * Aquest mètode s'executa a cada iteració. Durant aquest es farà render de tots els RenderableObjects.
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

  CTextureManager*        GetTextureManager       () const {return m_pTextureManager;};
  
  CStaticMeshManager*     GetStaticMeshManager    () const {return m_pStaticMeshManager;};
  
  CAnimatedModelManager*  GetAnimatedModelManager () const {return m_pAnimatedModelManager;};
  
  CEffectManager*         GetEffectManager        () const {return m_pEffectManager;};

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
  CActionManager* GetActionManager() const {return m_pActionManager;};
  //CAnimatedModelManager*  GetAnimatedModelManager      () const             {return m_pAnimatedModelManager;}

  CLightManager*      GetLightManager       () const                        {return m_pLightManager;}
  
  CRenderableObjectsManager*  GetRenderableObjectsManager      () const                        {return m_pRenderableObjectsManager;}

  CSceneEffectManager*GetSceneEffectManager () const                        {return m_pSceneEffectManager;};

  CScriptManager*     GetScriptManager      () const                        {return m_pScriptManager;};

  CLogRender*         GetLogRender          () const                        {return m_pLogRender;};

  CConsole*           GetConsole            () const                        {return m_pConsole;};

  CGUIManager*        GetGUIManager         () const                        {return m_pGUIManager;};
  CPhysicsManager*    GetPhysicsManager     () const                        {return m_pPhysicsManager;};

  CParticleManager*   GetParticleManager    () const                        {return m_pParticleManager;};

  CSoundManager*      GetSoundManager       () const                        {return m_pSoundManager;};

  CEngine*            GetEngine             () const                        {return m_pEngine;};

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
   * Timer.
  **/
  CTimer*                     m_pTimer;
  /**
   * RenderManager.
  **/
  CRenderManager*             m_pRenderManager;

  CTextureManager*        m_pTextureManager;

  CStaticMeshManager*     m_pStaticMeshManager;

  CAnimatedModelManager*  m_pAnimatedModelManager;

  CEffectManager*         m_pEffectManager;

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
  CActionManager*             m_pActionManager;
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

  CLogRender*                 m_pLogRender;

  CConsole*				            m_pConsole;

  CGUIManager*                m_pGUIManager;

  CPhysicsManager*            m_pPhysicsManager;

  CParticleManager*           m_pParticleManager;

  CSoundManager*              m_pSoundManager;

  CEngine*                    m_pEngine;
};

