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
  void						CleanUp()                      {Release();m_vResources.clear();};
  bool						Load	(const string& _szFileName, bool _bReload);
  CPhysicActor*   AddPhysicActor(CRenderableObject* l_pRenderObject, string& _szName, string& _szMode, float _fBody);
  virtual void		Release();
private:
  vector<CGameObject*> m_vResources;
};

