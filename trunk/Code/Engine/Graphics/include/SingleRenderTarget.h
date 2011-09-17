#pragma once
#ifndef __SINGLE_RENDER_TARGET_H__
#define __SINGLE_RENDER_TARGET_H__

#include "RenderTarget.h"
#include <d3dx9.h>

class CXMLTreeNode;

class CSingleRenderTarget :
  public CRenderTarget
{
public:
  CSingleRenderTarget() : m_pSurface(0) {};
  virtual ~CSingleRenderTarget() {Done();};

  void Activate(CRenderManager* l_pRM);
  virtual void Activate(CRenderManager* l_pRM, int _iIndex);

protected:

  virtual void Release();

  LPDIRECT3DSURFACE9 m_pSurface;

};

#endif