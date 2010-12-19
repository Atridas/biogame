#include "EnemyInstance.h"


CEnemyInstance::CEnemyInstance(void)
{
  m_bIsOk = false;
}


void CEnemyInstance::Release(void)
{
  // ----
}


bool CEnemyInstance::Init(  const Vect3f&   _vPos, 
                            const Vect3f&   _vRot, 
                            const float     _fMoveSpeed, 
                            const float     _fRotateSpeed, 
                            const float     _fHealth)
{
  m_vPos = _vPos;
  m_vRot = _vRot;
  m_fMoveSpeed = _fMoveSpeed;
  m_fRotateSpeed = _fRotateSpeed;
  m_fHealth = _fHealth;

  return m_bIsOk = true;
}