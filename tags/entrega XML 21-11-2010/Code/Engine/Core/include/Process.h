#pragma once
#include <base.h>

class CProcess:
  public CBaseControl
{

public:
                        CProcess()              {};
  
	virtual void          Update(float _fElapsedTime) = 0;
	void				          DebugInformation(float FPS);
	virtual void          Render() = 0;
	virtual bool          Init() = 0;


};

