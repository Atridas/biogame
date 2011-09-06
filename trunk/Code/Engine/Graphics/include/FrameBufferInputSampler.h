#pragma once
#ifndef __FRAME_BUFFER_INPUT_SAMPLER_H__
#define __FRAME_BUFFER_INPUT_SAMPLER_H__

#include "InputSampler.h"

class CFrameBufferInputSampler :
  public CInputSampler
{
public:
  CFrameBufferInputSampler() {};
  ~CFrameBufferInputSampler() {Done();};

  bool Init(int _iIndex);
  virtual void Release();

  virtual void Activate();
};

#endif