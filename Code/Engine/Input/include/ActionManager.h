#ifndef __ACTION_MANAGER_H__
#define __ACTION_MANAGER_H__
#pragma once

#include "base.h"
#include <Utils/MapManager.h>
#include "InputAction.h"

//forward declarations -------------
class CProcess;
//----------------------------------

class CActionManager 
  : public CMapManager<CInputAction>
{
public:
  CActionManager() : m_pProcess(0),m_szXMLFile(""),m_bReload(false) {};
  ~CActionManager() {Done();};

  bool Init(string _szXMLFile);

  void SetProcess(CProcess* _pProcess) {m_pProcess = _pProcess;};

  void Update(float _fDeltaSeconds);

  void Reload() {m_bReload = true;};

  virtual void Release();

private:

  void ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  void ExecuteScript(float _fDeltaSeconds, float _fDelta, const char* _pcScript);

  bool m_bReload;
  string m_szXMLFile;
  CProcess* m_pProcess;
};

#endif