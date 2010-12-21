#pragma once
#ifndef __ANIMATED_MODEL_MANAGER_H__
#define __ANIMATED_MODEL_MANAGER_H__

#include "base.h"

#include <set>

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
  bool Load(const string &_szFileName);
  bool Load(const vector<string>& _XMLs);
  bool Reload();

  CAnimatedCoreModel*     GetCore(const std::string &Name);
  CAnimatedInstanceModel* GetInstance(const std::string &Name);
  
private:
  CAnimatedCoreModel*     GetCore(const std::string &Name, const std::string &Path);

  set<string> m_vXMLFiles;
};

#endif