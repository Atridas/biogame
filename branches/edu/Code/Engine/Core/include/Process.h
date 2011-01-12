#pragma once
#include <base.h>

class CCamera;

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
                        CProcess(const string& _szProcessName):m_pCamera(0),m_szProcessName(_szProcessName),m_bRenderInfo(false)
                                                                          {};
  /**
   * Mètode d'update.
   * Aquest mètode s'executa a cada iteració. Durant aquest es realitzaran els càlculs de lògica del procés.
   * @param _fElapsedTime És necessari facilitar-hi el temps transcorregut desde l'última crida al mètode.
  **/
	virtual void          Update(float _fElapsedTime) = 0;
  /**
   * Mètode per mostrar informació de debug.
   * Aquest mètode mostrarà per pantalla diversa informació que pot ser d'utilitat per al debug:
   *  - Nom del procés
   *  - FramesPerSecond
   * @param _fFPS Valor dels FPS actuals. Aquest valor serà imprés per pantalla.
  **/
	void				          DebugInformation(float _fFPS);
  /**
   * Mètode de render.
   * Aquest mètode s'executa a cada iteració. Durant aquest es realitzarà el pintat de l'escena per pantalla.
  **/
	virtual void          Render() = 0;
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

protected:

  /**
   * Punter a càmara.
   * Punter que representa l'actual càmara assignada al procés. L'escena es dibuixarà a partir d'aquesta càmara.
  **/
  CCamera*              m_pCamera;

private:

  /**
   * Nom del procés.
  **/
  string                m_szProcessName;
  /**
   * Control de pintat.
   * Variable utilitzada per comprobar el correcte funcionament del pintat.
  **/
  bool                  m_bRenderInfo;
};

