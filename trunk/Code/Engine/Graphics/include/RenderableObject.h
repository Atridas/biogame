#pragma once
#ifndef __RENDERABLEOBJECT_H__
#define __RENDERABLEOBJECT_H__

#include "Object3D.h"

//forward declarations---------------
class CRenderManager;
//---------------------------------

class CRenderableObject : public CObject3D//, public CNamed
{
public:
  CRenderableObject();
  virtual ~CRenderableObject() {};
  virtual void Update(float ElapsedTime) {};
  virtual void Render(CRenderManager *RM) = 0;

//};<-----aqui hi ha un caracter invisible lol

};

#endif
