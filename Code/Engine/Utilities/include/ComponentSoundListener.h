#pragma once
#ifndef __COMPONENT_SOUND_LISTENER__
#define __COMPONENT_SOUND_LISTENER__

#include "base.h"
#include "EntityDefines.h"

class CComponentObject3D;
class CComponent3rdPSCamera;

class CComponentSoundListener:
  public CBaseComponent
{
public:

  ~CComponentSoundListener() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_SOUND_LISTENER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_SOUND_LISTENER;};

  static CComponentSoundListener* AddToEntity(CGameEntity* _pEntity);

  void Update(float _fDeltaTime);

protected:

  CComponentSoundListener():m_pO3D(0),m_pCamera(0) {};
  bool Init(CGameEntity* _pEntity);
  virtual void Release() {};

private:
  CComponentObject3D* m_pO3D;
  CComponent3rdPSCamera* m_pCamera;
};

#endif
