#pragma once
#ifndef INC_PHYSICS_GRENADE_H_
#define INC_PHYSICS_GRENADE_H_

#include "base.h"
#include "Named.h"

class CalBone;
class CPhysicActor;
class CPhysicUserData;
class CXMLTreeNode;
class CalSkeleton;



class CPhysxGrenade :
  public CBaseControl,
  public CNamed
{
public:

  //Constructors
  CPhysxGrenade(string _szName,float _fEffectRadius, float _fDuration, float _fPower):  m_bExploted(false),
                                                                                        m_fLifeTime(0.0f),
                                                                                        m_pPhysxActor(0),
                                                                                        m_fEffectRadius(_fEffectRadius),
                                                                                        m_fDuration(_fDuration),
                                                                                        m_fFirePower(_fPower),
                                                                                        CNamed(_szName)
                                                                                        {};
  ~CPhysxGrenade() {Done(); Release();};

  void              Init(float _fSphereRadius, float _fDensity, int _iColGroup, Vect3f _vPosition, Vect3f _vDirection, float _fVelocity, string _szEmitter);
  void              Explosion();
  void              Update(float _fElapsedTime);
  virtual void      Release();

private:
  float             m_fEffectRadius;
  float             m_fDuration;
  float             m_fFirePower;
  bool              m_bExploted;
  float             m_fLifeTime;
  CPhysicActor*     m_pPhysxActor;
  CPhysicUserData*  m_pPhysxUserData;
};

#endif 