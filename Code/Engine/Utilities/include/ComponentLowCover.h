#pragma once
#ifndef __COMPONENT_LOW_COVER__
#define __COMPONENT_LOW_COVER__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentLowCover:
  public CComponentPhysXActor
{
public:
  CComponentLowCover(): m_iNumPlaces(0) {};
  ~CComponentLowCover() {Done();}

  CComponentLowCover::Type GetType() {return CBaseComponent::ECT_LOW_COVER;};
  static CComponentLowCover::Type GetStaticType() {return CBaseComponent::ECT_LOW_COVER;};

  bool Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces);
  
  virtual void ReceiveEvent(const SEvent& _Event);

protected:
  virtual void Release() {};
private:

  int m_iNumPlaces;
};

#endif