#pragma once
#ifndef __SHADOW_MAP_RENDER_TO_TEXTURE_H__
#define __SHADOW_MAP_RENDER_TO_TEXTURE_H__

//Forward declarations ---------------------
class CLight;
// -----------------------------------------

#include "RenderToTextureSceneEffect.h"

class CShadowMapRenderToTexture: public CRenderToTextureSceneEffect
{
public:
  CShadowMapRenderToTexture(void):m_LightShadowCast(0) {};
  ~CShadowMapRenderToTexture() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);

protected:
  void Release() {};
private:
  CLight *m_LightShadowCast;
};

#endif
