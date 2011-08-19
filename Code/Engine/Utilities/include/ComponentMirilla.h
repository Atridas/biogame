#pragma once
#ifndef __COMPONENT_MIRILLA__
#define __COMPONENT_MIRILLA__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
class CDrawQuadSceneEffect;
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
  void SetActive(bool _bActive);
  
  virtual void PostUpdate(float _fDeltaTime);

protected:
  CComponentMirilla():m_bActive(false),m_pAim(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szName);
  virtual void Release();
private:

  CDrawQuadSceneEffect* m_pAim;
  bool m_bActive;
};

#endif