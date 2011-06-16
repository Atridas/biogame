#pragma once
#ifndef __COMPONENT_RAGDOLL__
#define __COMPONENT_RAGDOLL__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
class CRenderableAnimatedInstanceModel;
// -------------------------------------------------


class CComponentRagdoll:
  public CBaseComponent
{
public:
  CComponentRagdoll():m_bActive(false),m_pRagdoll(0),m_pRAIM(0) {};
  ~CComponentRagdoll() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_RAGDOLL;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_RAGDOLL;};

  bool Init(CGameEntity* _pEntity, const string& _szSkeletonFile);
  void SetActive(bool _bActive);
  
  virtual void UpdatePostAnim(float _fDeltaTime);

protected:
  virtual void Release();
private:

  CPhysxSkeleton* m_pRagdoll;
  CRenderableAnimatedInstanceModel* m_pRAIM;
  bool m_bActive;
};

#endif
