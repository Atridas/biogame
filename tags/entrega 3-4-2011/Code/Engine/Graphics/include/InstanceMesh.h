#pragma once

#ifndef __MESH_INSTANCE_H__
#define __MESH_INSTANCE_H__

#include "base.h"
#include "RenderableObject.h"
#include <XML/XMLTreeNode.h>

//forward declarations-------------
class CRenderManager;
class CStaticMesh;
//---------------------------------


class CInstanceMesh : public CRenderableObject
{
public:
  //Constructor / Destructor
  CInstanceMesh(const string& _szName);
  virtual ~CInstanceMesh() {Done();};

  bool          Init      (const string& _szCoreName);
  virtual void  InitFromXML(CXMLTreeNode& l_XMLObject);

  //Methods
  virtual void  RenderRenderableObject(CRenderManager* _pRM);

  CStaticMesh* GetStaticMesh() { return m_StaticMesh; };

protected:
  virtual void          Release                   ();

private:
  CStaticMesh *m_StaticMesh;
};

#endif //__MESH_INSTANCE_H__