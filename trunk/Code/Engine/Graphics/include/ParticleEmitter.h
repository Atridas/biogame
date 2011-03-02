#pragma once

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__
#define    NUMPARTICLES 500

#include "base.h"
#include <d3dx9.h>
#include "Utils\RecyclingArray.h"
#include "Particle.h"
#include "RenderManager.h"

class CParticleEmitter
{
public:
  CParticleEmitter();
  ~CParticleEmitter(void);

  //  Set
  void SetPosition          (D3DXVECTOR3& _vPos)                    {m_vPos=_vPos;};
  void SetMinEmitRate       (float _fMinEmitRate)                   {m_fMinEmitRate = _fMinEmitRate;};
  void SetMaxEmitRate       (float _fMaxEmitRate)                   {m_fMaxEmitRate = _fMaxEmitRate;};
  void SetColor1            (D3DXCOLOR& _Color1)                    {m_Color1 = _Color1;};
  void SetColor2            (D3DXCOLOR& _Color2)                    {m_Color2 = _Color2;};
  void SetMinSize           (float _fMinSize)                       {m_fMinSize = _fMinSize;};
  void SetMaxSize           (float _fMaxSize)                       {m_fMaxSize = _fMaxSize;};
  void SetSpawnDir1         (D3DXVECTOR3& _vSpawnDir1)              {m_vSpawnDir1 = _vSpawnDir1;};
  void SetSpawnDir2         (D3DXVECTOR3& _vSpawnDir2)              {m_vSpawnDir2 = _vSpawnDir2;};
  void SetNumNewPartsExcess (float _fNumNewPartsExcess)             {m_fNumNewPartsExcess = _fNumNewPartsExcess;};
  void SetTexture           (string& _sTexture)                     {m_sTexture = _sTexture;};
  void SetDevice            (LPDIRECT3DDEVICE9 _pd3dDevice)         {m_pd3dDevice = _pd3dDevice;};
  void SetParticle          (LPDIRECT3DVERTEXBUFFER9 _vbParticles)  {m_vbParticles = _vbParticles;};


  //Get
  const D3DXVECTOR3&      GetPosition         () const {return m_vPos;};
  float                   GetMinEmitRate      () const {return m_fMinEmitRate;};
  float                   GetMaxEmitRate      () const {return m_fMaxEmitRate;};
  const D3DXCOLOR&        GetColor1           () const {return m_Color1;};
  const D3DXCOLOR&        GetColor2           () const {return m_Color2;};
  float                   GetMinSize          () const {return m_fMinSize;};
  float                   GetMaxSize          () const {return m_fMaxSize;};
  const D3DXVECTOR3&      GetSpawnDir1        () const {return m_vSpawnDir1;};
  const D3DXVECTOR3&      GetSpawnDir2        () const {return m_vSpawnDir2;};
  float                   GetNumNewPartsExcess() const {return m_fNumNewPartsExcess;};
  const string&           GetTexture          () const {return m_sTexture;};
  LPDIRECT3DDEVICE9       GetDevice           () const {return m_pd3dDevice;};
  LPDIRECT3DVERTEXBUFFER9 GetParticle         () const {return m_vbParticles;};


  void                    Update              (float fElapsedTime);
  void                    Init                (CRenderManager* rm, const string& _texureFileName);
  void                    Release             ();
  void                    Render              ();

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
  CRecyclingArray<CParticle>                m_Particles;
};
#endif
