#pragma once

#include <Process.h>

class CGameProcess :
	public CProcess
{
public:
                        CGameProcess      ():CProcess("CGameProcess") {};
  virtual               ~CGameProcess     ()                          {Done();};

  virtual bool          Init              ();
	void                  Update            (float elT);
	void                  Render            ();
	bool				ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const string& _szAction);
private:
  void                  Release           ();
};

