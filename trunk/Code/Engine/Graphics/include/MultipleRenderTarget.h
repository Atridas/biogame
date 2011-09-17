#pragma once
#ifndef __MULTIPLE_RENDER_TARGET_H__
#define __MULTIPLE_RENDER_TARGET_H__

#include "RenderTarget.h"

class CXMLTreeNode;
class CTextureRenderTarget;
class CRenderManager;

class CMultipleRenderTarget :
  public CRenderTarget
{
public:
  CMultipleRenderTarget() {};
  virtual ~CMultipleRenderTarget() {Done();};
  
  bool Init(const string& _szName);

  virtual void Activate(CRenderManager* l_pRM);

  bool AddTextureRenderTarget(CTextureRenderTarget* _pTextureRenderTarget, int _iIndex);
  
  virtual int GetWidth ();
  virtual int GetHeight();

protected:
  virtual void Release();

  struct SIndexedTextureRenderTarget
  {
    CTextureRenderTarget* m_pTextureRenderTarget;
    int m_iIndex;

    SIndexedTextureRenderTarget() : m_pTextureRenderTarget(0), m_iIndex(0) {};
  };

  vector<SIndexedTextureRenderTarget> m_vTextureRenderTargets;
};

#endif