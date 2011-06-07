#include "Particle.h"
#include "base.h"


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
  m_iIncrementAngle=0.0f;
  m_fTimeInterpolation=0.0f;

  //*******
/*  m_iTexNumFiles=0;
  m_iTexNumColumnes=0;
  m_fTimeAnimationDiapo=0.0f;
  m_bAnimated=true;*/
  

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
  m_vTimeColorInterpolation.clear();
  m_vDirection.clear();
  m_vTimeDirection.clear();
  m_vTimeDirectionInterpolation.clear();
  m_vFilesColumnes.clear();
  m_vTimeAnimated.clear();
  m_vTimeAnimatedInterpolation.clear();
  m_vSize.clear();
  m_vTimeSize.clear();
  m_vTimeSizeInterpolation.clear();

}

bool CParticle::Update(float fTimeDelta,CCamera* camera)
{

  m_fAge += fTimeDelta;
  m_iIncrementAngle = m_iIncrementAngle+(m_fAngle*fTimeDelta);
  //m_fAngle += 0.01f;
  if (m_fAge >= m_fLifetime )
  {
    //its time to die..
    return false;
  }

  int i= m_vTimeColor.size()-1;
  int i_aux=i;
  while (i>=0)
  {
    if(m_vTimeColor[i]<m_fAge)
	 {
	   m_Color = m_vColor[i];
      if(i<i_aux)
     {
       float l_time_aux=m_vTimeColor[i+1]-m_fAge;
       m_fTimeInterpolation=m_vTimeColorInterpolation[i];
      if(l_time_aux<m_fTimeInterpolation)
      {
        m_Color = InterPolaterNumber(m_Color,m_vColor[i+1],m_fTimeInterpolation,fTimeDelta);
      }
     }
     m_vColor[i]=m_Color;
    
     i=0;
	 }
    i--;
  }
  //m_Color= m_Color;
  
  i= m_vTimeDirection.size()-1;
  i_aux=i;
  while (i>=0)
  {
    if(m_vTimeDirection[i]<m_fAge)
	 {
     m_vDir = m_vDirection[i];

     // nomes per interpolar
     
     if(i<i_aux)
     {
      float l_time_aux=m_vTimeDirection[i+1]-m_fAge;
      m_fTimeInterpolation=m_vTimeDirectionInterpolation[i];
      if(l_time_aux<m_fTimeInterpolation)
      {
        m_vDir = InterPolaterNumber(m_vDir,m_vDirection[i+1],m_fTimeInterpolation,fTimeDelta);
      }
     }
     m_vDirection[i]=m_vDir;
     i=0;
	 }
    i--;
  }

  //**************
  i= m_vTimeSize.size()-1;
  i_aux=i;
  while (i>=0)
  {
    if(m_vTimeSize[i]<m_fAge)
	 {
     m_fSize = m_vSize[i];

     // nomes per interpolar
     
     if(i<i_aux)
     {
      float l_time_aux=m_vTimeSize[i+1]-m_fAge;
      m_fTimeInterpolation=m_vTimeSizeInterpolation[i];
      if(l_time_aux<m_fTimeInterpolation)
      {
        m_fSize = InterPolaterNumber(m_fSize,m_vSize[i+1],m_fTimeInterpolation,fTimeDelta);
      }
     }
     m_vSize[i]=m_fSize;
     i=0;
	 }
    i--;
  }
  //******************
  
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
  l_cameraVecUp = mat.FromAxisAngle(l_cameraDirection.Normalize(), m_iIncrementAngle)* l_cameraVecUp.Normalize();
  m_VUp.x = l_cameraVecUp.x;
  m_VUp.y = l_cameraVecUp.y;
  m_VUp.z = l_cameraVecUp.z;

      
	D3DXVec3Cross(&m_VRight, &m_VUp, &m_VDirection);// producte vectorial ok
      
  D3DXVec3Normalize(&m_VRight, &m_VRight);
  m_PointA = m_vPos - (m_VRight*m_fSize*0.5f) - (m_VUp*m_fSize*0.5f);
	m_PointB = m_vPos + (m_VRight*m_fSize*0.5f) - (m_VUp*m_fSize*0.5f);
	m_PointC = m_vPos - (m_VRight*m_fSize*0.5f) + (m_VUp*m_fSize*0.5f);
	m_PointD = m_vPos + (m_VRight*m_fSize*0.5f) + (m_VUp*m_fSize*0.5f);


  //***** NOMES SI ES ANIMADA
  if(m_bAnimated)
  {
    int i= m_vFilesColumnes.size()-1;
    //int j=i
    while (i>=0)
    {
      if(m_vTimeAnimated[i]<m_fAge)
	   {
       
       m_iTexNumFiles=m_vFilesColumnes[i-1];
       m_iTexNumColumnes=m_vFilesColumnes[i];
       m_fTimeAnimationDiapo=m_vTimeAnimated[i];
       //m_pTexParticle=m_vTextureAnimation[i]; aki resta de 2 en 2 i nomes te una textura per temsp, per aixo peta
	     i=0;
	   }
      i=i-2;
    }
    m_fIncrementV = (float)m_pTexParticle->GetHeight()/m_iTexNumFiles;
    m_fIncrementV= m_fIncrementV/m_pTexParticle->GetHeight();
    m_fIncrementU = (float)m_pTexParticle->GetWidth()/m_iTexNumColumnes;
    m_fIncrementU = m_fIncrementU/m_pTexParticle->GetHeight();
	  m_iTotalDiapos=m_iTexNumFiles*m_iTexNumColumnes;
	  int l_canviDiapo=1;
		
	  m_fTimeAnimationActual += fTimeDelta;


    if(m_fTimeAnimationActual>m_fTimeAnimationDiapo)
    {
      m_iNumDiapo++;
      m_fTimeAnimationActual=0;
    }


    if(m_iNumDiapo>m_iTotalDiapos)
    {
      m_iNumDiapo=1;
      m_fTimeAnimationActual=0;
    }
    //Per saber en quina posició esta la diapositiva que volem ensenyar
    bool l_bOk=false;
    int l_Columna=0, fila=2, AuxNumDiapo;
    AuxNumDiapo=m_iNumDiapo;
    while (l_bOk==false)
    {
      if(m_iTexNumColumnes<AuxNumDiapo)
      {
        AuxNumDiapo -= m_iTexNumColumnes;
        fila++;
      }else
      {
        l_Columna = AuxNumDiapo;
        l_bOk=true;
      }
	  }

    //Extreure les cordenades de textura concretas
    m_fAU = m_fIncrementU*(l_Columna-1);
    m_fAV = m_fIncrementV*(fila-1);//1- (m_fIncrementV*(fila-1));

    m_fBU = m_fAU+m_fIncrementU;
    m_fBV = m_fAV;

    m_fCU = m_fAU;
    m_fCV = m_fAV-m_fIncrementV;
   
    m_fDU = m_fBU;
    m_fDV = m_fCV;

    
    /*CParticle::m_fAU = m_fIncrementU*(l_Columna-1);
    CParticle::m_fAV = m_fIncrementV*(fila-1);//1- (m_fIncrementV*(fila-1));

    CParticle::m_fBU = m_fAU+m_fIncrementU;
    CParticle::m_fBV = m_fAV;

    CParticle::m_fCU = m_fAU;
    CParticle::m_fCV = m_fAV-m_fIncrementV;

    CParticle::m_fDU = m_fBU;
    CParticle::m_fDV = m_fCV;*/

  }else
  {
    m_fAU = 0.0f;
    m_fAV = 1.0f;
    m_fBU = 1.0f;
    m_fBV = 1.0f;
    m_fCU = 0.0f;
    m_fCV = 0.0f;
    m_fDU = 1.0f;
    m_fDV = 0.0f;
  }

  return true;
}