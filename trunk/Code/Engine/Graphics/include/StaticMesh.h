#pragma once
#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include <base.h>

//forward declarations---------------
class CRenderableVertexs;
class CTexture;
class CRenderManager;
//---------------------------------

class CStaticMesh: public CBaseControl
{
protected:
  vector<CRenderableVertexs*> m_RVs;
  vector<vector<CTexture *>> m_Textures;
  string m_szFileName;
  unsigned int m_iNumVertexs, m_iNumFaces;
  void Unload();
public:
  CStaticMesh(): m_iNumVertexs(0),m_iNumFaces(0),m_szFileName(""){};
  ~CStaticMesh() {Done();};
  virtual void Release();
  bool Load (const string &_szFileName);
  bool ReLoad () {Unload();Load(m_szFileName);};
  void Render (CRenderManager *_pRM) const;
};

#endif