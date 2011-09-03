#pragma once
#ifndef __COMPONENT_Shield__
#define __COMPONENT_Shield__

#include "base.h"
#include "EntityDefines.h"

//TODO!! (Si fa falta...
class CComponentShield:
  public CBaseComponent
{
public:

  ~CComponentShield() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_SHIELD;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_SHIELD;};

  static CComponentShield* AddToEntity(CGameEntity* _pEntity, float _fHP, float _fMaxHP, float _fRegenAmount = 2.0f, float _fCooldown = 5.0f);
  
  void Update(float _fDeltaTime);

  /*bool Activate() { m_fTime <= m_fCooldown ? m_bActive = false : m_bActive = true; return m_bActive; };
  void Deactivate()  { m_bActive = false; m_fTime = 0.0f; };
  bool IsActive()  { return m_bActive; };*/

  bool IsReady() { return m_fTime >= m_fCooldown ; };

  float GetHP() { return m_fHP; };
  void Increase(float _fAmount);
  float Decrease(float _fAmount);
  void Set(float _fAmount);

  void SetRegen(float _fRegenAmount);


protected:

  CComponentShield(): m_fHP(100.0f), m_fMaxHP(100.0f), m_fRegenAmount(2.0f), m_fCooldown(5.0f), /*m_bActive(false),*/ m_fTime(0.0f) {};
  bool Init(CGameEntity* _pEntity, float _fHP, float _fMaxHP, float _fRegenAmount = 2.0f, float _fCooldown = 5.0f);
  virtual void Release() {};

private:
  //bool m_bActive;

  float m_fHP;
  float m_fMaxHP;
  float m_fCooldown;
  float m_fRegenAmount;

  float m_fTime;
};

#endif
