#include "Patroller.h"

CPatroller::CPatroller(void)
{
  m_bIsOk = false;
}


void CPatroller::Relase(void)
{
  // ----
}


bool CPatroller::Init(  const Vect3f& _vPos, 
                        const Vect3f& _vRot, 
                        const float   _fMoveSpeed, 
                        const float   _fRotateSpeed, 
                        const float   _fHealth, 
                        const char*   _pcPath)
{
  if(!Inherited::Init(_vPos,_vRot,_fMoveSpeed,_fRotateSpeed,_fHealth))
    return false;

  m_szPath = _pcPath;

  return m_bIsOk = true;
}