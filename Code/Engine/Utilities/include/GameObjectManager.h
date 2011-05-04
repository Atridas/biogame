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
  void						CleanUp()                      {Release();m_vResources.clear();m_vUserData.clear();m_vActors.clear();};
  bool						Load	(const string& _szFileName, bool _bReload);
  CPhysicActor*   AddPhysicActor(CRenderableObject* l_pRenderObject, string& _szName, float _fBody, bool _bPushable);
  CPhysicActor*   AddPhysicActorMesh(CRenderableObject* _pRenderObject, string& _szName, float _fBody, bool _bPushable);
  virtual void		Release();
  void            Update(float _fElapsedTime);
private:
  vector<CGameObject*> m_vResources;
  vector<CPhysicUserData*> m_vUserData;
  vector<CPhysicActor*> m_vActors;
};

