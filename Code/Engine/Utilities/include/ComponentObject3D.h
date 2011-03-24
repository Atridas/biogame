#pragma once
#ifndef __COMPONENT_OBJECT_3D__
#define __COMPONENT_OBJECT_3D__

#include "base.h"
#include "BaseComponent.h"
#include "Utils/Object3D.h"

class CComponentObject3D :
  public CBaseComponent,
  public CObject3D
{
public:
  CComponentObject3D(void);
  virtual ~CComponentObject3D(void) {Done();};



  void Release(){};
};

#endif
