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
  InitMat44();
}

void CObject3D::SetPosition(const Vect3f& _vPos)
{
  m_vPosition = _vPos;
  //m_vMat44.SetPos(_vPos);
  InitMat44();
}

void CObject3D::SetYaw( float _fYaw )
{
  m_fYaw = _fYaw;
  InitMat44();
}

void CObject3D::SetPitch( float _fPitch )
{
  m_fPitch = _fPitch;
  InitMat44();
}

void CObject3D::SetRoll( float _fRoll )
{
  m_fRoll = _fRoll;
  InitMat44();
}
	
Vect3f CObject3D::GetCenterPosition()
{
  return m_vMat44 * m_BoundingBox.GetMiddlePoint();
}


void  CObject3D::InitMat44()
{
  m_vMat44.SetFromAnglesYXZ(m_fYaw,m_fRoll,m_fPitch);
  m_vMat44.SetPos(m_vPosition);

  //Mat44f r, r2, r3, t, total;
  //  
  //t.SetIdentity();
  //r.SetIdentity();
  //r2.SetIdentity();
  //r3.SetIdentity();
  //
  //t.Translate(m_vPosition);
  //r.RotByAngleY(m_fYaw);
  //r2.RotByAngleX(m_fRoll);
  //r3.RotByAngleZ(m_fPitch);
  //
  //m_vMat44 = t*r*r2*r3;
}

void CObject3D::SetMat44(Mat44f _vMat44) 
{
  m_vMat44 = _vMat44;
  //m_fPitch = _vMat44.GetPitch();
  //m_fYaw = -_vMat44.GetYaw();
  //m_fRoll = _vMat44.GetRoll();
  m_fPitch =  _vMat44.GetAngleZ();
  m_fYaw   = _vMat44.GetAngleY();
  m_fRoll  =  _vMat44.GetAngleX();
  m_vPosition = _vMat44.GetPos();
}
	
	