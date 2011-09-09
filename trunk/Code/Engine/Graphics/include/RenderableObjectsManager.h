#pragma once
#ifndef __RENDERABLEOBJECTS_MANAGER_H__
#define __RENDERABLEOBJECTS_MANAGER_H__

#include <set>

#include "Utils/MapManager.h"
#include "RenderableObject.h"

//forward declarations---------------
class CRenderManager;
struct SHWIntancedMeshes;
class CStaticMesh;
class CInstanceMesh;
class CRenderableAnimatedInstanceModel;
//---------------------------------


class CRenderableObjectsManager : public CMapManager<CRenderableObject>
{
public:
  CRenderableObjectsManager():m_bRenderInstanced(false) {SetOk(true);};
  ~CRenderableObjectsManager() {Done();};

  void Update(float _fElapsedTime);
  void Render(CRenderManager* _pRM);

  CRenderableObject* AddMeshInstance(const string& _szCoreMeshName, const string& _szInstanceName);
  CRenderableObject* AddAnimatedModel(const string& _szCoreModelName, const string& _szInstanceName);

  void AddResource(const string& _szName, CRenderableObject* _pRenderableObject);
  void RemoveResource(const string& _szName);
  void CleanUp() {Release();m_RenderableObjects.clear();m_vMeshes.clear(),m_vAnimatedModels.clear();};
  virtual void Release();

  bool Load(const string& _szFileName)  {return Load(_szFileName,false);};
  bool Load(const vector<string>& _vXMLFiles);

  void SetAllVisibility(bool _bVisible);
  void SetAllRenderBoundingBox(bool _bVisible);
  void SetAllRenderBoundingSphere(bool _bVisible);

  CRenderableObject* GetRenderableObject (int _ID) {return m_RenderableObjects[_ID];};
  int GetRenderableVectorSize() {return m_RenderableObjects.size();};
  
  bool Reload()                          {CleanUp(); return Load(m_vXMLFiles);};
  bool Reload(const string& _szFileName) {CleanUp(); return Load(_szFileName);};

  const vector<CRenderableObject*>& GetRenderableObjects() {return m_RenderableObjects;};
  const vector<CRenderableObject*>& GetMeshes() {return m_vMeshes;};
  const vector<CRenderableAnimatedInstanceModel*>& GetAnimatedModels() {return m_vAnimatedModels;};

  void SetInstanced(bool _bInstanced) {m_bRenderInstanced = _bInstanced;};

private:

  bool m_bRenderInstanced;

  set<string> m_vXMLFiles;
  vector<CRenderableObject*> m_RenderableObjects;
  vector<CRenderableObject*> m_vMeshes;
  vector<CRenderableAnimatedInstanceModel*> m_vAnimatedModels;
  map<const CStaticMesh*,SHWIntancedMeshes*> m_mapHWStaticInstances;

  bool Load(const string& _szFileName, bool _bReload);
  bool Load(const set<string>& _vXMLFiles);

  void AddHWStaticInstance(CInstanceMesh* _pInstanceMesh);
  void RenderHWInstanced(CRenderManager* _pRM);
  void RenderOld(CRenderManager* _pRM);
  void FillBuffers(CRenderManager* _pRM);
};

#endif
