#pragma once
#ifndef INC_SHOULDER_CAMERA_H_
#define INC_SHOULDER_CAMERA_H_

#include "ThPSCamera.h"

class CShoulderCamera : public CThPSCamera
{
public:
  CShoulderCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, float _fObjectDistance, float _fShoulderDistance, float _fShoulderHeight);
  CShoulderCamera();
  ~CShoulderCamera(void);

  //---Interfaz de CCamera
	virtual Vect3f GetLookAt() const;
	virtual Vect3f GetEye() const;

  //---Interfaz de CShoulderCamera
  void	  SetShoulderDistance(float _fShoulderDistance) {m_fShoulderDistance = _fShoulderDistance;};
	float	  GetShoulderDistance() const	{return m_fShoulderDistance;}
  void	  SetShoulderHeight(float _fShoulderHeight) {m_fShoulderHeight = _fShoulderHeight;};
	float	  GetShoulderHeight() const	{return m_fShoulderHeight;}

private:

  float m_fShoulderDistance;
  float m_fShoulderHeight;

};

#endif
