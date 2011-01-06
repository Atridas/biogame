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
struct SInitParams;
//--------------------------


using namespace std;

typedef vector<CProcess*> VectorProcessPtr;

/**
 * Classe Engine.
 * Classe central de l'Engine. �s la que cont� a la resta de m�duls i controla els diferents processos.
**/
class CEngine:
  public CBaseControl
{
  
public:
  /**
    * Constructor per defecte.
  **/
	                          CEngine(void):m_pProcess(0),m_pCore(0)                      {};
  /**
   * Destructor.
   * S'encarrega d'alliberar recursos abans de destruir-se.
   * @see Done()
  **/
  virtual					          ~CEngine(void)                                              {Done();};

  /**
   * M�tode d'inicialitzaci� de l'engine.
   * Aquest m�tode inicialitzar� l'Engine i tots els seus m�duls segons els par�metres facilitats.
   * @param _InitParams Par�metres d'inicialitzaci�.
   * @param _hWnd Finestra win32 d'execuci� del programa.
   * @return True si s'ha iniciat correctament. False si no.
   * @see SInitParams
  **/
  bool                      Init					      (const SInitParams& _InitParams, HWND _hWnd);
	/**
   * M�tode d'update.
   * Aquest m�tode s'executa a cada iteraci�. Durant aquest es realitzaran els c�lculs de l�gica de l'engine i es cridar� a l'update del Core del proc�s actiu.
  **/
  void				              Update				      ();
	/**
   * M�tode de render.
   * Aquest m�tode s'executa a cada iteraci�. Durant aquest es cridar� al render del Core i del proc�s actiu.
  **/
  void				              Render				      ();
	/**
   * M�tode de render del proc�s actiu.
   * Aquest m�tode cridar� al render del proc�s actiu i s'encarregar� de cridar el seu DebugInformation.
  **/
  void				              RenderScene		      ();
	/**
   * M�tode de sel�lecci� de proc�s.
   * Aquest m�tode determinar� el proc�s que s'ha d'executar en aquell moment.
   * @param _pProcess Proc�s a executar.
  **/
  void				              SetProcess		      (CProcess* _pProcess);

private:

  /**
   * M�tode d'alliberament de recursos.
   * Aquest m�tode alliberar� tots els recursos de l'Engine.
  **/
  void                      Release              ();



	/**
   * Proc�s en execuci�.
   * TODO temporal, s'hauria d'utilitzar m_vProcesses.
  **/
  CProcess*			            m_pProcess;
	/**
   * Core.
  **/
  CCore*				            m_pCore;
	/**
   * Timer.
  **/
  CTimer*							      m_pTimer;
	/**
   * TODO vector de processos.
  **/
  VectorProcessPtr				  m_vProcesses;
};

