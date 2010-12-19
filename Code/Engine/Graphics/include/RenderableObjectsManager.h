#pragma once
#ifndef __RENDERABLEOBJECTS_MANAGER_H__
#define __RENDERABLEOBJECTS_MANAGER_H__

#include "Utils/MapManager.h"
#include "RenderableObject.h"

//forward declarations---------------
class CRenderManager;
//---------------------------------

class CRenderableObjectsManager : public CMapManager<CRenderableObject>
{
public:
  CRenderableObjectsManager() : m_szFileName("") {};
  ~CRenderableObjectsManager() {CleanUp();};

  void Update(float _fElapsedTime);
  void Render(CRenderManager *RM);

  CRenderableObject* AddMeshInstance(const string& _szCoreMeshName, const string& _szInstanceName);


  void AddResource(const string& _szName, CRenderableObject* _pRenderableObject);
  void CleanUp() {Destroy();m_RenderableObjects.clear();};
  bool Load(const string& _szFileName);
  bool Reload() {CleanUp(); return Load(m_szFileName);};
  //CRenderableObject* GetInstance(const string& _szName) const;
  // el get instance ja està ben implementat a MapManager

private:
  string m_szFileName;
  vector<CRenderableObject*> m_RenderableObjects;
};

#endif
