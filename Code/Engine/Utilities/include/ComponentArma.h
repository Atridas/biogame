#pragma once
#ifndef __COMPONENT_ARMA__
#define __COMPONENT_ARMA__

#include "base.h"
#include "EntityDefines.h"

// -------------------------------------------------
class CPhysxSkeleton;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;
// -------------------------------------------------


class CComponentArma:
  public CBaseComponent
{
public:
  ~CComponentArma() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_ARMA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_ARMA;};

  static CComponentArma* AddToEntity(CGameEntity* _pEntity, const string& _szMeshName);
  
  Vect3f GetPosition();
  Vect3f GetAimDirection();

  virtual void PostUpdate(float _fDeltaTime);

protected:
  CComponentArma():m_pRenderableObject(0),m_pAnimatedModel(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szMeshName);

  virtual void Enable();
  virtual void Disable();

  virtual void Release();
private:

  CRenderableObject*                  m_pRenderableObject;
  CRenderableAnimatedInstanceModel*   m_pAnimatedModel;
};

#endif