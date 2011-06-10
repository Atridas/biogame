#include "ShoulderCamera.h"
#include "base.h"

CShoulderCamera::CShoulderCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, float _fObjectDistance, float _fShoulderDistance, float _fShoulderHeight)
  : CThPSCamera(zn, zf, fov, aspect, object3D, _fObjectDistance), m_fShoulderDistance(_fShoulderDistance), m_fShoulderHeight(_fShoulderHeight)
{}

CShoulderCamera::CShoulderCamera()
  : CThPSCamera(), m_fShoulderDistance(1.0f)
{}

CShoulderCamera::~CShoulderCamera(void)
{
}

Vect3f CShoulderCamera::GetLookAt () const
{
	assert(m_pObject3D);

  Vect3f l_vPos(0.0f);
  float l_fYaw = m_pObject3D->GetYaw();

  l_vPos.x = m_fShoulderDistance * sin(l_fYaw);
  l_vPos.y = m_fShoulderHeight;
  l_vPos.z = -m_fShoulderDistance * cos(l_fYaw);

	return m_pObject3D->GetPosition() + l_vPos;
}

Vect3f CShoulderCamera::GetEye() const
{
	assert(m_pObject3D);

  Vect3f l_vPos(0.0f);
  float l_fYaw = m_pObject3D->GetYaw();

  l_vPos.x = m_fShoulderDistance * sin(l_fYaw);
  l_vPos.y = m_fShoulderHeight;
  l_vPos.z = -m_fShoulderDistance * cos(l_fYaw);

	return (l_vPos + CThPSCamera::GetEye()); 
}
