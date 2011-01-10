#pragma once

#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include "base.h"
#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:
  float m_fAngle;
  float m_fFallOff;

public:
  CSpotLight() : m_fAngle(0.0f), m_fFallOff(0.0f) {};

  ~CSpotLight(){};
  void SetAngle(float _fAngle) {m_fAngle = _fAngle;};
  float GetAngle() const {return m_fAngle;};
  void SetFallOff(const float _fFallOff) {m_fFallOff = _fFallOff;};
  float GetFallOff() const {return m_fFallOff;};
  virtual void Render(CRenderManager *RM);
};

#endif
