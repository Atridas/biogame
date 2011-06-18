#pragma once
#ifndef __COMPONENT_MIRILLA__
#define __COMPONENT_MIRILLA__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;
// -------------------------------------------------


class CComponentMirilla:
  public CBaseComponent
{
public:
  CComponentMirilla():m_bActive(false),m_pRenderableObject(0) {};
  ~CComponentMirilla() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_MIRILLA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_MIRILLA;};

  bool Init(CGameEntity* _pEntity, const string& _szMeshName);
  void SetActive(bool _bActive);
  
  virtual void UpdatePostAnim(float _fDeltaTime);

protected:
  virtual void Release();
private:

  CRenderableObject*                m_pRenderableObject;
  bool m_bActive;
};

#endif