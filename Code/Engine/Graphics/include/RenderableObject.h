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

class CRenderableObjectOrdering
{
  Vect3f m_vCameraPos;
public:
  CRenderableObjectOrdering(const Vect3f& _vCameraPos):m_vCameraPos(_vCameraPos)
    {;}
  CRenderableObjectOrdering(const CRenderableObjectOrdering& _other):m_vCameraPos(_other.m_vCameraPos)
    {;}
  bool operator() (const CRenderableObject* lhs, const CRenderableObject* rhs) const
  {
    Vect3f l_LeftDist  = lhs->GetPosition() - m_vCameraPos;
    Vect3f l_RightDist = rhs->GetPosition() - m_vCameraPos;
    
    float l_fLeftDistSQ  = l_LeftDist  * l_LeftDist;
    float l_fRightDistSQ = l_RightDist * l_RightDist;

    return l_fLeftDistSQ < l_fRightDistSQ;
  }
};

#endif
