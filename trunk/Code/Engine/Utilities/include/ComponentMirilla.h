#pragma once
#ifndef __COMPONENT_MIRILLA__
#define __COMPONENT_MIRILLA__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
class CRenderableObject;
// -------------------------------------------------


class CComponentMirilla:
  public CBaseComponent
{
public:
  ~CComponentMirilla() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_MIRILLA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_MIRILLA;};

  static CComponentMirilla* AddToEntity(CGameEntity* _pEntity, const string& _szName);

protected:
  CComponentMirilla() {};
  bool Init(CGameEntity* _pEntity, const string& _szName);

  virtual void Enable();
  virtual void Disable();

  virtual void Release();

};

#endif