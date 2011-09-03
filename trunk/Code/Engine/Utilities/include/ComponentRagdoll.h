#pragma once
#ifndef __COMPONENT_RAGDOLL__
#define __COMPONENT_RAGDOLL__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
class CRenderableAnimatedInstanceModel;
class CPhysxBone;
// -------------------------------------------------


class CComponentRagdoll:
  public CBaseComponent
{
public:
  ~CComponentRagdoll() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_RAGDOLL;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_RAGDOLL;};

  static CComponentRagdoll* AddToEntity(CGameEntity* _pEntity, const string& _szSkeletonFile, int _iCollisionGroup);
  CPhysxBone* GetBone(const string& _szBoneName);
  CPhysxSkeleton* GetRagdoll() const {return m_pRagdoll;};
  Vect3f GetPosition();

  void ApplyPhysics(bool _bValue);

  void UpdatePrePhysX(float _fDeltaTime);

  virtual void PostUpdate(float _fDeltaTime);

protected:
  CComponentRagdoll():m_pRagdoll(0),m_pRAIM(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szSkeletonFile, int _iCollisionGroup);

  virtual void Enable();
  virtual void Disable();

  virtual void Release();
private:

  CPhysxSkeleton* m_pRagdoll;
  CRenderableAnimatedInstanceModel* m_pRAIM;

};

#endif
