#pragma once

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "base.h"
#include "ParticleEmitter.h"
#include <XML/XMLTreeNode.h>
#include "params.h"
#include <Utils/MapManager.h>
#include "Camera.h"
#include "RenderableVertexs.h"
#include "Particle.h"

//forward declarations ----------------------------------------------
class CRenderManager;
//-------------------------------------------------------------------

class CParticleManager : public CMapManager<CParticleCore>
{
public:
  CParticleManager();
  ~CParticleManager(){Done();};
  
  bool Load(const string& _szFileName);
  void Update(const float _fElapsedTime, CCamera* camera);
  void Render(CRenderManager* _pRM);
  void Init(CRenderManager* _pRM);
  bool Reload();
  void Release();
  CParticleEmitter* GetParticleEmitter(const string& _szName);
  void SetAllEmittersActive(bool _bActive);

private:
  string                    m_szFileName;
  CRenderManager*           m_pRM;
  vector<CParticleEmitter*> m_vEmitterParticle;
  bool                      m_bReload;

  vector<D3DXVECTOR3>       m_vDirection;
  vector<float>             m_vTimeDirection;
  vector<float>             m_vTimeDirectionInterpolation;

   float                    m_fTime;
   float                    m_fTimeInterpolation;
   string				            m_szFormEmitter;
   float				            m_fSizeX;
   float				            m_fSizeY;
   float				            m_fSizeZ;
   int                      m_iNumDirections;
};
#endif
