#pragma once
#ifndef __COMPONENT_LASER__
#define __COMPONENT_LASER__

#include "base.h"
#include "EntityDefines.h"

class CComponentObject3D;


#define DEFAULT_LASER_SPEED 100

class CComponentLaser:
  public CBaseComponent
{
public:
  ~CComponentLaser() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_LASER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_LASER;};
  
  static CComponentLaser* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vDir, float _fDamage, uint32 _uiCollisionMask);

  virtual void Update(float _fDeltaTime);

  virtual void DebugRender(CRenderManager*);

  float m_fSpeed;
protected:
  CComponentLaser():m_fDamage(0),l_pCO3D(0),m_fSpeed(DEFAULT_LASER_SPEED) {};
  bool Init(CGameEntity* _pEntity, const Vect3f& _vDir, float _fDamage, uint32 _uiCollisionMask);
  virtual void Release() {};
private:
  Vect3f m_vDir;
  float  m_fDamage;
  uint32 m_uiCollisionMask;

  CComponentObject3D * l_pCO3D;
};

#endif