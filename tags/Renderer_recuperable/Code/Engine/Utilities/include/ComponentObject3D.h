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
  virtual ~CComponentObject3D(void) {Done();};
  

  virtual CBaseComponent::Type GetType() {return CBaseComponent::ECT_OBJECT_3D;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_OBJECT_3D;};
  
  static CComponentObject3D* AddToEntity(CGameEntity *_pEntity)
  {
    CComponentObject3D *l_pComp = new CComponentObject3D();
    assert(_pEntity && _pEntity->IsOk());
    if(l_pComp->Init(_pEntity))
    {
      l_pComp->SetEntity(_pEntity);
      return l_pComp;
    }
    else
    {
      delete l_pComp;
      return 0;
    }
  }

protected:
  CComponentObject3D(void) {};
  bool Init(CGameEntity *_pEntity) {SetOk(true); return IsOk();};

  void Release(){};
};

#endif
