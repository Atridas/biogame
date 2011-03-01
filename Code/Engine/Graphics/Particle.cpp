#include "Particle.h"


CParticle::CParticle(void)
{
}


CParticle::~CParticle(void)
{
}

bool CParticle::Update(float fTimeDelta)
{
  m_vPos=m_vPos+m_vDir*fTimeDelta;
  return true;
}