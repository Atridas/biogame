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
  virtual ~CComponent3rdPSCamera(void) {Done();};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_3RD_PERSON_SHOOTER_CAMERA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_3RD_PERSON_SHOOTER_CAMERA;};

  static CComponent3rdPSCamera* AddToEntity(CGameEntity *_pEntity,
                          float _fCameraHeight,
                          float _fCameraRight,
                          float _fZoom);

  void PostUpdate(float _fDeltaTime);

  CCamera* GetCamera() const;

  //Dades pròpies dels components
  float m_fCameraHeight;
  float m_fCameraRight;
  float m_fZoom;

protected:
  virtual void Release();

  CComponent3rdPSCamera():
      m_pObject3D(0),
      m_pCamera(0),
      m_fCameraHeight(0),
      m_fTimeObjectDistance(0.0f),
      m_fTargetObjectDistance(0.0f),
      m_fPrevObjectDistance(0.0f),
      m_fTargetRightDistance(0.0f),
      m_fTimeRightDistance(0.0f),
      m_tTimeConstantRightDistance(0.0f),
      m_tTimeConstantObjectDistance(0.0f),
      m_fPrevRightDistance(0.0f)
      {};

  bool Init(CGameEntity *_pEntity,
            float _fCameraHeight,
            float _fCameraRight,
            float _fZoom);
  
private:

  float m_tTimeConstantRightDistance;
  float m_tTimeConstantObjectDistance;

  float m_fTimeObjectDistance;
  float m_fTimeRightDistance;

  float m_fTargetObjectDistance;
  float m_fPrevObjectDistance;

  float m_fTargetRightDistance;
  float m_fPrevRightDistance;

  float FirstOrderSystem(float _fInput, float _fTimeConstant, float _fTime);

  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  CObject3D    m_CameraObject;
  CShoulderCamera* m_pCamera;
};

#endif
