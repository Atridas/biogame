#pragma once
#ifndef __LIGHT_MANAGER_H__
#define __LIGHT_MANAGER_H__

#include "base.h"
#include "Light.h"
#include <Utils/MapManager.h>

// Forward declarations -------------
class CRenderManager;
//----------------------------------

class CLightManager : public CMapManager<CLight>
{
public:
  CLightManager()   {SetOk(true);};
  ~CLightManager()  {Done();};
  void Load(const std::string &FileName);
  void Render(CRenderManager *RenderManager);
};
#endif