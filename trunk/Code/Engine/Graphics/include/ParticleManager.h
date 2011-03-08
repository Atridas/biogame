#pragma once

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "base.h"
#include "ParticleEmitter.h"
#include <XML/XMLTreeNode.h>
#include "params.h"



class CParticleManager
{
public:
  CParticleManager(void);
  ~CParticleManager(){/*Done;*/};
  //bool Load(const SPaticleManagerParams& _params);
  bool Load(const string& _szFileName);
  vector<CParticleEmitter> m_vEmitterParticle;

  void Reload();

private:
  string m_szFileName;

};
#endif
