#pragma once
#ifndef __COMPONENT_DELAYED_SCRIPT__
#define __COMPONENT_DELAYED_SCRIPT__

#include "base.h"
#include "EntityDefines.h"

class CComponentDelayedScript:
  public CBaseComponent
{
public:

  ~CComponentDelayedScript() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_DELAYED_SCRIPT;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_DELAYED_SCRIPT;};

  static CComponentDelayedScript* AddToEntity(CGameEntity* _pEntity, float _fTime, const string& _szScript);

  void Update(float _fDeltaTime);

  
  bool Reset(float _fTime, const string& _szScript);
  void Reset(float _fTime) { Reset(_fTime, m_szScript); };
protected:

  CComponentDelayedScript():m_fTime(0.0f),m_fTargetTime(1.0f), m_szScript(""), m_bTriggered(false) {};
  virtual void Release() {};

private:

  bool m_bTriggered;

  float m_fTime;
  float m_fTargetTime;

  string m_szScript;
};

#endif
