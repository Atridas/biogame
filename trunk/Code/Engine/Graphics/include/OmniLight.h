#pragma once

#ifndef __OMNI_LIGHT_H__
#define __OMNI_LIGHT_H__

#include "Light.h"

class COmniLight : public CLight
{
public:
  COmniLight() : CLight(){};

  ~COmniLight(void){};

  virtual void Render(CRenderManager* _pRM) const;
};
#endif