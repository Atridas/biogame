#pragma once
#ifndef __TEXTURE_RENDER_TARGET_H__
#define __TEXTURE_RENDER_TARGET_H__

#include "SingleRenderTarget.h"
#include "base.h"

class CTexture;
class CXMLTreeNode;

class CTextureRenderTarget :
  public CSingleRenderTarget
{
public:
  CTextureRenderTarget() : m_szTextureName(""),m_iWidth(0),m_iHeight(0) {};
  virtual ~CTextureRenderTarget() {Done();};

  bool Init(CXMLTreeNode& _treeRenderTarget);

  //virtual void Activate(CRenderManager* l_pRM, int _iIndex);

  virtual int GetWidth()  {return m_iWidth;};
  virtual int GetHeight() {return m_iHeight;};

protected:
  virtual void Release();

private:

  string m_szTextureName;

  int m_iWidth;
  int m_iHeight;
};

#endif
