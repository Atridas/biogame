#pragma once
#include <base.h>

class CProcess:
  public CBaseControl
{

public:
                        CProcess()              {};
  
	virtual void          Update() = 0;
	virtual void          Render() = 0;
	virtual bool          Init() = 0;


};

