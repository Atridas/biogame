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
// ----------------------------

class CEmiterInstance:
   public CBaseControl,
   public CObject3D
{
public:
  CEmiterInstance():
      m_pEmiterCore(0),
      m_RecyclingParticles(MAX_PARTICLES_PER_EMITER),
      m_fTimeToNextParticle(0),
      m_fTimeToAwakeOrSleep(0),
      m_iActiveParticles(0),
      m_vMinVolume(-.5f), m_vMaxVolume(.5f),
      m_fVolume(1),
      m_pObjectReference(0),
      m_bAwake(true),
      m_bActive(true)
      {};
  virtual ~CEmiterInstance() {Done();};

  bool Init( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume );
  bool Reset( const Vect3f& _vVolume ) { return Reset(m_szCoreName, *this, _vVolume );};
  bool Reset( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume )
    { Release(); SetOk(false); return Init(_szCoreName, _Position, _vVolume); };
  bool Reset( const string& _szCoreName )
    { Release(); SetOk(false); return Init(_szCoreName, *this, m_vVolume); };
  bool Reset()
    { Release(); SetOk(false); return Init(m_szCoreName, *this, m_vVolume); };

  void ChangePos(const CObject3D& _Position)
    {SetMat44(_Position.GetMat44());};

  void Update(float _fDeltaTime);
  void Render(CRenderManager* _pRM) { Mat44f m; m.SetIdentity(); Render(_pRM,m); };
  void DebugRender(CRenderManager* _pRM, bool _bRenderBoundingSphere = false) { Mat44f m; m.SetIdentity(); DebugRender(_pRM,m,_bRenderBoundingSphere); };
  
  void Activate() { m_bActive = true; Reset(); }
  void Deactivate() { m_bActive = false; }

  void SetReference(CObject3D* _pObjectReference) {_pObjectReference = m_pObjectReference;};

protected:
  virtual void Release();

private:
  void DebugRender(CRenderManager* _pRM, const Mat44f& _mTransform, bool _bRenderBoundingSphere);
  void Render(CRenderManager* _pRM, const Mat44f& _mTransform);

  bool         m_bActive;

  bool                     m_bIsSimple;
  vector<CEmiterInstance*> m_ChildEmiters;

  CObject3D*   m_pObjectReference;
  CEmiterCore* m_pEmiterCore;
  string       m_szCoreName;
  Vect3f       m_vVolume;
  Vect3f       m_vMinVolume, m_vMaxVolume;
  float        m_fVolume;

  float m_fTimeToNextParticle;

  float m_fTimeToAwakeOrSleep;
  bool  m_bAwake;

  int m_iActiveParticles; //nombre de partícules actives
  CRecyclingArray<CParticle>          m_RecyclingParticles;                  //array de partícules en si
  int                                 m_Particles[MAX_PARTICLES_PER_EMITER]; //array d'indexos a l'array anterior
  CInstancedData<SParticleRenderInfo> m_InstancedData;                       //buffer a la gpu per renderitzar
};


#endif
