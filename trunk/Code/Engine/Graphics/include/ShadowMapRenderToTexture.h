#pragma once
#ifndef __SHADOW_MAP_RENDER_TO_TEXTURE_H__
#define __SHADOW_MAP_RENDER_TO_TEXTURE_H__

//Forward declarations ---------------------
class CLight;
class CLight;
// -----------------------------------------

#include "RenderToTextureSceneEffect.h"

class CShadowMapRenderToTexture: public CRenderToTextureSceneEffect
{
public:
  CShadowMapRenderToTexture(void):m_pLightShadowCast(0) {};
  ~CShadowMapRenderToTexture() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);
  virtual void PreRender(CRenderManager* _pRM, CProcess* _pProc);

  void SetShadowMapLightCast(CLight* _pLight) {m_pLightShadowCast = _pLight;};

protected:
  void Release();
private:
  CLight *m_pLightShadowCast;
};

#endif
