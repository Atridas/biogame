#ifndef __INPUT_ACTION_H__
#define __INPUT_ACTION_H__
#pragma once

#include "base.h"
#include "Named.h"

//forward declarations -------------
class CXMLTreeNode;
class CInputTrigger;
//----------------------------------

class CInputAction
  : public CNamed
{
public:
  CInputAction() : CNamed(""),m_szScriptAction(""),m_bHasScript(false),m_fDelta(0.0f) {};
  ~CInputAction() {Release();};

  bool Init(CXMLTreeNode* _pTreeNode);

  string& GetScriptAction() {return m_szScriptAction;};
  bool HasScript() {return m_bHasScript;};

  void Update();

  bool IsTriggered() {return m_fDelta != 0;};

  float GetDelta() {return m_fDelta;};

private:

  void Release();

  float m_fDelta;
  string m_szScriptAction;
  bool m_bHasScript;
  vector<CInputTrigger*> m_vInputTriggers;

};

#endif

