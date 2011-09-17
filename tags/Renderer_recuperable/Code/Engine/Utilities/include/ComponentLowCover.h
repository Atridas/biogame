#pragma once
#ifndef __COMPONENT_LOW_COVER__
#define __COMPONENT_LOW_COVER__

#include "base.h"
#include "ComponentCover.h"

class CComponentLowCover:
  public CComponentCover
{
public:
  ~CComponentLowCover() {Done();}

  static CComponentLowCover* AddToEntity(CGameEntity *_pEntity, const Vect3f& _vSize, const int _iNumPlaces);
  virtual COVER_TYPE GetCoverType() {return COVER_LOW;};
  
private:
  CComponentLowCover() {};
  bool Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces);
};

#endif