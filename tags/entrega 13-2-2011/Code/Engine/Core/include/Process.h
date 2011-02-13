#pragma once
#include "base.h"

//Forward declarations---------------------
class CCamera;
class CSceneEffectManager;
class CRenderManager;
//-----------------------------------------

/**
 * Intef�cie b�sica d'un proc�s.
 * Classe que designa l'interf�cie principal que ha de tenir un proc�s que faci servir l'engine.
**/
class CProcess:
  public CBaseControl
{

public:
  /**
   * Constructor per defecte.
   * Constructor de la classe. Permet especificar el nom que rebr� el proc�s.
   * @param _szProcessName Nom que rebr� el proc�s.
  **/
  CProcess(const string& _szProcessName):
      m_pCamera(0),m_pSceneEffectManager(0),m_szProcessName(_szProcessName),m_bRenderInfo(true)
                                                                          {};
  /**
   * M�tode d'update.
   * Aquest m�tode s'executa a cada iteraci�. Durant aquest es realitzaran els c�lculs de l�gica del proc�s.
   * @param _fElapsedTime �s necessari facilitar-hi el temps transcorregut desde l'�ltima crida al m�tode.
  **/
	virtual void          Update(float _fElapsedTime) = 0;
  virtual void          RenderINFO(CRenderManager* _pRM) = 0;
  /**
   * M�tode per mostrar informaci� de debug.
   * Aquest m�tode mostrar� per pantalla diversa informaci� que pot ser d'utilitat per al debug:
   *  - Nom del proc�s
   *  - FramesPerSecond
  **/
	void				          DebugInformation();
  /**
   * M�tode de pre renderitzat.
   * Aquest m�tode s'executa a cada iteraci�. Es realitzar� el prerender dels efectes d'escena.
   * @param _pRM Render Manager.
  **/
	virtual void          PreRender(CRenderManager* _pRM);
  /**
   * M�tode de render.
   * Aquest m�tode s'executa a cada iteraci�. Es realitzen els seg�ents passos:
   *  - S'activen els efectes d'escena.
   *  - Es renderitza l'escena.
   *  - Es captura el buffer d'escena pels efectes de post-processat.
   *  - Es realitza el post-processat d'efectes d'escena.
   *  - Es captura el buffer de post-processat.
   * @param _pRM Render Manager.
  **/
	virtual void          Render(CRenderManager* _pRM);
  /**
   * M�tode de render d'escena.
   * Aquest m�tode s'executa a cada iteraci�. Durant aquest es realitzar� el pintat de l'escena per pantalla.
   * @param _pRM Render Manager.
  **/
	virtual void          RenderScene(CRenderManager* _pRM) = 0;
  /**
   * M�tode d'inicialitzaci�.
   * Aquest m�tode inicialitzar� el proc�s, deixant-lo llest pel seu funcionament.
  **/
	virtual bool          Init() = 0;

  /**
   * Getter de la c�mara.
   * Aquest m�tode retorna l'actual c�mara assignada al proc�s i sobre la qual es dibuixa l'escena.
   * @return CCamera* que representa l'actual c�mara assignada al proc�s.
  **/
  virtual CCamera*      GetCamera                 ()         {return m_pCamera;};

  /**
   * Setter del SceneEffectManager.
   * Aquest m�tode especifica el SceneEffectManager que s'utilitzar�.
   * @param _pSceneEffectManager Punter al SceneEffectManager a utilitzar.
  **/
  virtual void          SetSceneEffectManager     (CSceneEffectManager* _pSceneEffectManager)         { m_pSceneEffectManager = _pSceneEffectManager; };

  /**
   * M�tode d'execuci� d'accions.
   * Aquest m�tode executar� accions concretes del proc�s durant un temps determinat.
   * @param _fDeltaSeconds Temps de duraci� de l'acci�.
   * @param _fDelta Temps de duraci� de l'acci�. TODO ????
   * @param _pcAction Nom identificatiu de l'acci� a realitzar.
   * @return True si l'acci� existeix. False si no.
  **/
  bool                  ExecuteAction             (float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  /**
   * M�tode d'execuci� d'accions scriptades.
   * Aquest m�tode executar� accions scriptades que contingui el proc�s.
   * @param _fDeltaSeconds Temps de duraci� de l'acci�.
   * @param _fDelta Temps de duraci� de l'acci�. TODO ????
   * @param _pcScript Nom identificatiu de l'acci� scriptada a realitzar.
   * @return True si l'acci� existeix. False si no.
  **/
  bool                  ExecuteScript             (float _fDeltaSeconds, float _fDelta, const char* _pcScript);

  /**
   * TODO
   * M�tode d'execuci� d'accions.
   * Aquest m�tode executar� accions concretes del proc�s durant un temps determinat.
   * @param _fDeltaSeconds Temps de duraci� de l'acci�.
   * @param _fDelta Temps de duraci� de l'acci�. TODO ????
   * @param _pcAction Nom identificatiu de l'acci� a realitzar.
   * @return True si l'acci� existeix. False si no.
  **/
  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction) = 0;
  bool getRenderInfo() {return m_bRenderInfo;}

protected:

  /**
   * Punter a c�mara.
   * Punter que representa l'actual c�mara assignada al proc�s. L'escena es dibuixar� a partir d'aquesta c�mara.
  **/
  CCamera*              m_pCamera;

  /**
   * Punter a Manager d'efectes d'escena.
  **/
  CSceneEffectManager*  m_pSceneEffectManager;

  /**
   * Setter del DebugInfo.
   * Aquest m�tode permet determinar si es far� o no el pintat de debug.
   * @param _bRenderInfo Si es vol o no mostrar la informaci� de debug.
  **/
  virtual void          SetDebugInfo     (bool _bRenderInfo)         { m_bRenderInfo = _bRenderInfo; };
private:

  /**
   * Nom del proc�s.
  **/
  string                m_szProcessName;
  /**
   * Control de pintat.
   * Variable utilitzada per determinar si s'ha de fer el pintat de debug o no.
  **/
  bool                  m_bRenderInfo;
};

