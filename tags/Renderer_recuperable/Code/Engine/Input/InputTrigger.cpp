#include "InputTrigger.h"
#include "Core.h"
#include "InputManager.h"
#include <XML/XMLTreeNode.h>

bool CInputTrigger::Init(CXMLTreeNode* _pTreeNode)
{
  m_pInputManager = INPUT_MANAGER;

  if(!m_pInputManager)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CInputTrigger::Init NULL InputManager");
    return false;
  }

  if(strcmp(_pTreeNode->GetName(),"input") != 0)
  {
    LOGGER->AddNewLog(ELL_WARNING,"CInputTrigger::Init Error de format a l'xml, hi ha un element invàlid \"%s\"",_pTreeNode->GetName());
    return false;
  }

  string l_szDevice = _pTreeNode->GetPszISOProperty("deviceType","IDV_NOTHING", false);
  string l_szAxis   = _pTreeNode->GetPszISOProperty("AxisType","AXIS_NOTHING", false);
  string l_szEvent  = _pTreeNode->GetPszISOProperty("EventType","EVENT_NOTHING", false);
  string l_szCode   = _pTreeNode->GetPszISOProperty("Code","KEY_NULL", false);
  float  l_fDelta   = _pTreeNode->GetFloatProperty("Delta",1.0f, false);

  //TODO comprovar errors, accedir via "get"
  m_Device = (INPUT_DEVICE_TYPE) m_pInputManager->GetCode(l_szDevice);
  m_Axis = (INPUT_AXIS_TYPE) m_pInputManager->GetCode(l_szAxis);
  m_EventType = (INPUT_EVENT_TYPE)  m_pInputManager->GetCode(l_szEvent);
  m_uiCode = m_pInputManager->GetCode(l_szCode);
  m_fDelta = l_fDelta;
          
  LOGGER->AddNewLog(ELL_INFORMATION,"CInputTrigger::Init   Device %s, Axis %s, Event %s, Code %s, Delta %f",
                                          l_szDevice.c_str(),
                                          l_szAxis.c_str(),
                                          l_szEvent.c_str(),
                                          l_szCode.c_str(),
                                          l_fDelta);

  if(m_Axis == AXIS_NOTHING && m_EventType == EVENT_NOTHING)
  {
    LOGGER->AddNewLog(ELL_WARNING,"CInputTrigger::Init trigger sense axis ni event.");
    return false;
  }

  return true;
}

void CInputTrigger::Update()
{
  float x, y;
  
  m_fDelta = 0.f;

  switch(m_Axis)
  {
  //Moviment del mouse ---------------------------------
  case AXIS_MOUSE_X:
    m_fDelta = (float)m_pInputManager->GetMouseDelta().x;
    break;
  case AXIS_MOUSE_Y:
    m_fDelta = (float)m_pInputManager->GetMouseDelta().y;
    break;
  case AXIS_MOUSE_Z:
    m_fDelta = (float)m_pInputManager->GetMouseDelta().z;
    break;
  //Polzes del gamepad
  case AXIS_LEFT_THUMB_X:
    if(!m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y,m_Device))
    {
      //TODO error? log?
    }// else {
      m_fDelta = x;
    //}
    break;
  case AXIS_LEFT_THUMB_Y:
    m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y,m_Device);
    m_fDelta = y;
    break;


  case AXIS_RIGHT_THUMB_X:
    m_pInputManager->GetGamePadRightThumbDeflection(&x,&y,m_Device);
    m_fDelta = x;
    break;
  case AXIS_RIGHT_THUMB_Y:
    m_pInputManager->GetGamePadRightThumbDeflection(&x,&y,m_Device);
    m_fDelta = y;
    break;

    
  case AXIS_DELTA_TRIGGER_LEFT:
    m_pInputManager->GetGamePadDeltaTriggers(&x,&y,m_Device);
    m_fDelta = x;
    break;
  case AXIS_DELTA_TRIGGER_RIGHT:
    m_pInputManager->GetGamePadDeltaTriggers(&x,&y,m_Device);
    m_fDelta = y;
    break;
  //Tecles ---------------------------------------------
  case AXIS_NOTHING:
    switch(m_EventType)
    {
    case EVENT_DOWN:
      m_fDelta = (m_pInputManager->IsDown(m_Device,m_uiCode)? 1.f:0.f);
      break;
    case EVENT_UP_DOWN:
      m_fDelta = (m_pInputManager->IsUpDown(m_Device,m_uiCode)? 1.f:0.f);
      break;
    case EVENT_DOWN_UP:
      m_fDelta = (m_pInputManager->IsDownUp(m_Device,m_uiCode)? 1.f:0.f);
      break;
    case EVENT_NOTHING:
    default:
      //TODO error?
      break;
    }
    break;
  }
}