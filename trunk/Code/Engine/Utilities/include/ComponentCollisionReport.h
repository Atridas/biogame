#pragma once
#ifndef __COMPONENT_COLLISION_REPORT__
#define __COMPONENT_COLLISION_REPORT__

#include "base.h"
#include "EntityDefines.h"

class CComponentCollisionReport:
  public CBaseComponent
{
public:
  ~CComponentCollisionReport() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_COLLISION_REPORT;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_COLLISION_REPORT;};

  static CComponentCollisionReport* AddToEntity(CGameEntity* _pEntity,
                                                const string& _szOnStartTouch,
                                                const string& _szOnTouch,
                                                const string& _szOnEndTouch,
                                                const string& _szOnStartTouchForceThreshold,
                                                const string& _szOnTouchForceThreshold,
                                                const string& _szOnEndTouchForceThreshold,
                                                const float& _fThreshold);
  
  void OnStartTouch(CGameEntity* _pOther);
  void OnTouch(CGameEntity* _pOther);
  void OnEndTouch(CGameEntity* _pOther);
  void OnStartTouchForceThreshold(CGameEntity* _pOther);
  void OnTouchForceThreshold(CGameEntity* _pOther);
  void OnEndTouchForceThreshold(CGameEntity* _pOther);

protected:
  bool Init(CGameEntity* _pEntity,
            const string& _szOnStartTouch,
            const string& _szOnTouch,
            const string& _szOnEndTouch,
            const string& _szOnStartTouchForceThreshold,
            const string& _szOnTouchForceThreshold,
            const string& _szOnEndTouchForceThreshold,
            const float& _fThreshold);

  CComponentCollisionReport() {};

  virtual void Release() {};

private:

  void ExecuteLua(const string& _szLuaCode, CGameEntity* _pOther);

  string m_szOnStartTouch;
  string m_szOnTouch;
  string m_szOnEndTouch;
  string m_szOnStartTouchForceThreshold;
  string m_szOnTouchForceThreshold;
  string m_szOnEndTouchForceThreshold;

  unsigned int m_uiFlags;
  float m_fThreshold;
};

#endif