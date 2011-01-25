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
class CXMLTreeNode;
//---------------------------------

class CSceneEffect : public CNamed, public CBaseControl
{
public:

  CSceneEffect(/*MKeyValue &atts*/) : CNamed("") {};
  virtual ~CSceneEffect() {Done();};
  void ActivateTextures();
  void AddStageTexture(int _iStageId, CTexture* _pTexture);

  virtual bool Init(const CXMLTreeNode& _params) = 0;

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