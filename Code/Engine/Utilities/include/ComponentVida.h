#pragma once
#ifndef __COMPONENT_VIDA__
#define __COMPONENT_VIDA__

#include "base.h"
#include "EntityDefines.h"

class CComponentVida:
  public CBaseComponent
{
public:
  CComponentVida() {};
  ~CComponentVida() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_VIDA;};

  bool Init(CGameEntity* _pEntity, float _fVidaInicial);
  
  virtual void ReceiveEvent(const SEvent& _Event);

protected:
  virtual void Release() {};
private:
  float m_fVida;
};

#endif