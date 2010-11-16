#pragma once

#include <Process.h>

class CGameProcess :
	public CProcess
{
public:
                        CGameProcess      ()                        {};
  virtual               ~CGameProcess     ()                        {Done();};

  virtual bool          Init              ();
	void                  Update            ();
	void                  Render            ();
private:
  void                  Relase            ();
};

