#pragma once
#ifndef __ACTION_TO_INPUT_H__
#define __ACTION_TO_INPUT_H__

#include <base.h>
#include "InputDefs.h"

struct SInputInfo {
  INPUT_DEVICE_TYPE device;
  INPUT_AXIS_TYPE   axis;
  INPUT_EVENT_TYPE  eventType;
  uint32            code;
  float             delta;

  bool operator<(const SInputInfo& rhs) const
  {
    if(device == rhs.device)
    {
      if(axis == rhs.axis)
      {
        if(eventType == rhs.eventType)
        {
          //if(lhs.code == rhs.code)
          //{
          //  return lhs.delta < rhs.delta;
          //} else {
            return code < rhs.code;
          //}
            //la delta NO fa diferència etre un input i un altre!
        } else {
          return eventType < rhs.eventType;
        }
      } else {
        return axis < rhs.axis;
      }
    } else {
      return device < rhs.device;
    }
  }
};

struct SAction {
  string    hardCodedAction;
  string    script;
};

struct SInputCompare {
  bool operator() (const SInputInfo& lhs, const SInputInfo& rhs) const
  {
    if(lhs.device == rhs.device)
    {
      if(lhs.axis == rhs.axis)
      {
        if(lhs.eventType == rhs.eventType)
        {
          //if(lhs.code == rhs.code)
          //{
          //  return lhs.delta < rhs.delta;
          //} else {
            return lhs.code < rhs.code;
          //}
            //la delta NO fa diferència etre un input i un altre!
        } else {
          return lhs.eventType < rhs.eventType;
        }
      } else {
        return lhs.axis < rhs.axis;
      }
    } else {
      return lhs.device < rhs.device;
    }
  }
};

class CActionToInput:
  public CBaseControl
{
  
  
  typedef               map<std::string, uint32>                    TString2Code;
  typedef               vector<SInputInfo>                          TInputTriggers;
  typedef               map<TInputTriggers,SAction>   TTriggerToAction;

public:
  CActionToInput(void):m_szXMLFile("")                            {};
  ~CActionToInput(void)                                           {Release();};

  bool                  Init                      (const char* _pcXMLFile);
  bool                  Reload                    ()                        {Unload();Load();};
  bool                  SetFile                   (const char* _pcXMLFile)  {m_szXMLFile = _pcXMLFile;};

  void                  Update                    (float deltaSeconds);

protected:
  virtual void          Release                   ();
private:
  void                  InitString2Input          ();
  void                  Load                      ();
  void                  Unload                    ();
  void                  ExecuteAction             (const string& _szAction);
  void                  ExecuteScript             (const string& _szScript);

  string                m_szXMLFile;
  TString2Code          m_String2Code;
  TTriggerToAction      m_TriggerToAction;
};


#endif
