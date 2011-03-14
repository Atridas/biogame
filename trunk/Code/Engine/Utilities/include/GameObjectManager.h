#pragma once

#include <set>

#include "Utils/MapManager.h"
#include "GameObject.h"
#include "base.h"


//forward declarations---------------
class CRenderManager;
class CStaticMesh;
class CInstanceMesh;
//---------------------------------

class CGameObjectManager : 
  public CMapManager<CGameObject>
{
public:
  CGameObjectManager()                {SetOk(true);};
  ~CGameObjectManager()               {Done();};
  void CleanUp()                      {Release();m_vResources.clear();};
  virtual void Release();
private:
  vector<CGameObject*> m_vResources;
};

