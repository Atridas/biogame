#include "VertexsStructs.h"

#include "ParticleInstance.h"


void CParticleInstance::Init(CCoreEmiter* _pCoreEmiter, const Vect3f& _vInitialPosition)
{
  m_pCoreEmiter = _pCoreEmiter;

  m_fLivedTime = 0.f;
  m_fTotalLifetime = m_pCoreEmiter->GetLife();

  m_vSpeed = m_pCoreEmiter->GetStartingSpeed();
  m_fAngularSpeed = m_pCoreEmiter->GetStartingAngularSpeed();
  
  m_vAcceleration = m_pCoreEmiter->GetAcceleration();
  m_fAngularAcceleration = m_pCoreEmiter->GetAngularAcceleration();

  m_iCurrentColor = 
  m_iCurrentSize  = 0;

  m_fLastColorTime =
  m_fLastSizeTime  = 0;

  m_LastColor = m_pCoreEmiter->GetColor(0);
  m_fLastSize = m_pCoreEmiter->GetSize(0);

  if(m_pCoreEmiter->GetNumColorFrames() > m_iCurrentColor + 1)
  {
    m_bHasNextColor  = true;
    m_NextColor      = m_pCoreEmiter->GetColor(1);
    m_fNextColorTime = m_pCoreEmiter->GetColorControlTime(1);
  }
  else
  {
    m_bHasNextColor  = false;
    //m_NextColor      = m_LastColor;
    //m_fNextColorTime = m_fLastColorTime;
  }

  if(m_pCoreEmiter->GetNumSizeFrames() > m_iCurrentSize + 1)
  {
    m_bHasNextSize  = true;
    m_fNextSize     = m_pCoreEmiter->GetSize(1);
    m_fNextSizeTime = m_pCoreEmiter->GetSizeControlTime(1);
  }
  else
  {
    m_bHasNextSize  = false;
    //m_fNextSize     = m_LastSize;
    //m_fNextSizeTime = m_fLastSizeTime;
  }


  m_vPosition = _vInitialPosition;
  m_fAngle    = m_pCoreEmiter->GetStartingAngle();
  m_iFrame    = 0;
  m_Color     = m_LastColor;
  m_fSize     = m_fLastSize;
}

bool CParticleInstance::Update(float _fDeltaTime)
{
  m_fLivedTime += _fDeltaTime;
  if(m_fLivedTime > m_fTotalLifetime)
    return false;

  m_vSpeed        += m_vAcceleration        * _fDeltaTime;
  m_fAngularSpeed += m_fAngularAcceleration * _fDeltaTime;
  
  m_vPosition     += m_vSpeed        * _fDeltaTime;
  m_fAngle        += m_fAngularSpeed * _fDeltaTime;

  // diapositives -------------------------------------------------------------------------------------------------
  if(m_pCoreEmiter->IsSprite())
  {
    m_iFrame = (int)floor(m_fLivedTime / m_pCoreEmiter->GetTimePerFrame());
    int l_iNumFrames = m_pCoreEmiter->GetNumFrames();
    m_iFrame = (m_iFrame >= l_iNumFrames)? l_iNumFrames - 1 : m_iFrame;
  }
  
  //Color ----------------------------------------------------------------------------------------------
  if(m_bHasNextColor)
  {
    if(m_fLivedTime > m_fNextColorTime)
    {
      m_iCurrentColor++;
      m_LastColor      = m_NextColor;
      m_fLastColorTime = m_fNextColorTime;
      
      if(m_pCoreEmiter->GetNumColorFrames() > m_iCurrentColor + 1)
      {
        m_bHasNextColor  = true;
        m_NextColor      = m_pCoreEmiter->GetColor           (m_iCurrentColor + 1);
        m_fNextColorTime = m_pCoreEmiter->GetColorControlTime(m_iCurrentColor + 1);


        float l_fInterpolationFactor = (m_fLivedTime - m_fLastColorTime) / (m_fNextColorTime - m_fLastColorTime);
        m_Color = SIMPLE_INTERPOLATION(m_LastColor, m_NextColor, l_fInterpolationFactor);
      }
      else
      {
        m_bHasNextColor  = false;
        m_Color = m_LastColor;
      }
    }
    else
    {
      float l_fInterpolationFactor = (m_fLivedTime - m_fLastColorTime) / (m_fNextColorTime - m_fLastColorTime);
      m_Color = SIMPLE_INTERPOLATION(m_LastColor, m_NextColor, l_fInterpolationFactor);
    }
  }
  //else //el color no canvia
  //{
  //  m_Color = m_LastColor;
  //}


  //Size ----------------------------------------------------------------------------------------------
  if(m_bHasNextSize)
  {
    if(m_fLivedTime > m_fNextSizeTime)
    {
      m_iCurrentSize++;
      m_fLastSize     = m_fNextSize;
      m_fLastSizeTime = m_fNextSizeTime;
      
      if(m_pCoreEmiter->GetNumSizeFrames() > m_iCurrentSize + 1)
      {
        m_bHasNextSize  = true;
        m_fNextSize     = m_pCoreEmiter->GetSize           (m_iCurrentSize + 1);
        m_fNextSizeTime = m_pCoreEmiter->GetSizeControlTime(m_iCurrentSize + 1);


        float l_fInterpolationFactor = (m_fLivedTime - m_fLastSizeTime) / (m_fNextSizeTime - m_fLastSizeTime);
        m_fSize = SIMPLE_INTERPOLATION(m_fLastSize, m_fNextSize, l_fInterpolationFactor);
      }
      else
      {
        m_bHasNextSize  = false;
        m_fSize = m_fLastSize;
      }
    }
    else
    {
      float l_fInterpolationFactor = (m_fLivedTime - m_fLastSizeTime) / (m_fNextSizeTime - m_fLastSizeTime);
      m_fSize = SIMPLE_INTERPOLATION(m_fLastSize, m_fNextSize, l_fInterpolationFactor);
    }
  }
  //else // el tamany no canvia
  //{
  //   m_fSize = m_fLastSize;
  //}


  return true;
}


void CParticleInstance::FillRenderInfo(SParticleRenderInfo& Info_)
{
  Info_.x = m_vPosition.x;
  Info_.y = m_vPosition.y;
  Info_.z = m_vPosition.z;
  
  Info_.sizeX =
  Info_.sizeY = m_fSize;
  
  Info_.angleSin = sin(m_fAngle);
  Info_.angleCos = cos(m_fAngle);
    
  Info_.diapo = (float) m_iFrame;
  Info_.color = DWORD (D3DXCOLOR(m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha()) );
}