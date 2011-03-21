#pragma once

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "base.h"
#include "ParticleEmitter.h"
#include <XML/XMLTreeNode.h>
#include "params.h"
#include <Utils/MapManager.h>

class CRenderManager;

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
  CTexture*             m_pTexParticle;
  float                 m_fLife1;
  float                 m_fLife2;
  vector<D3DXCOLOR>      m_vColor;
  vector<float>          m_vTime;

};

struct SParticleEvent
{
  
  float                 m_fTime;
  D3DXCOLOR             m_Color1;
  D3DXCOLOR             m_Color2;

 
};

class CParticleManager : public CMapManager<SParticleInfo>
{
public:
  CParticleManager(void);
  ~CParticleManager(){Done();};
  
  bool Load(const string& _szFileName);
  void Update(const float _fElapsedTime);
  void Render(CRenderManager* _pRM);
  void Init(CRenderManager* _pRM);
  

  void Reload();

  virtual void Release();

private:
  string m_szFileName;
  vector<CParticleEmitter*> m_vEmitterParticle;
  

};
#endif
