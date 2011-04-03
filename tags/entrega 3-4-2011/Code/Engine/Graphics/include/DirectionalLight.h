#pragma once

#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "base.h"
#include "Light.h"

class CDirectionalLight : public CLight
{
public:
  CDirectionalLight(const string& _name) : 
      CLight(_name),
      m_vDirection(0) 
      {m_Type = CLight::DIRECTIONAL;};

  ~CDirectionalLight() {};

  virtual void Init(CXMLTreeNode& _XMLParams);

  void SetDirection(const Vect3f& _vDirection);
  Vect3f GetDirection() const {return m_vDirection;};
  virtual void Render(CRenderManager *RM) const;

  Mat44f GetLightViewMatrix()       const;
  Mat44f GetLightProjectionMatrix() const;

protected:
  void Release() {};

  Vect3f m_vDirection;
};
#endif
