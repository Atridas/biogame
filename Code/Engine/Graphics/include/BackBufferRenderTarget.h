#pragma once
#ifndef __BACK_BUFFER_RENDER_TARGET_H__
#define __BACK_BUFFER_RENDER_TARGET_H__

#include "SingleRenderTarget.h"

class CBackBufferRenderTarget :
  public CSingleRenderTarget
{
public:
  CBackBufferRenderTarget() {};
  virtual ~CBackBufferRenderTarget() {Done();};

  bool Init(const string& _szName);
  
  virtual int GetWidth ();
  virtual int GetHeight();

};

#endif