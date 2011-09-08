#include "VertexsStructs.h"

#include "Particle.h"


void CParticle::Init(const CSimpleEmiterCore* _pEmiterCore, const Vect3f& _vInitialPosition, bool _bBillboardMode)
{
  m_pEmiterCore = _pEmiterCore;
  m_bBillboardMode = _bBillboardMode;

  m_fLivedTime =
  m_fColorAnimationTime =
  m_fSizeAnimationTime = 0.f;

  if(!m_bBillboardMode)
  {
    m_fTotalLifetime = m_pEmiterCore->GetLife();


    m_vSpeed = m_pEmiterCore->GetStartingSpeed();
    m_fAngularSpeed = m_pEmiterCore->GetStartingAngularSpeed();
  
    m_vAcceleration = m_pEmiterCore->GetAcceleration();
    m_fAngularAcceleration = m_pEmiterCore->GetAngularAcceleration();
  }


  m_iCurrentColor = 
  m_iCurrentSize  = 0;

  m_fLastColorTime =
  m_fLastSizeTime  = 0;

  m_LastColor = m_pEmiterCore->GetColor(0);
  m_fLastSize = m_pEmiterCore->GetSize(0);

  if(m_pEmiterCore->GetNumColorFrames() > m_iCurrentColor + 1)
  {
    m_bHasNextColor  = true;
    m_NextColor      = m_pEmiterCore->GetColor(1);
    m_fNextColorTime = m_pEmiterCore->GetColorControlTime(1);
  }
  else
  {
    m_bHasNextColor  = false;
    //m_NextColor      = m_LastColor;
    //m_fNextColorTime = m_fLastColorTime;
  }

  if(m_pEmiterCore->GetNumSizeFrames() > m_iCurrentSize + 1)
  {
    m_bHasNextSize  = true;
    m_fNextSize     = m_pEmiterCore->GetSize(1);
    m_fNextSizeTime = m_pEmiterCore->GetSizeControlTime(1);
  }
  else
  {
    m_bHasNextSize  = false;
    //m_fNextSize     = m_LastSize;
    //m_fNextSizeTime = m_fLastSizeTime;
  }
  
  if(m_pEmiterCore->HasResetColorAnimation())
  {
    m_fTimeToResetColor = m_pEmiterCore->GetResetColorAnimationTime();
  }
  if(m_pEmiterCore->HasResetSizeAnimation())
  {
    m_fTimeToResetSize = m_pEmiterCore->GetResetSizeAnimationTime();
  }


  m_vPosition = _vInitialPosition;
  m_fAngle    = m_pEmiterCore->GetStartingAngle();
  m_iFrame    = 0;
  m_Color     = m_LastColor;
  m_fSize     = m_fLastSize;
}

