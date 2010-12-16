#pragma once
#ifndef __STATICMESH_MANAGER_H__
#define __STATICMESH_MANAGER_H__

#include <Utils/MapManager.h>
#include "StaticMesh.h"

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
public:
  CStaticMeshManager() : m_szFileName("") {};
  ~CStaticMeshManager() {Destroy();};
  bool Load(const string &_szFileName);
  bool Reload();

protected:
  string m_szFileName;

private:
  bool Load(const string &_szFileName, bool _bReload);
};

#endif