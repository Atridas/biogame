#pragma once

#include "Process.h"
#include "ParticleEmitter.h"
#include <base.h>
#include "EntityManager.h"


//Forward declarations---------------------
class CObject3D;
class CRenderManager;
class CRenderableObject;
//class CBillBoard;
class CPhysicUserData;
class CPhysicActor;

//-----------------------------------------


class CParticleProcess:
  public CProcess
{
public:
  CParticleProcess(): CProcess("CParticleProcess"),
                      m_iState(0),
                      m_bStateChanged(false),
                      m_pObject(0),
                      m_pObjectBot(0),
                      m_pObjectCamera(0),
                      m_fVelocity(0.0f),
                      m_bRenderLights(false),
                      B(7.f, 0.f, 1.f),
                      l_VUp(3.f, 4.f, 5.f),
                      m_Position(9.f, 2.f, 6.f),
                      m_vDemoObj(2.f, 2.f,3.f ),
                      m_pPlayerEntity(0),
                      m_pUserData(0),
                      m_pPActorPlane(0)
                      {};
                      


  
  virtual               ~CParticleProcess           ()                          {Done();};

  virtual bool          Init();

  void                  Update                  (float elapsedTime);
  void                  DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

  void                  Release                 ();

  int                   m_iState;
  bool                  m_bStateChanged;
  CObject3D*            m_pObject;
  CObject3D*            m_pObjectBot;
  CCamera*              m_pObjectCamera;
  float                 m_fVelocity;
  bool                  m_bRenderLights;
  float                 m_fPhysxVelocity;
  CGameEntity*          m_pPlayerEntity;
  CPhysicUserData*      m_pUserData;
  CPhysicActor*         m_pPActorPlane;

  Vect3f m_PointA;
  Vect3f m_PointB;
  Vect3f m_PointC;
  Vect3f m_PointD;
  float angle;
  Vect3f B;
  //Vect3f l_VDirection = (1,1,1);
  Vect3f l_VUp;
  Vect3f m_Position;
  Vect3f l_VDirection;
  Vect3f                m_vDemoObj;
  float ModAB;
  Vect3f AB;
};

