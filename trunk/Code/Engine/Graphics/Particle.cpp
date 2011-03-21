#include "Particle.h"


CParticle::CParticle(void)
{
  m_fSize     = 0.f;
  m_fLifetime = 0.f;
  m_fAge      = 0.f;
  m_Color     = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
  m_vPos      = D3DXVECTOR3(0.f, 0.f, 0.f);
  m_vDir      = D3DXVECTOR3(0.f, 0.f, 0.f);
}


CParticle::~CParticle(void)
{
}

bool CParticle::Update(float fTimeDelta)
{
  m_fAge += fTimeDelta;
  if (m_fAge >= m_fLifetime )
  {
    //its time to die..
    return false;
  }

  int i= m_vTime.size()-1;
  while (i>=0)
  {
     if(m_vTime[i]<m_fAge)
	 {
	   m_Color = m_vColor[i];
	 }
    i--;
  }

  m_vPos=m_vPos+m_vDir*fTimeDelta;
  return true;
}