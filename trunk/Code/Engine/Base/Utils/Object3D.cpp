#include "base.h"
#include "Utils/Object3D.h"

CObject3D::CObject3D(const Vect3f& _vPos, float _fYaw, float _fPitch, float _fRoll)
: m_vPosition(_vPos)
, m_fYaw(_fYaw)
, m_fPitch(_fPitch)
, m_fRoll(_fRoll)
, m_bVisible(true)
{

}

void CObject3D::SetPosition(const Vect3f& _vPos)
{
  m_vPosition = _vPos;
  m_BoundingBox.TranslateBox(_vPos);
}

void CObject3D::SetYaw( float _fYaw )
{
  m_fYaw = _fYaw;
  m_BoundingBox.DoYaw(_fYaw);
}

void CObject3D::SetPitch( float _fPitch )
{
  m_fPitch = _fPitch;
  m_BoundingBox.DoPitch(_fPitch);
}

void CObject3D::SetRoll( float _fRoll )
{
  m_fRoll = _fRoll;
  m_BoundingBox.DoRoll(_fRoll);
}
	
	
	
	