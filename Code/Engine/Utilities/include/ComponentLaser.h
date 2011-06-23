#pragma once
#ifndef __COMPONENT_LASER__
#define __COMPONENT_LASER__

#include "base.h"
#include "EntityDefines.h"

class CComponentLaser:
  public CBaseComponent
{
public:
  ~CComponentLaser() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_LASER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_LASER;};
  
  static CComponentLaser* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd, float _fTime);

  virtual void Update(float _fDeltaTime);

  virtual void DebugRender(CRenderManager*);

protected:
  CComponentLaser() {};
  bool Init(CGameEntity* _pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd, float _fTime);
  virtual void Release() {};
private:
  Vect3f m_vPosInit;
  Vect3f m_vPosEnd;
  float  m_fTime;
};

#endif