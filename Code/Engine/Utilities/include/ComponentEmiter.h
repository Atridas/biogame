#pragma once
#ifndef __COMPONENT_EMITER__
#define __COMPONENT_EMITER__

#include "base.h"
#include "EntityDefines.h"
#include "ParticleConstants.h"

class CEmiterInstance;

class CComponentEmiter:
  public CBaseComponent
{
public:

  ~CComponentEmiter() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_EMITER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_EMITER;};

  static CComponentEmiter* AddToEntity(CGameEntity* _pEntity, const string& _szCore, const Vect3f& _vVolume, int _iMaxParticles = DEFAULT_MAX_PARTICLES_PER_EMITER);
  
  void ChangeCore(const string& _szCore);
  void ChangeVolume(const Vect3f& _vVolume);

protected:

  CComponentEmiter():m_pEmiterInstance(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szCore, const Vect3f& _vVolume, int _iMaxParticles);
  virtual void Release();

private:

  CEmiterInstance* m_pEmiterInstance;
  string           m_szEmiterName;
};

#endif
