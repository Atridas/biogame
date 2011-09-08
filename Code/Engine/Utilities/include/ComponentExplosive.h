#pragma once
#ifndef __COMPONENT_EXPLOSIVE__
#define __COMPONENT_EXPLOSIVE__

#include "base.h"
#include "EntityDefines.h"

class CComponentExplosive:
  public CBaseComponent
{
public:

  ~CComponentExplosive() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_EXPLOSIVE;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_EXPLOSIVE;};

  static CComponentExplosive* AddToEntity(CGameEntity* _pEntity);

  void Explode(Vect3f _vPos, float _fRadius);

protected:
  CComponentExplosive() {};
  bool Init(CGameEntity* _pEntity);
  virtual void Release() {};
};

#endif
