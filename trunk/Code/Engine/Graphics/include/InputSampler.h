#pragma once
#ifndef __INPUT_SAMPLER_H__
#define __INPUT_SAMPLER_H__

#include "Utils/BaseControl.h"

class CTexture;

class CInputSampler :
  public CBaseControl
{
public:
  CInputSampler() : m_pTexture(0) {};
  ~CInputSampler() {Done();};

  virtual void Activate();

private:
  int m_iIndex;

protected:

  void Init(int _iIndex) {m_iIndex = _iIndex;};
  virtual void Release() {};

  CTexture* m_pTexture;

};

#endif
