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
  m_fAngle    = 0.0f;

  //*******
  m_iTexNumFiles=0;
  m_iTexNumColumnes=0;
  m_fTimeAnimationDiapo=0.0f;
  m_bAnimated=true;
  

  m_fAU = 0.0f;
  m_fAV = 1.0f;
  m_fBU = 1.0f;
  m_fBV = 1.0f;
  m_fCU = 0.0f;
  m_fCV = 0.0f;
  m_fDU = 1.0f;
  m_fDV = 0.0f;
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
  //m_fAngle += fTimeDelta*20;
  m_fAngle += 0.01f;
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

  Vect3f l_cameraDirection = camera->GetDirection();
  Vect3f l_cameraVecUp = camera->GetVecUp();

 
  m_VDirection.x = l_cameraDirection.x;
  m_VDirection.y = l_cameraDirection.y;
  m_VDirection.z = l_cameraDirection.z;
    
  Mat33f mat;
  l_cameraVecUp = mat.FromAxisAngle(l_cameraDirection.Normalize(), m_fAngle)* l_cameraVecUp.Normalize();
  m_VUp.x = l_cameraVecUp.x;
  m_VUp.y = l_cameraVecUp.y;
  m_VUp.z = l_cameraVecUp.z;

      
	D3DXVec3Cross(&m_VRight, &m_VUp, &m_VDirection);// producte vectorial ok
      
  D3DXVec3Normalize(&m_VRight, &m_VRight);
  m_PointA = m_vPos - (m_VRight*m_fSize*0.5f) - (m_VUp*m_fSize*0.5f);
	m_PointB = m_vPos + (m_VRight*m_fSize*0.5f) - (m_VUp*m_fSize*0.5f);
	m_PointC = m_vPos - (m_VRight*m_fSize*0.5f) + (m_VUp*m_fSize*0.5f);
	m_PointD = m_vPos + (m_VRight*m_fSize*0.5f) + (m_VUp*m_fSize*0.5f);
  return true;
}