#pragma once
#ifndef __BACK_BUFFER_RENDER_TARGET_H__
#define __BACK_BUFFER_RENDER_TARGET_H__

#include "RenderTarget.h"

class CXMLTreeNode;

class CBackBufferRenderTarget :
  public CRenderTarget
{
public:
  CBackBufferRenderTarget() {};
  virtual ~CBackBufferRenderTarget() {Done();};

  bool Init();

protected:
  virtual void Release();

};

#endif