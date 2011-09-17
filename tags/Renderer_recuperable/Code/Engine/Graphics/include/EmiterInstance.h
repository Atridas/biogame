#pragma once
#ifndef __EMITER_INSTANCE_H__
#define __EMITER_INSTANCE_H__
#include "base.h"
#include "Utils\RecyclingArray.h"

#include "Particle.h"
#include "VertexsStructs.h"
#include "InstancedData.h"
#include "ParticleConstants.h"
#include "Utils\Object3D.h"

// ----------------------------
class CEmiterCore;
class CRenderManager;
class CRoom;
// ----------------------------

class CEmiterInstance:
   public CBaseControl,
   public CObject3DRenderable
{
public:
  CEmiterInstance():
      m_pEmiterCore(0),
      m_RecyclingParticles(DEFAULT_MAX_PARTICLES_PER_EMITER),
      m_iaParticles(0),
      m_fTimeToNextParticle(0),
      m_fTimeToAwakeOrSleep(0),
      m_iActiveParticles(0),
      m_vMinVolume(-.5f), m_vMaxVolume(.5f),
      m_fVolume(1),
      m_pObjectReference(0),
      m_pInRoom(0),
      m_bAwake(true),
      m_bActive(true)
      {};
  virtual ~CEmiterInstance() {Done();};

  bool Init ( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume, int _iMaxParticles = DEFAULT_MAX_PARTICLES_PER_EMITER, bool _bBillboardMode = false );
  bool Reset( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume, int _iMaxParticles, bool _bBillboardMode )
    { Release(); SetOk(false); return Init(_szCoreName, _Position, _vVolume, m_iMaxParticles, m_bBillboardMode); };
  bool Reset(                                                                                                                         ) { return Reset( m_szCoreName, *this    , m_vVolume, m_iMaxParticles, m_bBillboardMode );};
  bool Reset( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume                                           ) { return Reset(  _szCoreName, _Position,  _vVolume, m_iMaxParticles, m_bBillboardMode );};
  bool Reset( const string& _szCoreName                                                                                               ) { return Reset(  _szCoreName, *this    , m_vVolume, m_iMaxParticles, m_bBillboardMode );};
  bool Reset(                                                        const Vect3f& _vVolume                                           ) { return Reset( m_szCoreName, *this    ,  _vVolume, m_iMaxParticles, m_bBillboardMode );};
  bool Reset(                                                                                int _iMaxParticles                       ) { return Reset( m_szCoreName, *this    , m_vVolume,  _iMaxParticles, m_bBillboardMode );};
  bool Reset(                                                                                                    bool _bBillboardMode ) { return Reset( m_szCoreName, *this    , m_vVolume, m_iMaxParticles,  _bBillboardMode );};

  void ChangePos(const CObject3D& _Position)  {SetMat44(_Position.GetMat44());};

  void Update(float _fDeltaTime);
  void Render(CRenderManager* _pRM, const vector<CEffect*>& _vEffects) { assert(_vEffects.size() == 1); Mat44f m; m.SetIdentity(); Render(_pRM,_vEffects[0],m); };
  void DebugRender(CRenderManager* _pRM, bool _bRenderBoundingSphere = false) { Mat44f m; m.SetIdentity(); DebugRender(_pRM,m,_bRenderBoundingSphere); };
  
  void Activate() { m_bActive = true; Reset(); }
  void Deactivate() { m_bActive = false; }

  void SetReference(CObject3D* _pObjectReference) {_pObjectReference = m_pObjectReference;};
  void SetRoom(CRoom* _pRoom) { m_pInRoom = _pRoom; };

  virtual const vector<CMaterial*>& GetMaterials() const;

protected:
  virtual void Release();

private:
  void DebugRender(CRenderManager* _pRM, const Mat44f& _mTransform, bool _bRenderBoundingSphere);
  void Render(CRenderManager* _pRM, CEffect* _pEffect, const Mat44f& _mTransform);

  bool         m_bActive;
  
  bool         m_bBillboardMode;
  CParticle    m_Billboard;
  int          m_iMaxParticles;

  bool                     m_bIsSimple;
  vector<CEmiterInstance*> m_ChildEmiters;

  CObject3D*         m_pObjectReference;
  CRoom*             m_pInRoom;
  const CEmiterCore* m_pEmiterCore;
  string             m_szCoreName;
  Vect3f             m_vVolume;
  Vect3f             m_vMinVolume, m_vMaxVolume;
  float              m_fVolume;

  float m_fTimeToNextParticle;

  float m_fTimeToAwakeOrSleep;
  bool  m_bAwake;

  int m_iActiveParticles; //nombre de partícules actives
  CRecyclingArray<CParticle>          m_RecyclingParticles;  //array de partícules en si
  int*                                m_iaParticles;         //array d'indexos a l'array anterior
  CInstancedData<SParticleRenderInfo> m_InstancedData;       //buffer a la gpu per renderitzar
};


#endif
