#pragma once
#ifndef __COMPONENT_VIDA__
#define __COMPONENT_VIDA__

#include "base.h"
#include "EntityDefines.h"

class CComponentVida:
  public CBaseComponent
{
public:
  CComponentVida():m_fVida(100), m_bImmortal(false) {};
  ~CComponentVida() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_VIDA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_VIDA;};

  bool Init(CGameEntity* _pEntity, float _fVidaInicial);
  
  virtual void ReceiveEvent(const SEvent& _Event);
  
  float m_fVida;
  bool  m_bImmortal;
protected:
  virtual void Release() {};
private:
};

#endif
