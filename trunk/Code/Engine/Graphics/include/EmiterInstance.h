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
      m_iActiveParticles(0),
      m_vMinVolume(-.5f), m_vMaxVolume(.5f),
      m_fVolume(1)
      {};
  virtual ~CEmiterInstance() {Done();};

  bool Init( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume );
  void Reset( const CObject3D& _Position, const Vect3f& _vVolume );
  void Reset( const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume )
    {m_szCoreName = _szCoreName; Reset(_Position, _vVolume);};
  void Reset( const string& _szCoreName )
    {m_szCoreName = _szCoreName; Reset(*this, m_vMaxVolume - m_vMinVolume);};
  void Reset() { Reset(*this, m_vMaxVolume - m_vMinVolume); };

  void Update(float _fDeltaTime);
  void Render(CRenderManager* _pRM);
  void DebugRender(CRenderManager* _pRM);

protected:
  virtual void Release() {};

private:

  CCoreEmiter* m_pCoreEmiter;
  string       m_szCoreName;
  Vect3f       m_vMinVolume, m_vMaxVolume;
  float        m_fVolume;

  float m_fTimeToNextParticle;

  int m_iActiveParticles; //nombre de partícules actives
  CRecyclingArray<CParticleInstance>  m_RecyclingParticles;                  //array de partícules en si
  int                                 m_Particles[MAX_PARTICLES_PER_EMITER]; //array d'indexos a l'array anterior
  CInstancedData<SParticleRenderInfo> m_InstancedData;                       //buffer a la gpu per renderitzar
};


#endif
