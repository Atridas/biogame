#pragma once
#ifndef __COMPONENT_3RD_PERSON_SHOOTER_CAMERA__
#define __COMPONENT_3RD_PERSON_SHOOTER_CAMERA__

#include "base.h"
#include "EntityDefines.h"
#include "Utils/Object3D.h"

//--- forward declarations
class CComponentObject3D;
class CShoulderCamera;
class CCamera;
//--------------------

class CComponent3rdPSCamera :
  public CBaseComponent
{
public:


  CComponent3rdPSCamera():
      m_pObject3D(0),
      m_pCamera(0),
     
      m_fCameraHeight(0)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_3RD_PERSON_SHOOTER_CAMERA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_3RD_PERSON_SHOOTER_CAMERA;};

  bool Init(CGameEntity *_pEntity,
            float _fCameraHeight,
            float _fCameraRight,
            float _fZoom);

  void PostUpdate(float _fDeltaTime);

  CCamera* GetCamera() const;

  virtual ~CComponent3rdPSCamera(void) {Done();};

  //Dades pròpies dels components
  float m_fCameraHeight;
  float m_fCameraRight;
  float m_fZoom;

protected:
  virtual void Release();

private:
  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  CObject3D    m_CameraObject;
  CShoulderCamera* m_pCamera;
};

#endif
