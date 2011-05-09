#pragma once
#ifndef __COMPONENT_RENDERABLE_OBJECT__
#define __COMPONENT_RENDERABLE_OBJECT__

#include "base.h"
#include "EntityDefines.h"
#include "Utils/Object3D.h"

//--- forward declarations
class CComponentObject3D;
class CRenderableObject;
//--------------------

class CComponentRenderableObject :
  public CBaseComponent
{
public:


  CComponentRenderableObject():
      m_pObject3D(0), 

      m_pRenderableObject(0)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_RENDERABLE_OBJECT;};

  bool Init(CGameEntity *_pEntity, const string& _szName);

  void PostUpdate(float _fDeltaTime);

  virtual ~CComponentRenderableObject(void) {Done();};

  CRenderableObject* GetRenderableObject() const {return m_pRenderableObject;};

protected:
  virtual void Release() {};

private:
  //Altres components referenciats
  CComponentObject3D* m_pObject3D;

  CRenderableObject*  m_pRenderableObject;
};

#endif
