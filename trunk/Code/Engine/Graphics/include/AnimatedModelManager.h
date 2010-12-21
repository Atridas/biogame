#pragma once
#ifndef __ANIMATED_MODEL_MANAGER_H__
#define __ANIMATED_MODEL_MANAGER_H__

#include "base.h"

#include "Utils/MapManager.h"

#include "AnimatedCoreModel.h"

// Forward declarations -------------
class CAnimatedInstanceModel;
// ----------------------------------

class CAnimatedModelManager:
  public CMapManager<CAnimatedCoreModel>
{
public:
  CAnimatedModelManager(void)   {SetOk(true);};
  ~CAnimatedModelManager(void)  {Done();};

  CAnimatedCoreModel*     GetCore(const std::string &Name, const std::string &Path);
  CAnimatedInstanceModel* GetInstance(const std::string &Name);
};

#endif