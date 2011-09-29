#pragma once
#ifndef __COMPONENT_INTERACTIVE__
#define __COMPONENT_INTERACTIVE__

#include "base.h"
#include "EntityDefines.h"


class CComponentObject3D;



#define RADI_ESFERA_INTERACTUABLES 5.f
#define UPDATE_BILLBOARD_SECONDS 1.f


class CComponentInteractive:
  public CBaseComponent
{
public:
  ~CComponentInteractive() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_INTERACTIVE;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_INTERACTIVE;};

  static CComponentInteractive* AddToEntity(CGameEntity* _pEntity, const string& _szAction);
  
  virtual void ReceiveEvent(const SEvent& _Event);
  virtual void Update(float _fDeltaTime);

protected:
  CComponentInteractive():m_szAction(""),m_bBillboardActive(false),m_fLastUpdate(0),m_pObject3D(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szAction);
  virtual void Release();

private:
  void CallAction(int _iCaller);

  string m_szAction;

  CComponentObject3D * m_pObject3D;

  bool m_bBillboardActive;
  float m_fLastUpdate;
};

#endif