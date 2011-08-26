#pragma once
#ifndef __COMPONENT_VIDA__
#define __COMPONENT_VIDA__

#include "base.h"
#include "EntityDefines.h"

class CComponentShield;

class CComponentVida:
  public CBaseComponent
{
public:

  ~CComponentVida() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_VIDA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_VIDA;};

  static CComponentVida* AddToEntity(CGameEntity* _pEntity, float _fVidaInicial, float _fVidaMaxima, bool _bRegen = false, float _fRegenAmount = 0.0f, float _fTimeForRegen = 5.0f);
  
  virtual void ReceiveEvent(const SEvent& _Event);

  void Update(float _fDeltaTime);

  float GetHP() { return m_fVida; };
  void Increase(float _fAmount);
  void Decrease(float _fAmount);
  void Set(float _fAmount);
  float GetMaxHP() {return m_fVidaMaxima;};
  void SetRegen(float _fRegenAmount);
  
  bool  m_bImmortal;
protected:

  CComponentVida():m_fVida(100.0f), m_fVidaMaxima(100.0f), m_bImmortal(false), m_fRegenAmount(0.0f), m_bRegen(false), m_pShield(0), m_fTimeSinceHit(0.0f), m_fTimeForRegen(5.0f) {};
  bool Init(CGameEntity* _pEntity, float _fVidaInicial, float _fVidaMaxima, bool _bRegen = false, float _fRegenAmount = 0.0f, float _fTimeForRegen = 5.0f);
  virtual void Release() {};

private:
  float m_fVida;
  float m_fVidaMaxima;
  bool  m_bRegen;
  float m_fRegenAmount;
  float m_fTimeSinceHit;
  float m_fTimeForRegen;

  //escut
  CComponentShield* m_pShield;
};

#endif
