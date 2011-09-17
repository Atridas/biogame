#pragma once
#ifndef __LEVEL_CHANGER__
#define __LEVEL_CHANGER__

#include "base.h"
#include "Utils\MapManager.h"

struct SLevel
{
  set<string> StaticMeshes;
  set<string> RenderableObjects;
  set<string> Portals;
  set<string> Entities;
};


class CLevelChanger: 
  private CMapManager<SLevel>
{
public:
  CLevelChanger():m_szNewLevel("") {};
  ~CLevelChanger() {Done();};

  bool Init(const string& _szXMLLevels);

  void Update(float _fElapsedTime);
  bool ChangingLevel() const { return false; };

  void SetNewLevel(const string& _szNewLevel) {m_szNewLevel = _szNewLevel;};

protected:
  void Release() {m_szNewLevel = ""; CMapManager<SLevel>::Release();};

private:
  
  string m_szNewLevel;

};

#endif