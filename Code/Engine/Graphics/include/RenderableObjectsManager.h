#pragma once
#ifndef __RENDERABLEOBJECTS_MANAGER_H__
#define __RENDERABLEOBJECTS_MANAGER_H__

#include <set>

#include "Utils/MapManager.h"
#include "RenderableObject.h"

//forward declarations---------------
class CRenderManager;
//---------------------------------

class CRenderableObjectsManager : public CMapManager<CRenderableObject>
{
public:
  CRenderableObjectsManager() {SetOk(true);};
  ~CRenderableObjectsManager() {Done();};

  void Update(float _fElapsedTime);
  void Render(CRenderManager* _pRM);

  CRenderableObject* AddMeshInstance(const string& _szCoreMeshName, const string& _szInstanceName);
  CRenderableObject* AddAnimatedModel(const string& _szCoreModelName, const string& _szInstanceName);

  void AddResource(const string& _szName, CRenderableObject* _pRenderableObject);
  void CleanUp()                        {Release();m_RenderableObjects.clear();m_vMeshes.clear(),m_vAnimatedModels.clear();};

  bool Load(const string& _szFileName)  {return Load(_szFileName,false);};
  bool Load(const vector<string>& _vXMLFiles);

  void SetAllVisibility(bool _bVisible);
  void SetAllRenderBoundingBox(bool _bVisible);

  CRenderableObject* GetRenderableObject (int _ID) {return m_RenderableObjects[_ID];};
  int GetRenderableVectorSize() {return m_RenderableObjects.size();};

  bool Reload() {CleanUp(); return Load(m_vXMLFiles);};

  const vector<CRenderableObject*>& GetMeshes() {return m_vMeshes;};
  const vector<CRenderableObject*>& GetAnimatedModels() {return m_vAnimatedModels;};

private:

  set<string> m_vXMLFiles;
  vector<CRenderableObject*> m_RenderableObjects;
  vector<CRenderableObject*> m_vMeshes;
  vector<CRenderableObject*> m_vAnimatedModels;

  bool Load(const string& _szFileName, bool _bReload);
  bool Load(const set<string>& _vXMLFiles);
};

#endif
