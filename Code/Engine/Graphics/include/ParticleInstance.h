#pragma once
#ifndef __PARTICLE_INSTANCE_H__
#define __PARTICLE_INSTANCE_H__
#include "base.h"
#include "CoreEmiter.h"
#include "ParticleConstants.h"

// ----------------------------
struct SParticleRenderInfo;
// ----------------------------

class CParticleInstance
{
public:
  CParticleInstance() {};

  ~CParticleInstance() {};
  
  void Init(CCoreEmiter* _pCoreEmiter, const Vect3f& _vInitialPosition);

  bool Update(float _fDeltaTime);

  void FillRenderInfo(SParticleRenderInfo& Info_);

private:

  float m_fLivedTime;
  float m_fTotalLifetime;

  Vect3f m_vSpeed;
  float  m_fAngularSpeed;
  
  Vect3f m_vAcceleration;
  float  m_fAngularAcceleration;

  // ------------------------------------------
  bool m_bHasNextColor, m_bHasNextSize;
  int  m_iCurrentColor, m_iCurrentSize;

  CColor m_LastColor, m_NextColor;
  float  m_fLastColorTime, m_fNextColorTime;
  float  m_fLastSize, m_fNextSize;
  float  m_fLastSizeTime, m_fNextSizeTime;
  //------------------------------------------


  CCoreEmiter* m_pCoreEmiter;

  //rendering information
  Vect3f m_vPosition;
  float  m_fSize;
  float  m_fAngle;
  int    m_iFrame;
  CColor m_Color;
};


#endif