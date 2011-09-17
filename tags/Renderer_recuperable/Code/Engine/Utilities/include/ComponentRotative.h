#pragma once
#ifndef __COMPONENT_ROTATIVE__
#define __COMPONENT_ROTATIVE__

#include "base.h"
#include "EntityDefines.h"

class CComponentRenderableObject;

class CComponentRotative:
  public CBaseComponent
{
public:

  ~CComponentRotative() {Done();}
  
  CBaseComponent::Type GetType() {return CBaseComponent::ECT_ROTATIVE;};
  static CComponentRotative::Type GetStaticType() {return CBaseComponent::ECT_ROTATIVE;};

  static CComponentRotative* AddToEntity(CGameEntity *_pEntity, float _fYawRotation, float _fPitchRotation, float _fRollRotation);

  virtual void PreUpdate(float _fDeltaTime);

  float m_fYawRotation, m_fPitchRotation, m_fRollRotation;

protected:
  virtual void Release() {};

private:
  bool Init(CGameEntity* _pEntity, float _fYawRotation, float _fPitchRotation, float _fRollRotation);
  CComponentRotative(): m_fYawRotation(0.f), m_fPitchRotation(0.f), m_fRollRotation(0.f), m_pCRO(0) {};

  CComponentRenderableObject* m_pCRO;
};

#endif