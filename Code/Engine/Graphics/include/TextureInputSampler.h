#pragma once
#ifndef __TEXTURE_INPUT_SAMPLER_H__
#define __TEXTURE_INPUT_SAMPLER_H__

#include "base.h"
#include "InputSampler.h"

class CTextureInputSampler :
  public CInputSampler
{
public:
  CTextureInputSampler() {};
  ~CTextureInputSampler() {Done();};

  bool Init(int _iIndex, const string& _szName);

protected:
  virtual void Release() {};


};

#endif

