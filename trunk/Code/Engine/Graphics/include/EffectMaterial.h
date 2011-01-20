#pragma once
#ifndef __EFFECT_MATERIAL_H__
#define __EFFECT_MATERIAL_H__

#include "base.h"

//---- Forward Declarations ----------------------
class CEffect;
//------------------------------------------------

class CEffectMaterial: public CBaseControl
{
public:
  CEffectMaterial(void);
  ~CEffectMaterial(void) {Done();};

private:

};

#endif
