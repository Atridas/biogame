#pragma once
#ifndef __STATICMESH_MANAGER_H__
#define __STATICMESH_MANAGER_H__

#include <Utils/MapManager.h>
#include <set>
#include "StaticMesh.h"

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
public:
  CStaticMeshManager()  {};
  ~CStaticMeshManager() {Destroy();};
  bool Load(const string &_szFileName);
  bool Load(const vector<string>& _XMLs);
  bool Reload();

protected:
  set<string> m_vXMLFiles;

private:
  bool Load(const string &_szFileName, bool _bReload);
};

#endif