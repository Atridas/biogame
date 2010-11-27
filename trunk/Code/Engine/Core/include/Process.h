#pragma once
#include <base.h>

class CCamera;

class CProcess:
  public CBaseControl
{

public:
                        CProcess():m_pCamera(0)              {};
  
	virtual void          Update(float _fElapsedTime) = 0;
	void				          DebugInformation(float FPS);
	virtual void          Render() = 0;
	virtual bool          Init() = 0;

  virtual CCamera*      GetCamera                 ()         {return m_pCamera;};

  virtual bool          ExecuteAction             (float _fDeltaSeconds, float _fDelta, const string& _szAction);
  virtual bool          ExecuteScript             (float _fDeltaSeconds, float _fDelta, const string& _szScript);

protected:

  CCamera*              m_pCamera;
};

