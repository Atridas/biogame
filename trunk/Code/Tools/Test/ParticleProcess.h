#pragma once

#include "Process.h"
#include "ParticleEmitter.h"
#include <base.h>


//Forward declarations---------------------
class CObject3D;
class CRenderManager;
class CRenderableObject;

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
                      m_bRenderLights(false)
                      {};
                      


  
  virtual               ~CParticleProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
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
  CParticleEmitter      m_ParticleEmitter;
};

