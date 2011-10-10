#pragma once
#ifndef __COMPONENT_ENERGY__
#define __COMPONENT_ENERGY__

#include "base.h"
#include "EntityDefines.h"

class CComponentEnergy;

class CComponentEnergy:
  public CBaseComponent
{
public:

  ~CComponentEnergy() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_ENERGY;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_ENERGY;};

  static CComponentEnergy* AddToEntity(CGameEntity* _pEntity, float _fInitialEnergy, float _fMaxEnergy = 100.f, float _fRegenAmount = 10.0f, float _fTimeForRegen = 2.0f);

  void Update(float _fDeltaTime);

  float    GetEnergy() {return m_fEnergy; };
  float GetMaxEnergy() {return m_fMaxEnergy;};
  
  void Increase(float _fAmount);
  bool Decrease(float _fAmount);

  void Set(float _fAmount);
  void SetRegen(float _fRegenAmount);

protected:

  CComponentEnergy():m_fEnergy(100.0f), m_fMaxEnergy(100.0f), m_fRegenAmount(0.0f), m_fTimeSinceUsed(0.0f), m_fTimeForRegen(5.0f)
                    {};
  bool Init(CGameEntity* _pEntity, float _fInitialEnergy, float _fMaxEnergy, float _fRegenAmount, float _fTimeForRegen);
  virtual void Release() {};

private:
  float m_fEnergy;
  float m_fMaxEnergy;
  float m_fRegenAmount;
  float m_fTimeSinceUsed;
  float m_fTimeForRegen;
};

#endif
