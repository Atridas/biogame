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
  void SetActive(bool _bActive);
  
  virtual void PostUpdate(float _fDeltaTime);

protected:
  CComponentArma():m_bActive(true),m_pRenderableObject(0),m_pAnimatedModel(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szMeshName);
  virtual void Release();
private:

  CRenderableObject*                  m_pRenderableObject;
  CRenderableAnimatedInstanceModel*   m_pAnimatedModel;
  bool m_bActive;
};

#endif