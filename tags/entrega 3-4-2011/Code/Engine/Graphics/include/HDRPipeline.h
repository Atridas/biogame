#pragma once
#ifndef __HDR_PIPELINE_H__
#define __HDR_PIPELINE_H__

#include "base.h"
#include "SceneEffect.h"

// Forward declarations -----------
class CRenderToTextureSceneEffect;
class CDrawQuadToTextureSceneEffect;
class CDrawQuadSceneEffect;
class CRenderManager;
class CProcess;
// --------------------------------

class CHDRPipeline:
  public CBaseControl,
  public CActivable
{
public:
  CHDRPipeline(): m_pFirstPass(0), m_pBrightPass(0),
                  m_pDownSample(0), m_pFirstBlurPass(0),
                  m_pBrightBlured(0), m_pRenderEffect(0) 
                {};
  ~CHDRPipeline() {Done();};

  bool Init(const string& _szXML);
  
  void PrepareTextures(CRenderManager* _pRM, CProcess* _pProcess);
  void Render(CRenderManager* _pRM);

protected:
  void Release();

private:

  CRenderToTextureSceneEffect*           m_pFirstPass;
  vector<CDrawQuadToTextureSceneEffect*> m_vLuminancePasses;
  CDrawQuadToTextureSceneEffect*         m_pBrightPass;
  CDrawQuadToTextureSceneEffect*         m_pDownSample;
  CDrawQuadToTextureSceneEffect*         m_pFirstBlurPass;
  CDrawQuadToTextureSceneEffect*         m_pBrightBlured;

  CDrawQuadSceneEffect*                  m_pRenderEffect;
};

#endif