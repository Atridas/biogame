#pragma once
#ifndef __COMPONENT_LIFETIME__
#define __COMPONENT_LIFETIME__

#include "base.h"
#include "EntityDefines.h"

class CComponentLifetime:
  public CBaseComponent
{
public:

  ~CComponentLifetime() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_LIFETIME;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_LIFETIME;};

  static CComponentLifetime* AddToEntity(CGameEntity* _pEntity, float _fTime, const string& _szScript = "");

  void Update(float _fDeltaTime);

protected:

  CComponentLifetime():m_fTime(1.0f), m_szScript("") {};
  bool Init(CGameEntity* _pEntity, float _fTime, const string& _szScript = "");
  virtual void Release() {};

private:
  float m_fTime;
  string m_szScript;
};

#endif
