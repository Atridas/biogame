#pragma once

#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:
  float m_Angle;
public:
  CSpotLight(){};
  ~CSpotLight(){};
  void SetAngle(float Angle);
  float GetAngle() const;
};
#endif