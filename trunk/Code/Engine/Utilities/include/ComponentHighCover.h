#pragma once
#ifndef __COMPONENT_HIGH_COVER__
#define __COMPONENT_HIGH_COVER__

#include "base.h"
#include "ComponentCover.h"

class CComponentHighCover:
  public CComponentCover
{
public:
  CComponentHighCover() {};
  ~CComponentHighCover() {Done();}

  bool Init(CGameEntity* _pEntity, const Vect3f& _vSize, const int _iNumPlaces);

  virtual COVER_TYPE GetCoverType() {return COVER_HIGH;};

};

#endif