#pragma once
#include "base.h"

//Forward declarations---------------------
class CCamera;
class CSceneEffectManager;
class CRenderManager;
//-----------------------------------------

/**
 * Intefície bàsica d'un procés.
 * Classe que designa l'interfície principal que ha de tenir un procés que faci servir l'engine.
**/
class CProcess:
  public CBaseControl
{

public:
  /**
   * Constructor per defecte.
   * Constructor de la classe. Permet especificar el nom que rebrà el procés.
   * @param _szProcessName Nom que rebrà el procés.
  **/
  CProcess(const string& _szProcessName):
      m_pCamera(0),m_pSceneEffectManager(0),m_szProcessName(_szProcessName),m_bRenderInfo(true)
                                                                          {};
  /**
   * Mètode d'update.
   * Aquest mètode s'executa a cada iteració. Durant aquest es realitzaran els càlculs de lògica del procés.
   * @param _fElapsedTime És necessari facilitar-hi el temps transcorregut desde l'última crida al mètode.
  **/
	virtual void          Update(float _fElapsedTime) = 0;
  virtual void          RenderINFO(CRenderManager* _pRM) = 0;
  /**
   * Mètode per mostrar informació de debug.
   * Aquest mètode mostrarà per pantalla diversa informació que pot ser d'utilitat per al debug:
   *  - Nom del procés
   *  - FramesPerSecond
  **/
	void				          DebugInformation();
  /**
   * Mètode de pre renderitzat.
   * Aquest mètode s'executa a cada iteració. Es realitzarà el prerender dels efectes d'escena.
   * @param _pRM Render Manager.
  **/
	virtual void          PreRender(CRenderManager* _pRM);
  /**
   * Mètode de render.
   * Aquest mètode s'executa a cada iteració. Es realitzen els següents passos:
   *  - S'activen els efectes d'escena.
   *  - Es renderitza l'escena.
   *  - Es captura el buffer d'escena pels efectes de post-processat.
   *  - Es realitza el post-processat d'efectes d'escena.
   *  - Es captura el buffer de post-processat.
   * @param _pRM Render Manager.
  **/
	virtual void          Render(CRenderManager* _pRM);
  /**
   * Mètode de render d'escena.
   * Aquest mètode s'executa a cada iteració. Durant aquest es realitzarà el pintat de l'escena per pantalla.
   * @param _pRM Render Manager.
  **/
	virtual void          RenderScene(CRenderManager* _pRM) = 0;
  /**
   * Mètode d'inicialització.
   * Aquest mètode inicialitzarà el procés, deixant-lo llest pel seu funcionament.
  **/
	virtual bool          Init() = 0;

  /**
   * Getter de la càmara.
   * Aquest mètode retorna l'actual càmara assignada al procés i sobre la qual es dibuixa l'escena.
   * @return CCamera* que representa l'actual càmara assignada al procés.
  **/
  virtual CCamera*      GetCamera                 ()         {return m_pCamera;};

  /**
   * Setter del SceneEffectManager.
   * Aquest mètode especifica el SceneEffectManager que s'utilitzarà.
   * @param _pSceneEffectManager Punter al SceneEffectManager a utilitzar.
  **/
  virtual void          SetSceneEffectManager     (CSceneEffectManager* _pSceneEffectManager)         { m_pSceneEffectManager = _pSceneEffectManager; };

  /**
   * Mètode d'execució d'accions.
   * Aquest mètode executarà accions concretes del procés durant un temps determinat.
   * @param _fDeltaSeconds Temps de duració de l'acció.
   * @param _fDelta Temps de duració de l'acció. TODO ????
   * @param _pcAction Nom identificatiu de l'acció a realitzar.
   * @return True si l'acció existeix. False si no.
  **/
  bool                  ExecuteAction             (float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  /**
   * Mètode d'execució d'accions scriptades.
   * Aquest mètode executarà accions scriptades que contingui el procés.
   * @param _fDeltaSeconds Temps de duració de l'acció.
   * @param _fDelta Temps de duració de l'acció. TODO ????
   * @param _pcScript Nom identificatiu de l'acció scriptada a realitzar.
   * @return True si l'acció existeix. False si no.
  **/
  bool                  ExecuteScript             (float _fDeltaSeconds, float _fDelta, const char* _pcScript);

  /**
   * TODO
   * Mètode d'execució d'accions.
   * Aquest mètode executarà accions concretes del procés durant un temps determinat.
   * @param _fDeltaSeconds Temps de duració de l'acció.
   * @param _fDelta Temps de duració de l'acció. TODO ????
   * @param _pcAction Nom identificatiu de l'acció a realitzar.
   * @return True si l'acció existeix. False si no.
  **/
  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction) = 0;
  bool getRenderInfo() {return m_bRenderInfo;}

protected:

  /**
   * Punter a càmara.
   * Punter que representa l'actual càmara assignada al procés. L'escena es dibuixarà a partir d'aquesta càmara.
  **/
  CCamera*              m_pCamera;

  /**
   * Punter a Manager d'efectes d'escena.
  **/
  CSceneEffectManager*  m_pSceneEffectManager;

  /**
   * Setter del DebugInfo.
   * Aquest mètode permet determinar si es farà o no el pintat de debug.
   * @param _bRenderInfo Si es vol o no mostrar la informació de debug.
  **/
  virtual void          SetDebugInfo     (bool _bRenderInfo)         { m_bRenderInfo = _bRenderInfo; };
private:

  /**
   * Nom del procés.
  **/
  string                m_szProcessName;
  /**
   * Control de pintat.
   * Variable utilitzada per determinar si s'ha de fer el pintat de debug o no.
  **/
  bool                  m_bRenderInfo;
};

