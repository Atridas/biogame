#pragma once

#include <Process.h>

//Forward declarations---------------------
class CRenderManager;
//-----------------------------------------

class CGameProcess :
	public CProcess
{
public:
                        CGameProcess      ():CProcess("CGameProcess") {};
  virtual               ~CGameProcess     ()                          {Done();};

  virtual bool          Init              ();
  void                  Update            (float elT);
  void                  RenderScene       (CRenderManager* _pRM);
  void                  RenderINFO        (CRenderManager* _pRM) {};
  bool                  ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
private:
  void                  Release           ();
};

