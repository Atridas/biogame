#pragma once
#ifndef __RENDERABLEOBJECT_H__
#define __RENDERABLEOBJECT_H__

#include "Utils/Object3D.h"
#include "Named.h"
#include <XML/XMLTreeNode.h>

//forward declarations---------------
class CRenderManager;
//---------------------------------

class CRenderableObject : 
  public CObject3D,
  public CNamed,
  public CBaseControl
{
public:
  CRenderableObject(const string& _szName):CNamed(_szName) {};
  virtual void Update(float _fElapsedTime) {};
  virtual void InitFromXML(CXMLTreeNode& l_XMLObject) {};
  void Render(CRenderManager *_pRM);

  virtual bool IsAlphaBlended() const = 0;
  
protected:
  virtual void RenderRenderableObject(CRenderManager *RM) = 0;

};

#endif
