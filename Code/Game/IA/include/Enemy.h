#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "EnemyManager.h"

class CEnemy:
  public CEnemyManager

{
public:
  
  enum TEnemyType
  {
	  MILITAR=0,
	  MINER
  };

public:
  CEnemy();
  ~CEnemy();

  // els set s'hauran de calcular
  void SetViewAngle(float _fViewAngle){m_fViewAngle = _fViewAngle;};
  void SetViewRadius(float _fViewAngle){m_fViewAngle = _fViewAngle;};
  void SetSoundRadius(float _fSoundRadius){m_fSoundRadius = _fSoundRadius;};

  float GetViewAngle() const{return m_fViewAngle;};
  float GetViewRadius() const{return m_fViewRadius;};
  float GetSoundRadius() const{return m_fSoundRadius;};
  

protected:
  TEnemyType m_Type;
  float   m_fViewAngle;
  float   m_fViewRadius;
  float   m_fSoundRadius ;
  //m_pFSM     NO SE DE QUIN TIPUS ES NI PQ SERVEIX AIXO (MAKINA ESTATS)
};

#endif