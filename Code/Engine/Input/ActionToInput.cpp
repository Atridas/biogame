#include "ActionToInput.h"

#include <XML/XMLTreeNode.h>
#include "InputManager.h"
#include "Process.h"

#define HARDCODED_ACTION_PROPERTY "name"
#define SCRIPTED_ACTION_PROPERTY  "script"
#define ACTION_ELEMENT            "action"
#define INPUT_ELEMENT             "input"

#define DEVICE_PROPERTY           "deviceType"
#define AXIS_PROPERTY             "AxisType"
#define EVENT_PROPERTY            "EventType"
#define DELTA_PROPERTY            "Delta"
#define CODE_PROPERTY             "Code"

#define DEVICE_DEFAULT            "IDV_NOTHING"
#define AXIS_DEFAULT              "AXIS_NOTHING"
#define EVENT_DEFAULT             "EVENT_NOTHING"
#define DELTA_DEFAULT             1.0f
#define CODE_DEFAULT              "KEY_NULL"

struct SActionTuple {
  float m_fDelta;
  const char* m_pcAction;
  const char* m_pcScript;

  SActionTuple(float _fDelta, const char* _pcAction, const char* _pcScript):
  m_fDelta(m_fDelta), m_pcAction(_pcAction), m_pcScript(_pcScript)
  {};
};


bool CActionToInput::Init(CInputManager* _pInputManager, const char* _pcXMLFile)
{
  m_pInputManager = _pInputManager;
  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::Init");
  m_szXMLFile = _pcXMLFile;
  InitString2Input();
  m_pActionToTriggers = new TActionToTriggers;
  Load();

  SetOk(true);
  return IsOk();
}

void CActionToInput::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::Release");
  Unload();
  CHECKED_DELETE(m_pActionToTriggers);
}

void CActionToInput::Load()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::Load");

  CXMLTreeNode l_XMLActions;
  if(!l_XMLActions.LoadFile(m_szXMLFile.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CActionToInput:: No s'ha trobat el XML \"%s\"", m_szXMLFile);
  }
  else
  {
    int l_iNumActions = l_XMLActions.GetNumChildren();
    for(int i = 0; i < l_iNumActions; i++)
    {
      CXMLTreeNode l_XMLAction = l_XMLActions(i);
      if(strcmp(l_XMLAction.GetName(),ACTION_ELEMENT) != 0)
      {
        if(!l_XMLAction.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING,"CActionToInput:: Error de format a l'xml, hi ha un element invàlid \"%s\"", l_XMLAction.GetName());
        }
      } else {
        //agafa l'acció
        SAction l_Action;
        l_Action.hardCodedAction = l_XMLAction.GetPszProperty(HARDCODED_ACTION_PROPERTY ,"");
        l_Action.script          = l_XMLAction.GetPszProperty(SCRIPTED_ACTION_PROPERTY  ,"");
        if(l_Action.hardCodedAction == "" && l_Action.script == "")
        {
          LOGGER->AddNewLog(ELL_WARNING,"CActionToInput:: Hi ha una acció sense script ni res assignat. La nº%d", i);
        } else {

          LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::  Action \"%s\" script \"%s\"",l_Action.hardCodedAction.c_str(),l_Action.script.c_str());

          int l_iNumTriggers = l_XMLAction.GetNumChildren();
          TInputTriggers* l_Triggers = new TInputTriggers(l_iNumTriggers);
          for(int j = 0; j < l_iNumTriggers; j++)
          {
            CXMLTreeNode l_XMLInput = l_XMLAction(j);
            if(strcmp(l_XMLInput.GetName(),INPUT_ELEMENT) == 0)
            {
              SInputInfo l_Trigger;
              string l_szDevice = l_XMLInput.GetPszProperty  (DEVICE_PROPERTY, DEVICE_DEFAULT);
              string l_szAxis   = l_XMLInput.GetPszProperty  (AXIS_PROPERTY,   AXIS_DEFAULT  );
              string l_szEvent  = l_XMLInput.GetPszProperty  (EVENT_PROPERTY,  EVENT_DEFAULT );
              string l_szCode   = l_XMLInput.GetPszProperty  (CODE_PROPERTY,   CODE_DEFAULT  );
              float  l_fDelta   = l_XMLInput.GetFloatProperty(DELTA_PROPERTY,  DELTA_DEFAULT );

              //TODO comprovar errors, accedir via "get"
              l_Trigger.device    = (INPUT_DEVICE_TYPE) m_String2Code[l_szDevice];
              l_Trigger.axis      = (INPUT_AXIS_TYPE)   m_String2Code[l_szAxis  ];
              l_Trigger.eventType = (INPUT_EVENT_TYPE)  m_String2Code[l_szEvent ];
              l_Trigger.code      =                     m_String2Code[l_szCode  ];
              l_Trigger.delta     = l_fDelta;

              (*l_Triggers)[j] = l_Trigger;
              
              LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::   Device %s, Axis %s, Event %s, Code %s, Delta %f",
                                                      l_szDevice.c_str(),
                                                      l_szAxis.c_str(),
                                                      l_szEvent.c_str(),
                                                      l_szCode.c_str(),
                                                      l_fDelta);
              if(l_Trigger.axis == AXIS_NOTHING && l_Trigger.eventType == EVENT_NOTHING)
              {
                LOGGER->AddNewLog(ELL_WARNING,"CActionToInput:: trigger sense axis ni event!");
              }
            } else {
              LOGGER->AddNewLog(ELL_WARNING,"CActionToInput:: Error de format a l'xml, hi ha un element invàlid \"%s\"", l_XMLInput.GetName());
            }
          }
          m_pActionToTriggers->insert(TActionToTriggersPair(l_Action,l_Triggers));
        }
      }
    }
  }
}

