#pragma once
#ifndef __COMPONENT_SOUND__
#define __COMPONENT_SOUND__

#include "base.h"
#include "EntityDefines.h"

class CComponentSoundEmiter:
  public CBaseComponent
{
public:

  ~CComponentSoundEmiter() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_SOUND;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_SOUND;};

  static CComponentSoundEmiter* AddToEntity(CGameEntity* _pEntity);

  void Update(float _fDeltaTime);

protected:

  CComponentSoundEmiter() {};
  bool Init(CGameEntity* _pEntity);
  virtual void Release();

private:

};

#endif