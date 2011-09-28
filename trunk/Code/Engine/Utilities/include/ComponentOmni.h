#pragma once
#ifndef __COMPONENT_OMNI__
#define __COMPONENT_OMNI__

#include "base.h"
#include "EntityDefines.h"

class COmniLight;
class CComponentObject3D;

class CComponentOmni:
  public CBaseComponent
{
public:

  ~CComponentOmni() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_OMNI;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_OMNI;};

  static CComponentOmni* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt);

  void Update(float _fDeltaTime);

protected:

  CComponentOmni(): m_pOmni(0), m_pObject3D(0) {};
  bool Init(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt);
  virtual void Release();

private:
  COmniLight*         m_pOmni;
  CComponentObject3D* m_pObject3D;
  Vect3f              m_vOffset;
};

#endif