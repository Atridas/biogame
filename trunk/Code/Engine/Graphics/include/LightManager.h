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
  string m_szFileName;
public:
  CLightManager() : m_szFileName("") {SetOk(true);};
  ~CLightManager()  {Done();};
  bool Load(const string& _szFileName);
  void Render(CRenderManager* _pRM);
};

#endif
