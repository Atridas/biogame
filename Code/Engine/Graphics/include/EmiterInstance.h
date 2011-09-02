#pragma once
#ifndef __EMITER_INSTANCE_H__
#define __EMITER_INSTANCE_H__
#include "base.h"
#include "Utils\RecyclingArray.h"

#include "ParticleInstance.h"
#include "VertexsStructs.h"
#include "InstancedData.h"
#include "ParticleConstants.h"
#include "Utils\Object3D.h"

// ----------------------------
class CCoreEmiter;
class CRenderManager;
// ----------------------------

class CEmiterInstance:
   public CBaseControl,
   public CObject3D
{
public:
  CEmiterInstance():
      m_pCoreEmiter(0),
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
  void Reset( const CObject3D& _Position, const Vect3f& _vVolume );
  void Reset( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume )
    {m_szCoreName = _szCoreName; Reset(_Position, _vVolume);};
  void Reset( const Vect3f& _vVolume )
    {Reset(*this, _vVolume);};
  void Reset( const string& _szCoreName )
    {m_szCoreName = _szCoreName; Reset(*this, m_vMaxVolume - m_vMinVolume);};
  void Reset() { Reset(*this, m_vMaxVolume - m_vMinVolume); };

  void ChangePos(const CObject3D& _Position)
    {SetMat44(_Position.GetMat44());};

  void Update(float _fDeltaTime);
  void Render(CRenderManager* _pRM);
  void DebugRender(CRenderManager* _pRM);
  
  void Activate() { m_bActive = true; Reset(); }
  void Deactivate() { m_bActive = false; }

  void SetReference(CObject3D* _pObjectReference) {_pObjectReference = m_pObjectReference;};

protected:
  virtual void Release() {m_pObjectReference=0;};

private:

  bool         m_bActive;

  CObject3D*   m_pObjectReference;
  CCoreEmiter* m_pCoreEmiter;
  string       m_szCoreName;
  Vect3f       m_vMinVolume, m_vMaxVolume;
  float        m_fVolume;

  float m_fTimeToNextParticle;

  float m_fTimeToAwakeOrSleep;
  bool  m_bAwake;

  int m_iActiveParticles; //nombre de partícules actives
  CRecyclingArray<CParticleInstance>  m_RecyclingParticles;                  //array de partícules en si
  int                                 m_Particles[MAX_PARTICLES_PER_EMITER]; //array d'indexos a l'array anterior
  CInstancedData<SParticleRenderInfo> m_InstancedData;                       //buffer a la gpu per renderitzar
};


#endif
