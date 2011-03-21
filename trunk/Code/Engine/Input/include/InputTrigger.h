#ifndef __INPUT_TRIGGER_H__
#define __INPUT_TRIGGER_H__
#pragma once

#include "base.h"
#include "InputDefs.h"

//forward declarations -------------
class CInputManager;
class CXMLTreeNode;
//----------------------------------

class CInputTrigger
{
public:
  CInputTrigger() : m_fDelta(0.0f) {};
  ~CInputTrigger() {};

  bool Init(CXMLTreeNode* _pTreeNode);

  void Update();
  float GetDelta() {return m_fDelta;};
  bool IsTriggered() {return m_fDelta != 0;};

private:

  INPUT_DEVICE_TYPE m_Device;
  INPUT_AXIS_TYPE   m_Axis;
  INPUT_EVENT_TYPE  m_EventType;
  uint32            m_uiCode;
  float             m_fDelta;

  //float m_fDelta;
  CInputManager* m_pInputManager;
};

#endif

