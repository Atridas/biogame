#pragma once

#include <Process.h>

//Forward declarations---------------------
class CRenderManager;
class CCamera;
class CObject3D;
//-----------------------------------------

class CGameProcess :
	public CProcess
{
public:
                        CGameProcess      ():CProcess("CGameProcess"),
                                             m_pObjectCamera(0),
                                             m_pRiggleObject3D(0)
                                                                      {};
  virtual               ~CGameProcess     ()                          {Done();};

  virtual bool          Init              ();
  void                  Update            (float elT);
  void                  RenderScene       (CRenderManager* _pRM);
  void                  RenderINFO        (CRenderManager* _pRM) {};
  bool                  ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
private:
  void                  Release           ();

  CCamera*              m_pObjectCamera; //default camera
  CObject3D*            m_pRiggleObject3D;
};

