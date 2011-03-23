#pragma once

#include <vector>
#include <string>
#include <base.h>

#include <Windows.h>

//---forward-declarations---
class CProcess;
class CCore;
class CRenderManager;
class CTimer;
class CHDRPipeline;

struct SInitParams;
//--------------------------


using namespace std;

typedef vector<CProcess*> VectorProcessPtr;

/**
 * Classe Engine.
 * Classe central de l'Engine. És la que conté a la resta de mòduls i controla els diferents processos.
**/
class CEngine:
  public CBaseControl
{
  
public:
  /**
    * Constructor per defecte.
  **/
	                          CEngine(void):m_pCore(0),m_pHDR(0),m_pActiveProcess(0),m_bExit(false) {};
  /**
   * Destructor.
   * S'encarrega d'alliberar recursos abans de destruir-se.
   * @see Done()
  **/
  virtual					          ~CEngine(void)                                              {Done();};

  /**
   * Mètode d'inicialització de l'engine.
   * Aquest mètode inicialitzarà l'Engine i tots els seus mòduls segons els paràmetres facilitats.
   * @param _InitParams Paràmetres d'inicialització.
   * @param _hWnd Finestra win32 d'execució del programa.
   * @return True si s'ha iniciat correctament. False si no.
   * @see SInitParams
  **/
  bool                      Init					      (const SInitParams& _InitParams, HWND _hWnd);
  
  bool                      GetExit() const     {return m_bExit;};
  void                      SetExit(bool _bExit){m_bExit = _bExit;};

	/**
   * Mètode d'update.
   * Aquest mètode s'executa a cada iteració. Durant aquest es realitzaran els càlculs de lògica de l'engine i es cridarà a l'update del Core del procés actiu.
  **/
  void				              Update				      ();
	/**
   * Mètode de render.
   * Aquest mètode s'executa a cada iteració. Durant aquest es cridarà al render del Core i del procés actiu.
  **/
  void				              Render				      ();
	/**
   * Mètode de sel·lecció de procés.
   * Aquest mètode determinarà el procés que s'ha d'executar en aquell moment.
   * @param _pProcess Procés a executar.
  **/
  void				              AddProcess		      (CProcess* _pProcess);
  
  void				              ActivateProcess		  (int _i);
  void				              ActivateProcess		  (CProcess* _pProcess);

  CProcess*                 GetActiveProcess    () const {return m_pActiveProcess;};

protected:

  /**
   * Mètode d'alliberament de recursos.
   * Aquest mètode alliberarà tots els recursos de l'Engine.
  **/
  void                      Release              ();

private:
  
  void                      RenderHDR            (CRenderManager* _pRM, CProcess* _pProcess);
  void                      RenderNoHDR          (CRenderManager* _pRM, CProcess* _pProcess);
  
  void                      RenderSystems        (CRenderManager* _pRM);
  void                      UpdateSystems        (float _fElapsedTime);

	/**
   * Core.
  **/
  CCore*				            m_pCore;
	/**
   * Timer.
  **/
  CTimer*							      m_pTimer;

  CHDRPipeline*             m_pHDR;
	/**
   * TODO vector de processos.
  **/
  VectorProcessPtr				  m_vProcesses;
  CProcess*                 m_pActiveProcess;

  bool                      m_bExit;

};

