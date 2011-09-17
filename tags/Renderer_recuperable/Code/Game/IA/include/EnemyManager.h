#pragma once

#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "base.h"
#include <Utils/MapManager.h>
#include "Enemy.h"

//Forward declarations-------------
class CXMLTreeNode;
//---------------------------------


class CEnemyManager : public CMapManager<CEnemy>
 // public CBaseControl de moment fora
{
public:
  CEnemyManager();
  ~CEnemyManager(){Done();};
  void Release();
  bool Load(const string& m_szFileName);


private:
  bool Load();
  string m_szFileName;
};
#endif
