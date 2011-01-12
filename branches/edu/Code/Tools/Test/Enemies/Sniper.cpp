#include "Sniper.h"


CSniper::CSniper(void)
{
  m_bIsOk = false;
}


void CSniper::Release(void)
{
  // ----
}


bool CSniper::Init( const Vect3f&   _vPos, 
                    const Vect3f&   _vRot, 
                    const float     _fMoveSpeed, 
                    const float     _fRotateSpeed, 
                    const float     _fHealth, 
                    const float     _fAimDistance)
{
  if(!Inherited::Init(_vPos,_vRot,_fMoveSpeed,_fRotateSpeed,_fHealth))
    return false;

  m_fAimDistance = _fAimDistance;

  return m_bIsOk = true;
}
