#pragma once

#include <Process.h>

#include "NivellInicial.h"

//Forward declarations---------------------
class CRenderManager;
class CCamera;
class CObject3D;
class COmniLight;
//-----------------------------------------

class CGameProcess :
	public CProcess
{
public:
                        CGameProcess      ():CProcess("CGameProcess"),m_pOmniLight(0),m_bMenuPrincipal(false)
                                                                      {};
  virtual               ~CGameProcess     ()                          {Done();};

  virtual bool          Init              ();
  void                  Update            (float elT);
  //void                  RenderScene       (CRenderManager* _pRM);
  void                  RenderINFO        (CRenderManager* _pRM);
  bool                  ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
private:
  void                  Release           ();

  COmniLight* m_pOmniLight;
  bool        m_bMenuPrincipal;
};

