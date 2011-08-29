#pragma once

#include <Process.h>

//Forward declarations---------------------
class CRenderManager;
class CCamera;
class CObject3D;
class COmniLight;
class CEmiterInstance;
//-----------------------------------------

class CBiotestProcess :
	public CProcess
{
public:
                        CBiotestProcess      ():CProcess("CBiotestProcess"),m_pOmniLight(0)
                                                                         {};
  virtual               ~CBiotestProcess     ()                          {Done();};

  virtual bool          Init              ();
  void                  Update            (float elT);
  void                  RenderScene       (CRenderManager* _pRM);
  void                  RenderINFO        (CRenderManager* _pRM);
  bool                  ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
private:
  void                  Release           ();

  COmniLight* m_pOmniLight;

  CEmiterInstance* m_pEmiter;
};

