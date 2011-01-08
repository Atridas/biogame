#pragma once

#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "Light.h"
#include "Object3D.h"

class CDirectionalLight : public CLight
{
protected:
  CObject3D m_Direction;
  
public:
  CDirectionalLight() : CLight() {};
  ~CDirectionalLight(){};
  void SetDirection(const CObject3D &Direction);
  CObject3D GetDirection() const;
  virtual void Render(CRenderManager *RM);
};
#endif