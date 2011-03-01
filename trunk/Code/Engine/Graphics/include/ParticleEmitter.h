#pragma once

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__
#define    NUMPARTICLES 500

#include "base.h"
#include <d3dx9.h>
#include "Utils\RecyclingArray.h"
#include "Particle.h"

class CParticleEmitter
{
public:
  CParticleEmitter(void);
  ~CParticleEmitter(void);

  //  Set
  void SetPosition(D3DXVECTOR3 _vPos){m_vPos=_vPos;};
  void SetMinEmitRate(float _fMinEmitRate){m_fMinEmitRate = _fMinEmitRate;};
  void SetMaxEmitRate(float _fMaxEmitRate){m_fMaxEmitRate = _fMaxEmitRate;};
  void SetColor1(D3DXCOLOR _Color1){m_Color1 = _Color1;};
  void SetColor2(D3DXCOLOR _Color2){m_Color2 = _Color2;};
  void SetMinSize(float _fMinSize){m_fMinSize = _fMinSize;};
  void SetMaxSize(float _fMaxSize){m_fMaxSize = _fMaxSize;};
  void SetSpawnDir1(D3DXVECTOR3 _vSpawnDir1){m_vSpawnDir1 = _vSpawnDir1;};
  void SetSpawnDir2(D3DXVECTOR3 _vSpawnDir2){m_vSpawnDir2 = _vSpawnDir2;};


  //Get
  D3DXVECTOR3 GetPosition() const {return m_vPos;};


private:

  D3DXVECTOR3                               m_vPos;
  float                                     m_fMinEmitRate;
  float                                     m_fMaxEmitRate;
  D3DXCOLOR                                 m_Color1;
  D3DXCOLOR                                 m_Color2;
  float                                     m_fMinSize;
  float                                     m_fMaxSize;
  D3DXVECTOR3                               m_vSpawnDir1;
  D3DXVECTOR3                               m_vSpawnDir2;
  float                                     m_fNumNewPartsExcess;
  string                                    m_sTexture;
  LPDIRECT3DDEVICE9                         m_pd3dDevice;
  LPDIRECT3DVERTEXBUFFER9                   m_vbParticles;
  //CRecyclingArray<CParticle, NUMPARTICLES>  m_Particles;
};
#endif
