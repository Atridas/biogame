#include "Particle.h"
#include "base.h"
#include "ParticleEmitter.h"


CParticle::CParticle():
  m_fSize     (1.0f),
  m_fLifetime (2.0f),
  m_fAge      (0.0f),
  m_Color     (1.f, 1.f, 1.f, 1.f),
  m_vPos      (0.f, 0.f, 0.f),
  m_vDir      (0.f, 0.f, 0.f),
  m_vVel      (1.f, 1.1f, 1.f),
  m_vGravity  (0.f, 0.0f, 0.f),
  m_PointA(0.0f,0.0f,0.0f),
  m_PointB(0.0f,0.0f,0.0f),
  m_PointC(0.0f,0.0f,0.0f),
  m_PointD(0.0f,0.0f,0.0f),
  m_fAngle    (0.0f),
  m_fIncrementAngle(0.0f),
  m_fTimeInterpolation(0.0f),

  m_fAU ( 0.0f),
  m_fAV ( 1.0f),
  m_fBU ( 1.0f),
  m_fBV ( 1.0f),
  m_fCU ( 0.0f),
  m_fCV ( 0.0f),
  m_fDU ( 1.0f),
  m_fDV ( 0.0f)
{
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

bool CParticle::UpdateState(float _fDeltaTime)
{
  //Actualitzem el temps
  m_fAge += _fDeltaTime;
  //m_fAngle += 0.01f;
  if (m_fAge >= m_fLifetime )
  {
    //its time to die..
    return false;
  }
  
  // actualitzem l'angle de rotació
  m_fIncrementAngle = m_fIncrementAngle+(m_fAngle*_fDeltaTime);
  m_fIncrementAngle = (m_fIncrementAngle > FLOAT_PI_VALUE * 2) ? m_fIncrementAngle - FLOAT_PI_VALUE * 2 : m_fIncrementAngle;

  //****************************** Actualitzem el Color ************************************************************
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
          m_Color = InterPolaterNumber(m_Color,m_vColor[i+1],m_fTimeInterpolation,_fDeltaTime);
        }
      }
      m_vColor[i]=m_Color;
    
      i=0;
	  }
    i--;
  }

  //************************************** Actualitzem la direcció *************************************************
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
        m_vDir = InterPolaterNumber(m_vDir,m_vDirection[i+1],m_fTimeInterpolation,_fDeltaTime);
      }
      }
      m_vDirection[i]=m_vDir;
      i=0;
	  }
    i--;
  }

  //**************************************** Actualitza el tamany **************************************************
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
        m_fSize = InterPolaterNumber(m_fSize,m_vSize[i+1],m_fTimeInterpolation,_fDeltaTime);
      }
     }
     m_vSize[i]=m_fSize;
     i=0;
	 }
    i--;
  }
  //****************************************************************************************************************

  
  //m_vPos=m_vPos+m_vDir*fTimeDelta;
  m_vPos.x= m_vPos.x+(m_vDir.x*m_vVel.x*_fDeltaTime); 
  m_vPos.y= m_vPos.y+(m_vDir.y*m_vVel.y*_fDeltaTime); 
  m_vPos.z= m_vPos.z+(m_vDir.z*m_vVel.z*_fDeltaTime); 
  m_vVel = m_vVel + m_vGravity * _fDeltaTime;

  
		
  //***** NOMES SI ES ANIMADA
  if(m_bAnimated)
  {
	  m_fTimeAnimationActual += _fDeltaTime;


    if(m_fTimeAnimationActual>m_fTimeAnimationDiapo)
    {
      m_iNumDiapo++;
      m_fTimeAnimationActual=0;
      if(m_iNumDiapo>m_iTotalDiapos)
      {
        m_iNumDiapo=1;
      }
    }
  }

  return true;
}

void CParticle::UpdateBillboard(CCamera* _pCamera)
{
  Vect3f l_cameraDirection = _pCamera->GetDirection();
  Vect3f l_cameraVecUp = _pCamera->GetVecUp();

 
  m_VDirection = l_cameraDirection.GetNormalized();
    
  Mat33f mat;
  l_cameraVecUp = mat.FromAxisAngle(l_cameraDirection.Normalize(), m_fIncrementAngle)* l_cameraVecUp.Normalize();
  m_VUp = l_cameraVecUp.GetNormalized();
      
	//D3DXVec3Cross(&m_VRight, &m_VUp, &m_VDirection);// producte vectorial ok
  m_VRight = (m_VUp ^ m_VDirection).GetNormalized();

  m_PointA = m_vPos - (m_VRight*m_fSize*0.5f) - (m_VUp*m_fSize*0.5f);
	m_PointB = m_vPos + (m_VRight*m_fSize*0.5f) - (m_VUp*m_fSize*0.5f);
	m_PointC = m_vPos - (m_VRight*m_fSize*0.5f) + (m_VUp*m_fSize*0.5f);
	m_PointD = m_vPos + (m_VRight*m_fSize*0.5f) + (m_VUp*m_fSize*0.5f);

  
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
  
  //float l_fSin = sin(m_iIncrementAngle + FLOAT_PI_VALUE * .25f);
  //float l_fCos = cos(m_iIncrementAngle + FLOAT_PI_VALUE * .25f);
  //
  //float l_fSqrt2 = sqrt(2.f);
  //
  //m_PointA = m_vPos - (m_fSize*l_fSqrt2*l_fSin*m_VRight) - (m_fSize*l_fSqrt2*l_fCos*m_VUp);
  //m_PointB = m_vPos + (m_fSize*l_fSqrt2*l_fCos*m_VRight) - (m_fSize*l_fSqrt2*l_fSin*m_VUp);
  //m_PointC = m_vPos - (m_fSize*l_fSqrt2*l_fSin*m_VRight) + (m_fSize*l_fSqrt2*l_fCos*m_VUp);
  //m_PointD = m_vPos + (m_fSize*l_fSqrt2*l_fCos*m_VRight) + (m_fSize*l_fSqrt2*l_fSin*m_VUp);
}

bool CParticle::Update(float _fDeltaTime, CCamera* _pCamera)
{
  if(!UpdateState(_fDeltaTime))
  {
    return false;
  }

#ifndef __PARTICLE_VIA_SHADER__
  UpdateBillboard(_pCamera);
#endif
  
  return true;
}


void  CParticle::FillInstanceData(SParticleRenderInfo* data)
{
  data->x = m_vPos.x;
  data->y = m_vPos.y;
  data->z = m_vPos.z;
  data->sizeX = 
  data->sizeY = m_fSize;

  //data->u = 
  data->angleCos = cos(m_fIncrementAngle + FLOAT_PI_VALUE * .25f);
  data->angleSin = sin(m_fIncrementAngle + FLOAT_PI_VALUE * .25f);
  //data->active = (m_fAge >= m_fLifetime) ? 0.f : 1.f;
  
  if(m_bAnimated)
    data->diapo = (float)m_iNumDiapo - 1.f;
  else
    data->diapo = 0.f;
  //data->minU = m_fCU;
  //data->minV = m_fCV;
  //data->maxU = m_fBU;
  //data->maxV = m_fBV;
  
  data->color=(DWORD)GetColor();
}
