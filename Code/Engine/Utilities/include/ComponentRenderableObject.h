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


      
  CBaseComponent::Type GetType() {return CBaseComponent::ECT_RENDERABLE_OBJECT;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_RENDERABLE_OBJECT;};
  
  static CComponentRenderableObject* AddToEntity(CGameEntity *_pEntity, const string& _szName, const string& _szCore);
  static CComponentRenderableObject* AddToEntityWithAnimatedModel(CGameEntity *_pEntity, const string& _szName, const string& _szCore);

  void PostUpdate(float _fDeltaTime);

  virtual ~CComponentRenderableObject(void) {Done();};

  CRenderableObject* GetRenderableObject() const {return m_pRenderableObject;};

  void SetYaw(float _fYaw);

  bool ChangeInstance(const string& _szName);

  bool m_bRemoveRenderableObject;
  bool m_bBlockPitchRoll;
  bool m_bBlockYaw;
  float m_fHeightAdjustment;
  float m_fYawAdjustment;

  bool m_bActive;

protected:
  CComponentRenderableObject():
      m_pObject3D(0), 

      m_pRenderableObject(0),

      m_bRemoveRenderableObject(false),
      m_bBlockPitchRoll(false),
      m_bBlockYaw(false),
      m_fHeightAdjustment(0),
      m_fYawAdjustment(0),
      m_bActive(true)
      {};
  bool Init(CGameEntity *_pEntity, const string& _szName, const string& _szCore);
  bool InitAnimatedModel(CGameEntity *_pEntity, const string& _szName, const string& _szCore);
  virtual void Release();

private:
  //Altres components referenciats
  CComponentObject3D* m_pObject3D;

  CRenderableObject*  m_pRenderableObject;

  string m_szResourceName;
};

#endif
