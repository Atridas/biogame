#pragma once

#ifndef __OMNI_LIGHT_H__
#define __OMNI_LIGHT_H__

#include "Light.h"

class COmniLight : public CLight
{
public:
  COmniLight() : CLight(){m_Type = CLight::OMNI;};

  ~COmniLight(void){};

  virtual void Init(CXMLTreeNode& _XMLParams);

  virtual void Render(CRenderManager* _pRM) const;
};
#endif