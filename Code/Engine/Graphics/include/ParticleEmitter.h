#pragma once

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__
#define    NUMPARTICLES 500

#include "base.h"
#include <d3dx9.h>
#include "Utils\RecyclingArray.h"
#include "Particle.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>
#include "Camera.h"

struct SParticleInfo;


class CParticleEmitter:
  public CBaseControl
{
public:
  CParticleEmitter();
  ~CParticleEmitter(){Done(); Release();};
  
  

  //  Set
  void SetName              (string _szName)                        {m_szName = _szName;};
  void SetId                (string _szId)                          {m_szId = _szId;};
  void SetPosition          (D3DXVECTOR3& _vPos)                    {m_vPos=_vPos;};
  void SetMinEmitRate       (float _fMinEmitRate)                   {m_fMinEmitRate = _fMinEmitRate;};
  void SetMaxEmitRate       (float _fMaxEmitRate)                   {m_fMaxEmitRate = _fMaxEmitRate;};
  void SetColor1            (D3DXCOLOR& _Color1)                    {m_Color1 = _Color1;};
  void SetColor2            (D3DXCOLOR& _Color2)                    {m_Color2 = _Color2;};
  void SetMinSize           (float _fMinSize)                       {m_fMinSize = _fMinSize;};
  void SetMaxSize           (float _fMaxSize)                       {m_fMaxSize = _fMaxSize;};
  void SetSpawnDir1         (D3DXVECTOR3& _vSpawnDir1)              {m_vSpawnDir1 = _vSpawnDir1;};
  void SetSpawnDir2         (D3DXVECTOR3& _vSpawnDir2)              {m_vSpawnDir2 = _vSpawnDir2;};
  void SetTexParticle       (CTexture* _pTexParticle)               {m_pTexParticle = _pTexParticle;};
  void SetNumNewPartsExcess (float _fNumNewPartsExcess)             {m_fNumNewPartsExcess = _fNumNewPartsExcess;};
  void SetParticle          (LPDIRECT3DVERTEXBUFFER9 _vbParticles)  {m_vbParticles = _vbParticles;};
  void SetActive            (bool _Active)                          {m_bActive = _Active;};
  void SetVel               (D3DXVECTOR3& _vVel)                    {m_vVel = _vVel;};
  void SetGravity           (D3DXVECTOR3& _vGravity)                {m_vGravity = _vGravity;};


  //void SetAngle             (float _fAngle)                         {m_fAngle = _fAngle;};
  //void SetReload            (bool _bReload)                         {m_bReload = _bReload;};

  //Get
  string                          GetName             () const {return m_szName;};
  string                          GetId               () const {return m_szId;};
  const D3DXVECTOR3&				      GetPosition         () const {return m_vPos;};
  float                           GetMinEmitRate      () const {return m_fMinEmitRate;};
  float                           GetMaxEmitRate      () const {return m_fMaxEmitRate;};
  const D3DXCOLOR&                GetColor1           () const {return m_Color1;};
  const D3DXCOLOR&                GetColor2           () const {return m_Color2;};
  float                           GetMinSize          () const {return m_fMinSize;};
  float                           GetMaxSize          () const {return m_fMaxSize;};
  const D3DXVECTOR3&              GetSpawnDir1        () const {return m_vSpawnDir1;};
  const D3DXVECTOR3&              GetSpawnDir2        () const {return m_vSpawnDir2;};
  CTexture*                       GetTexParticle      () const {return m_pTexParticle;};        
  float                           GetNumNewPartsExcess() const {return m_fNumNewPartsExcess;};
  const LPDIRECT3DVERTEXBUFFER9   GetParticle         () const {return m_vbParticles;};
  const bool                      GetActive           () const {return m_bActive;};
  const D3DXVECTOR3&              GetVel              () const {return m_vVel;};
  const D3DXVECTOR3&              GetGravity          () const {return m_vGravity;};
  //*********

  //float                           GetAngle            () const {return m_fAngle;};
  //bool                            GetReload           () const {return m_bReload;};



  void                    Update              (float fElapsedTime, CCamera* camera);
  void                    Init                (CRenderManager* rm);
  virtual void            Release             ();
  virtual void            Render              (CRenderManager* _pRM);
  virtual void            SetAttributes       (SParticleInfo* _info);

  
protected:
  string                                    m_szName;
  string                                    m_szId;
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
  CTexture*                                 m_pTexParticle;
  float                                     m_fLife1;
  float                                     m_fLife2;
  LPDIRECT3DVERTEXBUFFER9                   m_vbParticles;
  CRecyclingArray<CParticle>                m_Particles;
  vector<D3DXCOLOR>                         m_vColor;
  vector<D3DXCOLOR>                         m_vNewColor;
  vector<float>                             m_vTimeColor;
  vector<D3DXVECTOR3>                       m_vDirection;
  vector<D3DXVECTOR3>                       m_vNewDirection;
  vector<float>                             m_vTimeDirection;
  bool                                      m_bActive;
  D3DXVECTOR3                               m_vVel; //velocitat de la particula
  D3DXVECTOR3                               m_vGravity; // for�es gravitatories
  //float                                     m_fAngle;
  //bool                                      m_bReload;
  //CRenderManager*                           _pRM;
  float             m_fSizeX;
  float             m_fSizeY;
  bool                m_bAnimated;
  vector<int>           m_vFilesColumnes;//guarda el numero de files i de columnes que conte la textura animada
  vector<float>			    m_vTimeAnimated;//aqui guardem el temps en que pasa el event, i el temps de l'animacio
 /* D3DXVECTOR3		        m_PointA;
  D3DXVECTOR3            m_PointB;
  D3DXVECTOR3            m_PointC; 
  D3DXVECTOR3            m_PointD;*/

};


#endif