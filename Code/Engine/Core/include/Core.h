#pragma once

#include <Windows.h>
#include "Utils/Singleton.h"
#include <base.h>

// -- forward declarations --
struct SInitParams;
class CRenderManager;
class CRenderer;
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
//class CSceneEffectManager;
class CScriptManager;
class CLogRender;
class CConsole;
class CGUIManager;
class CPhysicsManager;
class CSoundManager;
class CEngine;
class CEntityManager;
class CPhysicTriggerReport;
class CPhysicCollisionReport;
class CPortalManager;
class CIAManager;
class CEmiterCoreManager;
class CEmiterManager;
class CLevelChanger;
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
                                                  m_pActionManager(0),m_pRenderableObjectsManager(0),m_pLightManager(0),
                                                  m_pRenderer(0), m_pTimer(0), m_pScriptManager(0),m_pTextureManager(0),
                                                  m_pStaticMeshManager(0),m_pAnimatedModelManager(0),m_pEffectManager(0),
                                                  m_pGUIManager(0),m_pPhysicsManager(0),
                                                  m_pSoundManager(0), m_pEntityManager(0), m_pPortalManager(0), m_pIAManager(0),
                                                  m_pEmiterCoreManager(0), m_pEmiterManager(0), m_pLevelChanger(0),m_bExit(false),
                                                  m_bPaused(false)
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

  CRenderer*              GetRenderer() const {return m_pRenderer;};

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

  //CSceneEffectManager* GetSceneEffectManager () const                        {return m_pSceneEffectManager;};

  CScriptManager*     GetScriptManager      () const                        {return m_pScriptManager;};

  CLogRender*         GetLogRender          () const                        {return m_pLogRender;};

  CConsole*           GetConsole            () const                        {return m_pConsole;};

  CGUIManager*        GetGUIManager         () const                        {return m_pGUIManager;};

  CPhysicsManager*    GetPhysicsManager     () const                        {return m_pPhysicsManager;};

  CSoundManager*      GetSoundManager       () const                        {return m_pSoundManager;};

  CEntityManager*     GetEntityManager      () const                        {return m_pEntityManager;};

  CPortalManager*     GetPortalManager      () const                        {return m_pPortalManager;};

  CIAManager*         GetIAManager          () const                        {return m_pIAManager;};

  CEmiterCoreManager* GetEmiterCoreManager  () const                        {return m_pEmiterCoreManager;};

  CEmiterManager*     GetEmiterManager      () const                        {return m_pEmiterManager;};

  CLevelChanger*      GetLevelChanger       () const                        {return m_pLevelChanger;};

  CPhysicTriggerReport*    GetPhysicTriggerReport()   const { return   m_pPhysicTriggerReport;   };
  CPhysicCollisionReport*  GetPhysicCollisionReport() const { return   m_pPhysicCollisionReport; };
  string                   GetLuaInitFile()           const { return   m_szLuaInitFile;          };
  
  void SetExit(bool _bExit) {m_bExit = _bExit;};
  bool GetExit() const      {return m_bExit;};

  void SetPause(bool _bPause) {m_bPaused = _bPause;};
  bool GetPause() const       {return m_bPaused;};

private:

  friend CLevelChanger;



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


  bool   m_bExit, m_bPaused;

  string m_szLuaInitFile;

  /**
   * Timer.
  **/
  CTimer*                     m_pTimer;
  /**
   * RenderManager.
  **/
  CRenderManager*             m_pRenderManager;

  CRenderer* m_pRenderer;

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

  //CSceneEffectManager*        m_pSceneEffectManager;

  CScriptManager*             m_pScriptManager;

  CLogRender*                 m_pLogRender;

  CConsole*				            m_pConsole;

  CGUIManager*                m_pGUIManager;

  CPhysicsManager*            m_pPhysicsManager;

  CSoundManager*              m_pSoundManager;

  CEntityManager*             m_pEntityManager;

  CPhysicTriggerReport*       m_pPhysicTriggerReport;

  CPhysicCollisionReport*     m_pPhysicCollisionReport;

  CPortalManager*             m_pPortalManager;

  CIAManager*                 m_pIAManager;

  CEmiterCoreManager*         m_pEmiterCoreManager;

  CEmiterManager*             m_pEmiterManager;

  CLevelChanger*              m_pLevelChanger;
};

