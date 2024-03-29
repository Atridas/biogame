#include "Camera.h"

CCamera::CCamera(void)
{
  m_bIsOk = false;
}


void CCamera::Release(void)
{
  // ----
}


bool CCamera::Init( const Vect3f& _vPos, 
                    const Vect3f& _vRot, 
                    const float _fMoveSpeed, 
                    const float _fRotateSpeed, 
                    const float _fHealth, 
                    const Vect3f& _vMaxAngle,
                    const Vect3f& _vMinAngle)
{
  if(!Inherited::Init(_vPos,_vRot,_fMoveSpeed,_fRotateSpeed,_fHealth))
    return false;

  m_vMaxAngle = _vMaxAngle;
  m_vMinAngle = _vMinAngle;

  return m_bIsOk = true;
}