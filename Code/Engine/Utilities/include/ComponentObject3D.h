#pragma once
#ifndef __COMPONENT_OBJECT_3D__
#define __COMPONENT_OBJECT_3D__

#include "base.h"
#include "EntityDefines.h"
#include "Utils/Object3D.h"

class CComponentObject3D :
  public CBaseComponent,
  public CObject3D
{
public:
  CComponentObject3D(void) {};
  virtual ~CComponentObject3D(void) {Done();};
  

  virtual CBaseComponent::Type GetType() {return CBaseComponent::ECT_OBJECT_3D;};
  
  bool Init(CGameEntity *_pEntity) { assert(_pEntity->IsOk()); SetEntity(_pEntity); SetOk(true); return IsOk();};


  void Release(){};
};

#endif
