#pragma once
#ifndef __SCENE_EFFECT_H__
#define __SCENE_EFFECT_H__

#include "base.h"
#include "Core.h"
#include "RenderManager.h"
#include "Named.h"

//Forward declarations-------------
class CRenderManager;
class CProcess;
class CTexture;
//---------------------------------

class CSceneEffect : public /*CActive,*/ CNamed
{
public:

  CSceneEffect(/*MKeyValue &atts*/) : CNamed("") {};
  virtual ~CSceneEffect() {};
  void ActivateTextures();
  void AddStageTexture(int _iStageId, CTexture* _pTexture);

  virtual void PreRender(CRenderManager* _pRM, CProcess* _pProc);
  virtual void PostRender(CRenderManager* _pRM);
  virtual void CaptureFrameBuffers(CRenderManager* _pRM);

protected:
  class CStageTexture
  {
    public:
    CStageTexture(int _iStageId, CTexture* _pTexture);
    void Activate();

    int m_iStageId;
    CTexture* m_pTexture;
  };

  vector<CStageTexture> m_vStageTextures;
};

#endif