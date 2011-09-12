#pragma once
#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

#include "base.h"
#include "Utils/BaseControl.h"
#include <d3dx9.h>

class CXMLTreeNode;

class CRenderTarget :
  public CBaseControl
{
public:
  CRenderTarget() : m_iIndex(0),m_pSurface(0),m_pDepthStencilSurface(0),m_pPrevSurface(0),m_pPrevDepthStencilSurface(0) {};
  virtual ~CRenderTarget() {Done();};
  
  virtual void Activate(CRenderManager* l_pRM);
  void Deactivate(CRenderManager* l_pRM);

protected:

  void Init(int _iIndex) {m_iIndex = _iIndex;};
  virtual void Release();

  LPDIRECT3DSURFACE9 m_pSurface;
  LPDIRECT3DSURFACE9 m_pDepthStencilSurface;

  LPDIRECT3DSURFACE9 m_pPrevSurface;
  LPDIRECT3DSURFACE9 m_pPrevDepthStencilSurface;

  int m_iIndex;
};

#endif