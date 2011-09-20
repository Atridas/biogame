#pragma once
#ifndef __DEFERRED_POST_SCENE_RENDERER_STEP_H_
#define __DEFERRED_POST_SCENE_RENDERER_STEP_H_

#include "PostSceneRendererStep.h"

class CLight;
class CCamera;

class CDeferredPostSceneRendererStep :
  virtual public CPostSceneRendererStep
{
public:
  CDeferredPostSceneRendererStep() {};
  virtual ~CDeferredPostSceneRendererStep() {Done();};

protected:
  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera);

  virtual void Release();

private:

  bool ClipOmniLight(CLight* _pOmni, RECT& _Rect, CCamera* _pCamera);
};

#endif