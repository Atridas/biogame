#pragma once
#ifndef __COMPONENT_RAGDOLL__
#define __COMPONENT_RAGDOLL__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
// -------------------------------------------------


class CComponentRagdoll:
  public CBaseComponent
{
public:
  CComponentRagdoll() {};
  ~CComponentRagdoll() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_RAGDOLL;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_RAGDOLL;};

  bool Init(CGameEntity* _pEntity, const string& _szSkeletonFile);
  void ActivateRagdoll();
  
  virtual void Update(float _fDeltaTime);

protected:
  virtual void Release();
private:

  CPhysxSkeleton* m_pRagdoll;
};

#endif
