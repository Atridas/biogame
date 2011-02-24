#include "base.h"
#include "Utils/Object3D.h"

CObject3D::CObject3D(const Vect3f& _vPos, float _fYaw, float _fPitch, float _fRoll)
: m_vPosition(_vPos)
, m_fYaw(_fYaw)
, m_fPitch(_fPitch)
, m_fRoll(_fRoll)
, m_bVisible(true)
, m_bRenderBoundingBox(false)
, m_bRenderBoundingSphere(false)
{

}

void CObject3D::SetPosition(const Vect3f& _vPos)
{
  m_vPosition = _vPos;
}

void CObject3D::SetYaw( float _fYaw )
{
  m_fYaw = _fYaw;
}

void CObject3D::SetPitch( float _fPitch )
{
  m_fPitch = _fPitch;
}

void CObject3D::SetRoll( float _fRoll )
{
  m_fRoll = _fRoll;
}
	
 Vect3f CObject3D::GetCenterPosition()
 {
   return m_vPosition+m_BoundingBox.GetMiddlePoint();
 }
	
	