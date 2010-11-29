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
private:
  void                  Release           ();
};

