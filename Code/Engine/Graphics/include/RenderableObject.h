#pragma once
#ifndef __RENDERABLEOBJECT_H__
#define __RENDERABLEOBJECT_H__

#include "Utils/Object3D.h"
#include "Named.h"

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
  virtual void Update(float ElapsedTime) {};
  void Render(CRenderManager *RM);
  Vect3f m_vMax;
  Vect3f m_vMin;
  float m_fAltura;
  

protected:
  virtual void RenderRenderableObject(CRenderManager *RM) = 0;

private:
};

#endif
