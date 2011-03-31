#include "Particle.h"


CParticle::CParticle()
{
  m_fSize     = 0.f;
  m_fLifetime = 0.f;
  m_fAge      = 0.f;
  m_Color     = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
  m_vPos      = D3DXVECTOR3(0.f, 0.f, 0.f);
  m_vDir      = D3DXVECTOR3(0.f, 0.f, 0.f);
}

void CParticle::Release()
{
  m_vColor.clear();
  m_vTimeColor.clear();
  m_vDirection.clear();
  m_vTimeDirection.clear();

}

bool CParticle::Update(float fTimeDelta)
{
  m_fAge += fTimeDelta;
  if (m_fAge >= m_fLifetime )
  {
    //its time to die..
    return false;
  }

  int i= m_vTimeColor.size()-1;
  while (i>=0)
  {
    if(m_vTimeColor[i]<m_fAge)
	 {
	   m_Color = m_vColor[i];
     i=0;
	 }
    i--;
  }
  //m_Color= m_Color;
  
  i= m_vTimeDirection.size()-1;
  while (i>=0)
  {
    if(m_vTimeDirection[i]<m_fAge)
	 {
     m_vDir = m_vDirection[i];
     i=0;
	 }
    i--;
  }

  m_vPos=m_vPos+m_vDir*fTimeDelta;
  return true;
}