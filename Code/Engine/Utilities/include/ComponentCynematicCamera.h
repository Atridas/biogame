#pragma once
#ifndef __COMPONENT_CYNEMATIC_CAMERA__
#define __COMPONENT_CYNEMATIC_CAMERA__

#include "base.h"
#include "EntityDefines.h"
#include "Utils\Object3D.h"
#include "SphereCamera.h"


class CComponentCynematicCamera:
  public CBaseComponent
{
public:

  ~CComponentCynematicCamera() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_CYNEMATIC_CAMERA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_CYNEMATIC_CAMERA;};

  static CComponentCynematicCamera* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vTarget, float _fFov, float _fNear, float _fFar);
  
  CSphereCamera* GetCamera() {return m_pCamera;};
  void SetTargetPosition(const Vect3f& v) { m_Target.SetPosition(v); };
  void ActivateCamera() const;
  void DeactivateCamera() const;

protected:

  CComponentCynematicCamera():m_pCamera(0) {};
  bool Init(CGameEntity* _pEntity, const Vect3f& _vTarget, float _fFov, float _fNear, float _fFar);
  virtual void Release();

private:
  CSphereCamera* m_pCamera;

  CObject3D m_Target;
};

#endif
