#include "base.h"
#include "FPSCamera.h"

void CFPSCamera::Init(float zn, float zf, float fov, float aspect, const CObject3D* object3D)
{
  m_fZNear = zn;
  m_fZFar  = zf;
  m_fFOV   = fov;
  m_fAspectRatio = aspect;
  SetObject3D(object3D);
}

CFPSCamera::CFPSCamera()
: CCamera()
{}



Vect3f CFPSCamera::GetDirection () const
{
	assert(m_pObject3D);

	return (GetLookAt() - GetEye());
}

Vect3f CFPSCamera::GetLookAt () const
{
	assert(m_pObject3D);

	//float yaw		= m_pObject3D->GetYaw();
	//float pitch	= m_pObject3D->GetPitch();
	Vect3f pos	= m_pObject3D->GetPosition();


  Mat44f m = m_pObject3D->GetMat44();
  Vect3f dir(-1,0,0);
  dir = m * dir;
	//Pasamos de coordenadas esfericas a coordenadas cartesianas

	//Vect3f vEyePt(	cos(yaw) * cos(pitch), 
	//	sin(pitch),
	//	sin(yaw) * cos(pitch) );

	return (pos + dir); 
}

Vect3f CFPSCamera::GetEye () const
{
	assert(m_pObject3D);

	return m_pObject3D->GetPosition();


}

Vect3f CFPSCamera::GetVecUp () const
{
	assert(m_pObject3D);

	//float yaw		= m_pObject3D->GetYaw();
	//float pitch	= m_pObject3D->GetPitch();
  //
	//Vect3f vUpVec(	-cos(yaw) * sin(pitch), 
	//	cos(pitch), 
	//	-sin(yaw) * sin(pitch) );
  
  Mat44f m = m_pObject3D->GetMat44();
  Vect3f dir(0,1,0);
  dir = m * dir;

	return dir;
}