bool CParticle::Update(float _fDeltaTime)
{
  m_fLivedTime += _fDeltaTime;
  m_fColorAnimationTime += _fDeltaTime;
  m_fSizeAnimationTime += _fDeltaTime;

  if(!m_bBillboardMode)
  {
    if(m_fLivedTime > m_fTotalLifetime)
      return false;
  

    m_vSpeed        += m_vAcceleration        * _fDeltaTime;
    m_fAngularSpeed += m_fAngularAcceleration * _fDeltaTime;
  
    m_vPosition     += m_vSpeed        * _fDeltaTime;
    m_fAngle        += m_fAngularSpeed * _fDeltaTime;
  }

  // diapositives -------------------------------------------------------------------------------------------------
  if(m_pEmiterCore->IsSprite())
  {
    m_iFrame = (int)floor(m_fLivedTime / m_pEmiterCore->GetTimePerFrame());
    int l_iNumFrames = m_pEmiterCore->GetNumFrames();
    m_iFrame = (m_iFrame >= l_iNumFrames)? l_iNumFrames - 1 : m_iFrame;
  }
  
  //Color ----------------------------------------------------------------------------------------------
  if(m_pEmiterCore->HasResetColorAnimation())
  {
    m_fTimeToResetColor -= _fDeltaTime;
    while(m_fTimeToResetColor <= 0)
    {
      m_fColorAnimationTime = -m_fTimeToResetColor;
      m_fTimeToResetColor += m_pEmiterCore->GetResetColorAnimationTime();
      
      m_iCurrentColor = 0;

      m_fLastColorTime = 0;

      m_LastColor = m_pEmiterCore->GetColor(0);

      if(m_pEmiterCore->GetNumColorFrames() > m_iCurrentColor + 1)
      {
        m_bHasNextColor  = true;
        m_NextColor      = m_pEmiterCore->GetColor(1);
        m_fNextColorTime = m_pEmiterCore->GetColorControlTime(1);
      }
      else
      {
        m_bHasNextColor  = false;
      }
    }
  }
  if(m_bHasNextColor)
  {
    if(m_fColorAnimationTime > m_fNextColorTime)
    {
      m_iCurrentColor++;
      m_LastColor      = m_NextColor;
      m_fLastColorTime = m_fNextColorTime;
      
      if(m_pEmiterCore->GetNumColorFrames() > m_iCurrentColor + 1)
      {
        m_bHasNextColor  = true;
        m_NextColor      = m_pEmiterCore->GetColor           (m_iCurrentColor + 1);
        m_fNextColorTime = m_pEmiterCore->GetColorControlTime(m_iCurrentColor + 1);


        float l_fInterpolationFactor = (m_fColorAnimationTime - m_fLastColorTime) / (m_fNextColorTime - m_fLastColorTime);
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
      float l_fInterpolationFactor = (m_fColorAnimationTime - m_fLastColorTime) / (m_fNextColorTime - m_fLastColorTime);
      m_Color = SIMPLE_INTERPOLATION(m_LastColor, m_NextColor, l_fInterpolationFactor);
    }
  }


  //Size ----------------------------------------------------------------------------------------------
  if(m_pEmiterCore->HasResetSizeAnimation())
  {
    m_fTimeToResetSize -= _fDeltaTime;
    while(m_fTimeToResetSize <= 0)
    {
      m_fSizeAnimationTime = -m_fTimeToResetSize;
      m_fTimeToResetSize += m_pEmiterCore->GetResetSizeAnimationTime();
      
      m_iCurrentSize = 0;

      m_fLastSizeTime = 0;

      m_fLastSize = m_pEmiterCore->GetSize(0);

      if(m_pEmiterCore->GetNumSizeFrames() > m_iCurrentSize + 1)
      {
        m_bHasNextSize  = true;
        m_fNextSize      = m_pEmiterCore->GetSize(1);
        m_fNextSizeTime = m_pEmiterCore->GetSizeControlTime(1);
      }
      else
      {
        m_bHasNextSize  = false;
      }
    }
  }
  if(m_bHasNextSize)
  {
    if(m_fSizeAnimationTime > m_fNextSizeTime)
    {
      m_iCurrentSize++;
      m_fLastSize     = m_fNextSize;
      m_fLastSizeTime = m_fNextSizeTime;
      
      if(m_pEmiterCore->GetNumSizeFrames() > m_iCurrentSize + 1)
      {
        m_bHasNextSize  = true;
        m_fNextSize     = m_pEmiterCore->GetSize           (m_iCurrentSize + 1);
        m_fNextSizeTime = m_pEmiterCore->GetSizeControlTime(m_iCurrentSize + 1);


        float l_fInterpolationFactor = (m_fSizeAnimationTime - m_fLastSizeTime) / (m_fNextSizeTime - m_fLastSizeTime);
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
      float l_fInterpolationFactor = (m_fSizeAnimationTime - m_fLastSizeTime) / (m_fNextSizeTime - m_fLastSizeTime);
      m_fSize = SIMPLE_INTERPOLATION(m_fLastSize, m_fNextSize, l_fInterpolationFactor);
    }
  }


  return true;
}


void CParticle::FillRenderInfo(SParticleRenderInfo& Info_)
{
  Info_.x = m_vPosition.x;
  Info_.y = m_vPosition.y;
  Info_.z = m_vPosition.z;
  
  Info_.sizeX =
  Info_.sizeY = m_fSize;
  
  Info_.angleSin = sin(m_fAngle + FLOAT_PI_VALUE * .25f);
  Info_.angleCos = cos(m_fAngle + FLOAT_PI_VALUE * .25f);
    
  Info_.diapo = (float) m_iFrame;
  Info_.color = DWORD (D3DXCOLOR(m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha()) );
}