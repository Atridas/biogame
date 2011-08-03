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
  ~CComponentRagdoll() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_RAGDOLL;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_RAGDOLL;};

  static CComponentRagdoll* AddToEntity(CGameEntity* _pEntity, const string& _szSkeletonFile, int _iCollisionGroup);
  void SetActive(bool _bActive);
  
  void UpdatePostAnim(float _fDeltaTime);

protected:
  CComponentRagdoll():m_bActive(false),m_pRagdoll(0),m_pRAIM(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szSkeletonFile, int _iCollisionGroup);
  virtual void Release();
private:

  CPhysxSkeleton* m_pRagdoll;
  CRenderableAnimatedInstanceModel* m_pRAIM;
  bool m_bActive;
};

#endif
