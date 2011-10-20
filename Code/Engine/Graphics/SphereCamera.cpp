#include "SphereCamera.h"
#include "base.h"


CSphereCamera::CSphereCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, CObject3D* lookat)
: CCamera( zn, zf, fov, aspect, object3D, TC_FPS)
{
  m_pLookat = lookat;
}

CSphereCamera::CSphereCamera()
: CCamera()
{}



Vect3f CSphereCamera::GetDirection () const
{
	assert(m_pObject3D);

	return (GetLookAt() - GetEye()).GetNormalized();
}

Vect3f CSphereCamera::GetLookAt () const
{
	assert(m_pObject3D);
	assert(m_pLookat);

  return m_pLookat->GetPosition(); 
}

Vect3f CSphereCamera::GetEye () const
{
	assert(m_pObject3D);

	return m_pObject3D->GetPosition();
}

Vect3f CSphereCamera::GetVecUp () const
{
	Vect3f l_Dir = GetDirection();
  Vect3f l_Right = l_Dir ^ Vect3f(0,1,0);
  if(l_Right.SquaredLength() < .05f)
    l_Right = l_Dir ^ Vect3f(0,1,1);
  return (l_Right ^ l_Dir).GetNormalized();
}