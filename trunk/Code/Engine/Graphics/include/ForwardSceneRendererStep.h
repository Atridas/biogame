#pragma once
#ifndef __FORWARD_SCENE_RENDERER_STEP_H__
#define __FORWARD_SCENE_RENDERER_STEP_H__

#include "SceneRendererStep.h"

class CForwardSceneRendererStep :
  virtual public CSceneRendererStep
{

public:
  CForwardSceneRendererStep() {};
  ~CForwardSceneRendererStep() {Done();};

  bool Init();
  virtual void Release();
};

#endif