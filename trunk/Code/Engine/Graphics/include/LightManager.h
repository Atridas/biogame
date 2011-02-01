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
  CLightManager() : m_szFileName(""),m_vAmbientLight(0) {};
  ~CLightManager()  {Done();};
  bool Load(const string& _szFileName);
  void Render(CRenderManager* _pRM) const;
  
  CLight* GetLight (int _i)  const;
  vector<CLight*> GetLights(int _num) const;

  const Vect3f& getAmbientLight() const {return m_vAmbientLight;};
private:
  Vect3f m_vAmbientLight;
};

#endif