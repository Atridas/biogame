#pragma once
#ifndef __COMPONENT_COVER__
#define __COMPONENT_COVER__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentCover:
  public CComponentPhysXActor
{
public:

  enum COVER_TYPE
  {
    COVER_LOW = 0,
    COVER_HIGH
  };

  CComponentCover(): m_iNumPlaces(0) {};
  ~CComponentCover() {Done();}

  bool Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces);
  
  virtual COVER_TYPE GetCoverType() = 0;

  virtual CComponentCover::Type GetType() {return CBaseComponent::ECT_COVER;};
  static CComponentCover::Type GetStaticType() {return CBaseComponent::ECT_COVER;};

  virtual void ReceiveEvent(const SEvent& _Event);
  
private:

  int m_iNumPlaces;
};

#endif