void CActionToInput::Unload()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::Unload");
  TActionToTriggersIterator l_End = m_pActionToTriggers->end();
  for(TActionToTriggersIterator l_It = m_pActionToTriggers->begin(); l_It != l_End; ++l_It)
  {
    TInputTriggers* l_Triggers = l_It->second;
    delete l_Triggers;
  }
  m_pActionToTriggers->clear();
}

void CActionToInput::Update(float _fDeltaSeconds)
{
  TActionToTriggersIterator l_End = m_pActionToTriggers->end();
  static vector<SActionTuple> actions;
  actions.clear();
  //per totes les accions
  for(TActionToTriggersIterator l_It = m_pActionToTriggers->begin(); l_It != l_End; ++l_It)
  {
    TInputTriggers* l_Triggers = l_It->second;
    TInputTriggersIterator l_EndTriggers = l_Triggers->end();
    //per tots els triggers
    float l_fDelta = 1.0f;
    for(TInputTriggersIterator l_ItTriggers = l_Triggers->begin(); l_ItTriggers != l_EndTriggers; ++l_ItTriggers)
    {
      float l_Res = IsTriggered(*l_ItTriggers);
      if(l_Res == 0.f) 
      {
        l_fDelta = 0.f;
        break;
      } else {
        l_fDelta *= l_Res;
      }
    }
    if(l_fDelta != 0.f)
    {
      const SAction& l_Action = l_It->first;
      /*if(l_Action.hardCodedAction != "")
        ExecuteAction(_fDeltaSeconds,l_fDelta,l_Action.hardCodedAction);
      if(l_Action.script != "")
        ExecuteAction(_fDeltaSeconds,l_fDelta,l_Action.script);*/
      actions.push_back(SActionTuple(l_fDelta,l_Action.hardCodedAction.c_str(),l_Action.script.c_str()));
    }
  }

  vector<SActionTuple>::iterator l_enditActionsExecuted = actions.end();
  for(vector<SActionTuple>::iterator l_itActionsExecuted = actions.begin();l_itActionsExecuted!=l_enditActionsExecuted;++l_itActionsExecuted)
  {
    SActionTuple& l_Action = *l_itActionsExecuted;
    if(strlen(l_Action.m_pcAction) > 0)
      ExecuteAction(_fDeltaSeconds,l_Action.m_fDelta,l_Action.m_pcAction);
    if(strlen(l_Action.m_pcScript) > 0)
      ExecuteAction(_fDeltaSeconds,l_Action.m_fDelta,l_Action.m_pcScript);
  }

}

