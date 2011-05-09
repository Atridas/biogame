#include "Particle.h"


CParticle::CParticle()
{
  m_fSize     = 1.0f;
  m_fLifetime = 2.0f;
  m_fAge      = 0.0f;
  m_Color     = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
  m_vPos      = D3DXVECTOR3(0.f, 0.f, 0.f);
  m_vDir      = D3DXVECTOR3(0.f, 0.f, 0.f);
  m_vVel      = D3DXVECTOR3(1.f, 1.1f, 1.f);
  m_vGravity  = D3DXVECTOR3(0.f, 0.0f, 0.f);
  //m_fAngle    =0.0f;
  m_PointA=(D3DXVECTOR3(0.0f,0.0f,0.0f));
  m_PointB=(D3DXVECTOR3(0.0f,0.0f,0.0f));
  m_PointC=(D3DXVECTOR3(0.0f,0.0f,0.0f)); 
  m_PointD=(D3DXVECTOR3(0.0f,0.0f,0.0f));
}

void CParticle::Release()
{
  m_vColor.clear();
  m_vTimeColor.clear();
  m_vDirection.clear();
  m_vTimeDirection.clear();

}

bool CParticle::Update(float fTimeDelta,CCamera* camera)
{

  m_fAge += fTimeDelta;
  //m_fAngle += fTimeDelta*5;
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
  
  //m_vPos=m_vPos+m_vDir*fTimeDelta;
  m_vPos.x= m_vPos.x+(m_vDir.x*m_vVel.x*fTimeDelta); 
  m_vPos.y= m_vPos.y+(m_vDir.y*m_vVel.y*fTimeDelta); 
  m_vPos.z= m_vPos.z+(m_vDir.z*m_vVel.z*fTimeDelta); 
  m_vVel= m_vVel+m_vGravity*fTimeDelta;

  Vect3f cameraDirection = camera->GetDirection();
  Vect3f cameraVecUp = camera->GetVecUp();

  D3DXVECTOR3 l_VDirection;
  D3DXVECTOR3 l_VUp;
  D3DXVECTOR3 l_VRight;
  l_VDirection.x = cameraDirection.x;
  l_VDirection.y = cameraDirection.y;
  l_VDirection.z = cameraDirection.z;

  l_VUp.x = cameraVecUp.x;
  l_VUp.y = cameraVecUp.y;
  l_VUp.z = cameraVecUp.z;
      
	D3DXVec3Cross(&l_VRight, &l_VUp, &l_VDirection);// producte vectorial ok
      
  D3DXVec3Normalize(&l_VRight, &l_VRight);
  m_PointA = m_vPos - (l_VRight*m_fSize*0.5f) - (l_VUp*m_fSize*0.5f);
	m_PointB = m_vPos + (l_VRight*m_fSize*0.5f) - (l_VUp*m_fSize*0.5f);
	m_PointC = m_vPos - (l_VRight*m_fSize*0.5f) + (l_VUp*m_fSize*0.5f);
	m_PointD = m_vPos + (l_VRight*m_fSize*0.5f) + (l_VUp*m_fSize*0.5f);
  return true;
}