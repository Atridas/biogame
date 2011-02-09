#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "base.h"
#include "Utils/Object3D.h"
#include <XML/XMLTreeNode.h>
#include "Named.h"

class CEnemy:
  public CObject3D,
  public CNamed,
  public CBaseControl

{
public:
  
  enum TEnemyType
  {
	  MILITAR=0,
	  MINER,
    VIGIA
  };

public:
  CEnemy(const string& _name):
				    CNamed(_name),
            m_Type(MILITAR),
            m_fViewAngle(0.0f),
            m_fViewRadius(0.0f),
            m_fSoundRadius(0.0f),
            m_fRunSpeed(0.0f),
            m_fWalkSpeed(0.0f),
            m_fAtacSpeed(0.0f),
            m_fLife(0.0f),
            m_iWeaponStrength(0)    {};

  virtual ~CEnemy(){};

  virtual void Init(CXMLTreeNode& _XMLParams);
  
	   

  // els set s'hauran de calcular
  void SetViewAngle(float _fViewAngle){m_fViewAngle = _fViewAngle;};
  void SetViewRadius(float _fViewAngle){m_fViewAngle = _fViewAngle;};
  void SetSoundRadius(float _fSoundRadius){m_fSoundRadius = _fSoundRadius;};
  void SetType(const TEnemyType _Type) {m_Type = _Type;};
  void SetRunSpeed(float _fRunSpeed){m_fRunSpeed = _fRunSpeed;};
  void SetWalkSpeed(float _fWalkSpeed){m_fWalkSpeed = _fWalkSpeed;};
  void SetAtacSpeed(float _fAtacSpeed){m_fAtacSpeed = _fAtacSpeed;};
  void SetLife(float _fLife){m_fLife = _fLife;};
  void SetWeaponStrength(int _iWeaponStrength){m_iWeaponStrength = _iWeaponStrength;}; 

  float GetViewAngle() const{return m_fViewAngle;};
  float GetViewRadius() const{return m_fViewRadius;};
  float GetSoundRadius() const{return m_fSoundRadius;};
  TEnemyType GetType() const {return m_Type;};
  float GetRunSpeed() const {return m_fRunSpeed;};
  float GetWalkSpeed() const {return m_fWalkSpeed;};
  float GetAtacSpeed() const {return m_fAtacSpeed;};
  float GetLife() const {return m_fLife;};
  int   GetWeaponStrength() const {return m_iWeaponStrength;};

protected:
  TEnemyType m_Type;
  float   m_fViewAngle;
  float   m_fViewRadius;
  float   m_fSoundRadius;
  float	  m_fRunSpeed;
  float	  m_fWalkSpeed;
  float	  m_fAtacSpeed;
  float	  m_fLife;
  int     m_iWeaponStrength;

  //m_pFSM     NO SE DE QUIN TIPUS ES NI PQ SERVEIX AIXO (MAKINA ESTATS)
};

#endif