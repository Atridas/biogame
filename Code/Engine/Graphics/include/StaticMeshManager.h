#pragma once
#ifndef __STATICMESH_MANAGER_H__
#define __STATICMESH_MANAGER_H__

#include <Utils/MapManager.h>
#include <set>
#include "StaticMesh.h"

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
public:
  CStaticMeshManager()  {SetOk(true);};
  ~CStaticMeshManager() {Done();};
  bool Load(const string &_szFileName);
  bool Load(const vector<string>& _XMLs);
  bool Reload();

private:
  bool Load(const string &_szFileName, bool _bReload);

  set<string> m_vXMLFiles;
};

#endif