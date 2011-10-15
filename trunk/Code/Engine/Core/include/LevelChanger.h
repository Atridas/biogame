#pragma once
#ifndef __LEVEL_CHANGER__
#define __LEVEL_CHANGER__

#include "base.h"
#include "Utils\MapManager.h"

struct SLevel
{
  string      RenderPath;
  string      Script;
  vector<string> StaticMeshes;
  vector<string> RenderableObjects;
  vector<string> Portals;
  vector<string> Entities;
  vector<string> Lights;
};

#define INITIAL_LEVEL_CHANGER_COUNTDOWN 2

class CLevelChanger: 
  private CMapManager<SLevel>
{
public:
  CLevelChanger():m_szNewLevel(""),m_bChanging(false),m_szCurrentLevel("") {};
  ~CLevelChanger() {Done();};

  bool Init(const string& _szXMLLevels);

  void Update(float _fElapsedTime);
  bool ChangingLevel() const { return m_bChanging; };

  void SetNewLevel(const string& _szNewLevel) {m_szNewLevel = m_szCurrentLevel = _szNewLevel;};
  void ResetLevel() {m_szNewLevel = m_szCurrentLevel;};

protected:
  void Release() {m_szNewLevel = ""; CMapManager<SLevel>::Release();};

private:
  
  string m_szNewLevel;
  string m_szCurrentLevel;

  bool m_bChanging;
  int  m_iCountdown;
  set<string> m_RenderPathsToActivate;

};

#endif