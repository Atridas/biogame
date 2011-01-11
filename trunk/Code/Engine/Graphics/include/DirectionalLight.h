#pragma once

#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "base.h"
#include "Light.h"

class CDirectionalLight : public CLight
{
protected:
  Vect3f m_vDirection;
  
public:
  CDirectionalLight() : m_vDirection(0) {};

  ~CDirectionalLight() {};
  void SetDirection(const Vect3f& _vDirection);
  Vect3f GetDirection() const {return m_vDirection;};
  virtual void Render(CRenderManager *RM) const;
};
#endif
