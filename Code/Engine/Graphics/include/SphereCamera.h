#pragma once
#ifndef INC_SPHERE_CAMERA_H_
#define INC_SPHERE_CAMERA_H_

#include "Camera.h"

class CSphereCamera: public CCamera
{
public:
  CSphereCamera(float zn, float zf, float fov, float aspect, CObject3D* eye, CObject3D* lookat);
	CSphereCamera();
	virtual ~CSphereCamera(){/*Nothing*/;}

	//---Interfaz de CCamera
	virtual Vect3f					GetDirection		() const;
	virtual Vect3f					GetLookAt				() const;
	virtual Vect3f					GetEye					() const;
	virtual Vect3f					GetVecUp				() const;

  CObject3D* m_pLookat;
};

#endif
