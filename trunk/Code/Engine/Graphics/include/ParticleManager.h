#pragma once

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "base.h"
#include "ParticleEmitter.h"
#include <XML/XMLTreeNode.h>
#include "params.h"
#include <Utils/MapManager.h>

struct SParticleInfo
{
  string                m_szId;
  float                 m_fMinEmitRate;
  float                 m_fMaxEmitRate;
  D3DXCOLOR             m_Color1;
  D3DXCOLOR             m_Color2;
  float                 m_fMinSize;
  float                 m_fMaxSize;
  D3DXVECTOR3           m_vSpawnDir1;
  D3DXVECTOR3           m_vSpawnDir2;
  LPDIRECT3DTEXTURE9    m_pTexParticle;
  
};

class CParticleManager : public CMapManager<SParticleInfo>
{
public:
  CParticleManager(void);
  ~CParticleManager(){/*Done;*/};
  //bool Load(const SPaticleManagerParams& _params);
  bool Load(const string& _szFileName);
 
 

  void Reload();

private:
  string m_szFileName;
	 vector<CParticleEmitter> m_vEmitterParticle;

};
#endif
