#pragma once
#ifndef __TEXTURE_RENDER_TARGET_H__
#define __TEXTURE_RENDER_TARGET_H__

#include "RenderTarget.h"
#include "base.h"

class CTexture;
class CXMLTreeNode;

class CTextureRenderTarget :
  public CRenderTarget
{
public:
  CTextureRenderTarget() : m_szTextureName(""),m_iWidth(0),m_iHeight(0),m_pTexture(0) {};
  virtual ~CTextureRenderTarget() {Done();};

  bool Init(CXMLTreeNode& _treeRenderTarget, int _iWidth, int _iHeight, const string& _szFormat);

  virtual void Activate();

protected:
  virtual void Release();

private:

  string m_szTextureName;

  CTexture* m_pTexture;
  
  int m_iWidth;
  int m_iHeight;
};

#endif
