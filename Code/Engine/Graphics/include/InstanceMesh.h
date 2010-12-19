#pragma once

#ifndef __MESH_INSTANCE_H__
#define __MESH_INSTANCE_H__

#include "base.h"
#include "RenderableObject.h"
#include "StaticMesh.h"

//forward declarations-------------
class CRenderManager;
//---------------------------------


class CInstanceMesh : public CRenderableObject
{
public:
  //Constructor / Destructor
  CInstanceMesh(const string& _szName, const string& _szCoreName);
  ~CInstanceMesh()                                                      {CHECKED_DELETE(m_StaticMesh);};

  //Methods
  void Render(CRenderManager* _pRM);

private:
  CStaticMesh *m_StaticMesh;
};

#endif //__MESH_INSTANCE_H__