float CActionToInput::IsTriggered(const SInputInfo& _Trigger)
{
  float x, y;
  switch(_Trigger.axis)
  {
  //Moviment del mouse ---------------------------------
  case AXIS_MOUSE_X:
    return (float)m_pInputManager->GetMouseDelta().x;
    break;
  case AXIS_MOUSE_Y:
    return (float)m_pInputManager->GetMouseDelta().y;
    break;
  case AXIS_MOUSE_Z:
    return (float)m_pInputManager->GetMouseDelta().z;
    break;
  //Polzes del gamepad
  case AXIS_LEFT_THUMB_X:
    if(!m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y,_Trigger.device))
    {
      //TODO error? log?
    }// else {
      return x;
    //}
    break;
  case AXIS_LEFT_THUMB_Y:
    m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y,_Trigger.device);
    return y;
    break;


  case AXIS_RIGHT_THUMB_X:
    m_pInputManager->GetGamePadRightThumbDeflection(&x,&y,_Trigger.device);
    return x;
    break;
  case AXIS_RIGHT_THUMB_Y:
    m_pInputManager->GetGamePadRightThumbDeflection(&x,&y,_Trigger.device);
    return y;
    break;

    
  case AXIS_DELTA_TRIGGER_LEFT:
    m_pInputManager->GetGamePadDeltaTriggers(&x,&y,_Trigger.device);
    return x;
    break;
  case AXIS_DELTA_TRIGGER_RIGHT:
    m_pInputManager->GetGamePadDeltaTriggers(&x,&y,_Trigger.device);
    return y;
    break;
  //Tecles ---------------------------------------------
  case AXIS_NOTHING:
    switch(_Trigger.eventType)
    {
    case EVENT_DOWN:
      return (m_pInputManager->IsDown(_Trigger.device,_Trigger.code)? 1.f:0.f);
      break;
    case EVENT_UP_DOWN:
      return (m_pInputManager->IsUpDown(_Trigger.device,_Trigger.code)? 1.f:0.f);
      break;
    case EVENT_DOWN_UP:
      return (m_pInputManager->IsDownUp(_Trigger.device,_Trigger.code)? 1.f:0.f);
      break;
    case EVENT_NOTHING:
    default:
      //TODO error?
      break;
    }
    break;
  }
  return 0.f;
}

void CActionToInput::ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(m_pProcess)
  {
    if(m_pProcess->ExecuteAction(_fDeltaSeconds, _fDelta, _pcAction))
      return;
  }
  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput:: Action \"%s\" delta[%f] seconds[%f]", _pcAction, _fDelta, _fDeltaSeconds);
}

void CActionToInput::ExecuteScript(float _fDeltaSeconds, float _fDelta, const char* _pcScript)
{
  if(m_pProcess)
  {
    if(m_pProcess->ExecuteScript(_fDeltaSeconds, _fDelta, _pcScript))
      return;
  }
  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput:: Script \"%s\" delta[%f] seconds[%f]", _pcScript, _fDelta, _fDeltaSeconds);
}


