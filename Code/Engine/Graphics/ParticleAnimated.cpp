#include "include\ParticleAnimated.h"


CParticleAnimated::CParticleAnimated()
{
  CParticle::CParticle();
  m_iTexNumFiles = 0;
  m_iTexNumColumnes = 0;
  m_fTimeAnimationActual =0.0f;
  m_fTimeAnimationDiapo =0.0f;
  m_iNumDiapo = 0;
  m_fIncrementV = 0.0f;
  m_fIncrementU = 0.0f;
  m_iTotalDiapos = 0;
}



bool  CParticleAnimated::Update(float fTimeDelta, CCamera* camera)
{

  int i= m_vFilesColumnes.size()-1;
  while (i>=0)
  {
    if(m_vTimeAnimated[i]<m_fAge)
	 {

     m_iTexNumFiles=m_vFilesColumnes[i-1];
     m_iTexNumColumnes=m_vFilesColumnes[i];
     m_fTimeAnimationDiapo=m_vTimeAnimated[i];
	   i=0;
	 }
    i=i-2;
  }
  m_fIncrementV = //*(float)m_pTexParticle->GetHeight();*/ 256.f/m_iTexNumFiles;
  m_fIncrementV= m_fIncrementV/256.f;
  m_fIncrementU = /*(float)m_pTexParticle->GetWidth()*/256.f/m_iTexNumColumnes;
  m_fIncrementU = m_fIncrementU/256.f;
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
  /*m_fAU = m_fIncrementU*(l_Columna-1);
  m_fAV = m_fIncrementV*(fila-1);//1- (m_fIncrementV*(fila-1));

  m_fBU = m_fAU+m_fIncrementU;
  m_fBV = m_fAV;

  m_fCU = m_fAU;
  m_fCV = m_fAV-m_fIncrementV;
   
  m_fDU = m_fBU;
  m_fDV = m_fCV;*/

  CParticle::m_fAU = m_fIncrementU*(l_Columna-1);
  CParticle::m_fAV = m_fIncrementV*(fila-1);//1- (m_fIncrementV*(fila-1));

  CParticle::m_fBU = m_fAU+m_fIncrementU;
  CParticle::m_fBV = m_fAV;

  CParticle::m_fCU = m_fAU;
  CParticle::m_fCV = m_fAV-m_fIncrementV;

  CParticle::m_fDU = m_fBU;
  CParticle::m_fDV = m_fCV;

 return true;
}