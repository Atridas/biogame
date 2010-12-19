#pragma once
#include <base.h>

class CCamera;

class CProcess:
  public CBaseControl
{

public:
                        CProcess(const string& _szProcessName):m_pCamera(0),m_szProcessName(_szProcessName),m_bRenderInfo(false)
                                                                          {};
  
	virtual void          Update(float _fElapsedTime) = 0;
	void				          DebugInformation(float FPS);
	virtual void          Render() = 0;
	virtual bool          Init() = 0;

  virtual CCamera*      GetCamera                 ()         {return m_pCamera;};

  bool                  ExecuteAction             (float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  bool                  ExecuteScript             (float _fDeltaSeconds, float _fDelta, const char* _pcScript);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction) = 0;

protected:

  CCamera*              m_pCamera;

private:

  string                m_szProcessName;
  bool                  m_bRenderInfo;
};