void CActionToInput::InitString2Input ()
{
  typedef std::pair<std::string, uint32> PairString2Code;

  //----------------INPUT_DEVICE_TYPE---------------//
  m_String2Code.insert( PairString2Code("IDV_KEYBOARD", IDV_KEYBOARD) );
  m_String2Code.insert( PairString2Code("IDV_MOUSE", IDV_MOUSE) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD1", IDV_GAMEPAD1) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD2", IDV_GAMEPAD2) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD3", IDV_GAMEPAD3) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD4", IDV_GAMEPAD4) );
  m_String2Code.insert( PairString2Code("IDV_NOTHING", IDV_NOTHING) );

  //----------------INPUT_AXIS_TYPE-----------------//
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_X", AXIS_MOUSE_X) );
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_Y", AXIS_MOUSE_Y) );
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_Z", AXIS_MOUSE_Z) );
  m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_X", AXIS_LEFT_THUMB_X) );
  m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_Y", AXIS_LEFT_THUMB_Y) );
  m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_X", AXIS_RIGHT_THUMB_X) );
  m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_Y", AXIS_RIGHT_THUMB_Y) );
  m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_RIGHT", AXIS_DELTA_TRIGGER_RIGHT) );
  m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_LEFT", AXIS_DELTA_TRIGGER_LEFT) );
  m_String2Code.insert( PairString2Code("AXIS_NOTHING", AXIS_NOTHING) );

  //----------------INPUT_EVENT_TYPE-----------------//
  m_String2Code.insert( PairString2Code("EVENT_DOWN", EVENT_DOWN) );
  m_String2Code.insert( PairString2Code("EVENT_UP_DOWN", EVENT_UP_DOWN) );
  m_String2Code.insert( PairString2Code("EVENT_DOWN_UP", EVENT_DOWN_UP) );
  m_String2Code.insert( PairString2Code("EVENT_NOTHING", EVENT_NOTHING) );

  //----------------MOUSE_BUTTON---------------------//
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_NOTHING", MOUSE_BUTTON_NOTHING) );


  //----------------Gamepad codes from XInput.h------//
  m_String2Code.insert( PairString2Code("PAD_DPAD_UP", PAD_DPAD_UP) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN", PAD_DPAD_DOWN) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_LEFT", PAD_DPAD_LEFT) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_RIGHT", PAD_DPAD_RIGHT) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_START", PAD_DPAD_START) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_BACK", PAD_DPAD_BACK) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB", PAD_BUTTON_LEFT_THUMB) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB", PAD_BUTTON_RIGHT_THUMB) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER", PAD_BUTTON_LEFT_SHOULDER) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER",PAD_BUTTON_RIGHT_SHOULDER) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_A", PAD_BUTTON_A) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_B", PAD_BUTTON_B) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_X", PAD_BUTTON_X) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_Y", PAD_BUTTON_Y) );

  //-----------Keyboard scan codes from dinput.h ------//
  m_String2Code.insert( PairString2Code("KEY_ESCAPE", KEY_ESCAPE) );
  m_String2Code.insert( PairString2Code("KEY_TAB", KEY_TAB) );
  m_String2Code.insert( PairString2Code("KEY_SPACE", KEY_SPACE) );
  m_String2Code.insert( PairString2Code("KEY_RETURN", KEY_RETURN) );
  m_String2Code.insert( PairString2Code("KEY_BACK", KEY_BACK) );
  m_String2Code.insert( PairString2Code("KEY_CAPITAL", KEY_CAPITAL) );
  m_String2Code.insert( PairString2Code("KEY_MINUS", KEY_MINUS) );
  m_String2Code.insert( PairString2Code("KEY_EQUALS", KEY_EQUALS) );
  m_String2Code.insert( PairString2Code("KEY_LBRACKET", KEY_LBRACKET) );
  m_String2Code.insert( PairString2Code("KEY_RBRACKET", KEY_RBRACKET) );
  m_String2Code.insert( PairString2Code("KEY_SEMICOLON", KEY_SEMICOLON) );
  m_String2Code.insert( PairString2Code("KEY_APOSTROPHE", KEY_APOSTROPHE) );
  m_String2Code.insert( PairString2Code("KEY_GRAVE", KEY_GRAVE) );
  m_String2Code.insert( PairString2Code("KEY_BACKSLASH", KEY_BACKSLASH) );
  m_String2Code.insert( PairString2Code("KEY_COMMA", KEY_COMMA) );
  m_String2Code.insert( PairString2Code("KEY_PERIOD", KEY_PERIOD) );
  m_String2Code.insert( PairString2Code("KEY_SLASH", KEY_SLASH) );
  m_String2Code.insert( PairString2Code("KEY_A", KEY_A) );
  m_String2Code.insert( PairString2Code("KEY_S", KEY_S) );
  m_String2Code.insert( PairString2Code("KEY_D", KEY_D) );
  m_String2Code.insert( PairString2Code("KEY_F", KEY_F) );
  m_String2Code.insert( PairString2Code("KEY_G", KEY_G) );
  m_String2Code.insert( PairString2Code("KEY_H", KEY_H) );
  m_String2Code.insert( PairString2Code("KEY_J", KEY_J) );
  m_String2Code.insert( PairString2Code("KEY_K", KEY_K) );
  m_String2Code.insert( PairString2Code("KEY_L", KEY_L) );
  m_String2Code.insert( PairString2Code("KEY_X", KEY_X) );
  m_String2Code.insert( PairString2Code("KEY_C", KEY_C) );
  m_String2Code.insert( PairString2Code("KEY_B", KEY_B) );
  m_String2Code.insert( PairString2Code("KEY_N", KEY_N) );
  m_String2Code.insert( PairString2Code("KEY_M", KEY_M) );
  m_String2Code.insert( PairString2Code("KEY_Q", KEY_Q) );
  m_String2Code.insert( PairString2Code("KEY_W", KEY_W) );
  m_String2Code.insert( PairString2Code("KEY_E", KEY_E) );
  m_String2Code.insert( PairString2Code("KEY_R", KEY_R) );
  m_String2Code.insert( PairString2Code("KEY_T", KEY_T) );
  m_String2Code.insert( PairString2Code("KEY_Y", KEY_Y) );
  m_String2Code.insert( PairString2Code("KEY_U", KEY_U) );
  m_String2Code.insert( PairString2Code("KEY_I", KEY_I) );
  m_String2Code.insert( PairString2Code("KEY_O", KEY_O) );
  m_String2Code.insert( PairString2Code("KEY_P", KEY_P) );
  m_String2Code.insert( PairString2Code("KEY_1", KEY_1) );
  m_String2Code.insert( PairString2Code("KEY_2", KEY_2) );
  m_String2Code.insert( PairString2Code("KEY_3", KEY_3) );
  m_String2Code.insert( PairString2Code("KEY_4", KEY_4) );
  m_String2Code.insert( PairString2Code("KEY_5", KEY_5) );
  m_String2Code.insert( PairString2Code("KEY_6", KEY_6) );
  m_String2Code.insert( PairString2Code("KEY_7", KEY_7) );
  m_String2Code.insert( PairString2Code("KEY_8", KEY_8) );
  m_String2Code.insert( PairString2Code("KEY_9", KEY_9) );
  m_String2Code.insert( PairString2Code("KEY_0", KEY_0) );
  m_String2Code.insert( PairString2Code("KEY_F1", KEY_F1) );
  m_String2Code.insert( PairString2Code("KEY_F2", KEY_F2) );
  m_String2Code.insert( PairString2Code("KEY_F3", KEY_F3) );
  m_String2Code.insert( PairString2Code("KEY_F4", KEY_F4) );
  m_String2Code.insert( PairString2Code("KEY_F5", KEY_F5) );
  m_String2Code.insert( PairString2Code("KEY_F6", KEY_F6) );
  m_String2Code.insert( PairString2Code("KEY_F7", KEY_F7) );
  m_String2Code.insert( PairString2Code("KEY_F8", KEY_F8) );
  m_String2Code.insert( PairString2Code("KEY_F9", KEY_F9) );
  m_String2Code.insert( PairString2Code("KEY_F10", KEY_F10) );
  m_String2Code.insert( PairString2Code("KEY_F11", KEY_F11) );
  m_String2Code.insert( PairString2Code("KEY_F12", KEY_F12) );
  m_String2Code.insert( PairString2Code("KEY_NPPLUS", KEY_NPPLUS) );
  m_String2Code.insert( PairString2Code("KEY_NPMINUS", KEY_NPMINUS) );
  m_String2Code.insert( PairString2Code("KEY_NPDECIMAL", KEY_NPDECIMAL) );
  m_String2Code.insert( PairString2Code("KEY_NPCOMMA", KEY_NPCOMMA) );
  m_String2Code.insert( PairString2Code("KEY_NPDIVIDE", KEY_NPDIVIDE) );
  m_String2Code.insert( PairString2Code("KEY_NPMULTIPLY", KEY_NPMULTIPLY) );
  m_String2Code.insert( PairString2Code("KEY_NPENTER", KEY_NPENTER) );
  m_String2Code.insert( PairString2Code("KEY_NUMLOCK", KEY_NUMLOCK) );
  m_String2Code.insert( PairString2Code("KEY_NP1", KEY_NP1) );
  m_String2Code.insert( PairString2Code("KEY_NP2", KEY_NP2) );
  m_String2Code.insert( PairString2Code("KEY_NP3", KEY_NP3) );
  m_String2Code.insert( PairString2Code("KEY_NP4", KEY_NP4) );
  m_String2Code.insert( PairString2Code("KEY_NP5", KEY_NP5) );
  m_String2Code.insert( PairString2Code("KEY_NP6", KEY_NP6) );
  m_String2Code.insert( PairString2Code("KEY_NP7", KEY_NP7) );
  m_String2Code.insert( PairString2Code("KEY_NP8", KEY_NP8) );
  m_String2Code.insert( PairString2Code("KEY_NP9", KEY_NP9) );
  m_String2Code.insert( PairString2Code("KEY_NP0", KEY_NP0) );
  m_String2Code.insert( PairString2Code("KEY_RSHIFT", KEY_RSHIFT) );
  m_String2Code.insert( PairString2Code("KEY_LSHIFT", KEY_LSHIFT) );
  m_String2Code.insert( PairString2Code("KEY_RCTRL", KEY_RCTRL) );
  m_String2Code.insert( PairString2Code("KEY_LCTRL", KEY_LCTRL) );
  m_String2Code.insert( PairString2Code("KEY_RALT", KEY_RALT) );
  m_String2Code.insert( PairString2Code("KEY_LALT", KEY_LALT) );
  m_String2Code.insert( PairString2Code("KEY_LWIN", KEY_LWIN) );
  m_String2Code.insert( PairString2Code("KEY_RWIN", KEY_RWIN) );
  m_String2Code.insert( PairString2Code("KEY_UP", KEY_UP) );
  m_String2Code.insert( PairString2Code("KEY_DOWN", KEY_DOWN) );
  m_String2Code.insert( PairString2Code("KEY_LEFT", KEY_LEFT) );
  m_String2Code.insert( PairString2Code("KEY_RIGHT", KEY_RIGHT) );
  m_String2Code.insert( PairString2Code("KEY_INSERT", KEY_INSERT) );
  m_String2Code.insert( PairString2Code("KEY_DELETE", KEY_DELETE) );
  m_String2Code.insert( PairString2Code("KEY_HOME", KEY_HOME) );
  m_String2Code.insert( PairString2Code("KEY_END", KEY_END) );
  m_String2Code.insert( PairString2Code("KEY_PGDOWN", KEY_PGDOWN) );
  m_String2Code.insert( PairString2Code("KEY_PGUP", KEY_PGUP) );
  m_String2Code.insert( PairString2Code("KEY_PAUSE", KEY_PAUSE) );
  m_String2Code.insert( PairString2Code("KEY_SCROLL", KEY_SCROLL) );
  m_String2Code.insert( PairString2Code("KEY_NULL", KEY_NULL) );
}