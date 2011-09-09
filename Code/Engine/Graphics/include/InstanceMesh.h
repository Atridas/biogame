#pragma once

#ifndef __MESH_INSTANCE_H__
#define __MESH_INSTANCE_H__

#include "base.h"
#include "RenderableObject.h"
#include <XML/XMLTreeNode.h>
#include "StaticMesh.h"

//forward declarations-------------
class CRenderManager;
class CStaticMesh;
class CEffect;
class CMaterial;
//---------------------------------


class CInstanceMesh : public CRenderableObject
{
public:
  //Constructor / Destructor
  CInstanceMesh(const string& _szName) : CRenderableObject(_szName) {};
  virtual ~CInstanceMesh() {Done();};

  bool          Init      (const string& _szCoreName);
  virtual void  InitFromXML(CXMLTreeNode& l_XMLObject);

  CStaticMesh* GetStaticMesh() { return m_StaticMesh; };

  virtual const vector<CMaterial*>& GetMaterials() const {return m_StaticMesh->GetMaterials();};

  virtual bool IsAlphaBlended() const;

  //TODO
  virtual bool ChangeInstance(const string& _szName);

protected:
  virtual void  RenderRenderableObject(CRenderManager* _pRM, const vector<CEffect*>& _vEffects);

  virtual void          Release                   ();

private:
  CStaticMesh *m_StaticMesh;
};

#endif //__MESH_INSTANCE_H__