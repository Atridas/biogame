#pragma once
#ifndef __ACTION_TO_INPUT_H__
#define __ACTION_TO_INPUT_H__

#include <base.h>
#include "InputDefs.h"

//forward declarations -------------
class CInputManager;
class CProcess;
//----------------------------------

//---------------------------------
struct SInputInfo {
  INPUT_DEVICE_TYPE device;
  INPUT_AXIS_TYPE   axis;
  INPUT_EVENT_TYPE  eventType;
  uint32            code;
  float             delta;

  //bool operator<(const SInputInfo& rhs) const{if(device == rhs.device){if(axis == rhs.axis){if(eventType == rhs.eventType){return code < rhs.code;} else {return eventType < rhs.eventType;}} else {return axis < rhs.axis;}} else {return device < rhs.device;}}
};

struct SAction {
  string    hardCodedAction;
  string    script;

  bool operator<(const SAction& other) const {
    if(hardCodedAction == other.hardCodedAction)
    {
      return script < other.script;
    } else {
      return hardCodedAction < other.hardCodedAction;
    }
  }
};
//---------------------------------

class CActionToInput:
  public CBaseControl
{
  
  
  typedef               map<std::string, uint32>                    TString2Code;
  typedef               vector<SInputInfo>                          TInputTriggers;
  typedef               TInputTriggers::iterator                    TInputTriggersIterator;
  typedef               multimap<SAction,TInputTriggers*>           TActionToTriggers;
  typedef               TActionToTriggers::iterator                 TActionToTriggersIterator;
  typedef               pair<SAction,TInputTriggers*>               TActionToTriggersPair;

public:
  CActionToInput(void):m_szXMLFile(""),m_pActionToTriggers(0),m_pInputManager(0),m_pProcess(0)
                                                                  {};
  ~CActionToInput(void)                                           {Release();};

  bool                  Init                      (CInputManager* _pInputManager, const char* _pcXMLFile);
  void                  Reload                    ()                        {Unload();Load();};
  bool                  SetFile                   (const char* _pcXMLFile)  {m_szXMLFile = _pcXMLFile;};
  
  void                  SetProcess                (CProcess* _pProcess)     {m_pProcess = _pProcess;};

  void                  Update                    (float _fDeltaSeconds);

protected:
  virtual void          Release                   ();
private:
  void                  InitString2Input          ();
  void                  Load                      ();
  void                  Unload                    ();
  void                  ExecuteAction             (float _fDeltaSeconds, float _fDelta, const string& _szAction);
  void                  ExecuteScript             (float _fDeltaSeconds, float _fDelta, const string& _szScript);
  float                 IsTriggered               (const SInputInfo& _Trigger);

  string                m_szXMLFile;
  TString2Code          m_String2Code;
  TActionToTriggers*    m_pActionToTriggers;
  CInputManager*        m_pInputManager;
  CProcess*             m_pProcess;
};


#endif
