#pragma once
#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

#include "base.h"
#include "Utils/BaseControl.h"
#include "Named.h"

class CRenderManager;

class CRenderTarget :
  public CBaseControl, public CNamed
{
public:
  CRenderTarget() : CNamed("") {};
  virtual ~CRenderTarget() {};

  virtual void Activate(CRenderManager* l_pRM) = 0;

  LPDIRECT3DSURFACE9 GetDepthStencilSurface() const { return m_pDepthStencilSurface; };
  
  virtual int GetWidth () = 0;
  virtual int GetHeight() = 0;

protected:
  virtual void Release();

  void Init(int m_iWidth, int m_iHeight);
  void Init();
  void Init(const LPDIRECT3DSURFACE9 _pDepthStencilSurface) { m_pDepthStencilSurface = _pDepthStencilSurface; };

  
  LPDIRECT3DSURFACE9 m_pDepthStencilSurface;
};

#endif