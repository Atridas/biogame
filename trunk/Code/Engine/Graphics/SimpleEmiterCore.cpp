#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "TextureManager.h"
#include "Texture.h"
#include "StaticMeshEmptyMaterial.h"
#include "DiffuseTextureDecorator.h"
#include "SpritePropertyDecorator.h"
#include "GlowPropertyDecorator.h"
#include "GlowTextureDecorator.h"


// Main include
#include "SimpleEmiterCore.h"

bool CSimpleEmiterCore::Init(CXMLTreeNode& _xmlEmiter)
{
  bool l_bParsingBillboar = false;
  if(strcmp(_xmlEmiter.GetName(), "SimpleEmiter") != 0)
  {
    if(strcmp(_xmlEmiter.GetName(), "Billboard") == 0)
    {
      l_bParsingBillboar = true;
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "CSimpleCoreEmiter::Init XML element not correct: \"%s\" and should be \"SimpleEmiter\"", _xmlEmiter.GetName());
    }
  }

  SetName(_xmlEmiter.GetPszISOProperty("id"));
  LOGGER->AddNewLog(ELL_INFORMATION, "CSimpleCoreEmiter::Init Reading Particle Core: \"%s\"", GetName().c_str());
  
  m_fEmitRate1 = _xmlEmiter.GetFloatProperty("emit_rate1", 0, !l_bParsingBillboar);
  m_fEmitRate2 = _xmlEmiter.GetFloatProperty("emit_rate2", 0, !l_bParsingBillboar);
  LOGGER->AddNewLog(ELL_INFORMATION, "Emit rate: %f - %f", m_fEmitRate1, m_fEmitRate2);
  
  m_bEmitAbsolute = _xmlEmiter.GetBoolProperty("emit_absolute", false, false);
  if(m_bEmitAbsolute)
    LOGGER->AddNewLog(ELL_INFORMATION, "Emit absolute: true");
  else
    LOGGER->AddNewLog(ELL_INFORMATION, "Emit absolute: false");

  m_fLife1 = _xmlEmiter.GetFloatProperty("life1", 0, !l_bParsingBillboar);
  m_fLife2 = _xmlEmiter.GetFloatProperty("life2", 0, !l_bParsingBillboar);
  LOGGER->AddNewLog(ELL_INFORMATION, "Life: %f - %f", m_fLife1, m_fLife2);
  
  m_fAwakeTime1 = _xmlEmiter.GetFloatProperty("awake_time1", 1, false);
  m_fAwakeTime2 = _xmlEmiter.GetFloatProperty("awake_time2", m_fAwakeTime2, false);
  LOGGER->AddNewLog(ELL_INFORMATION, "Awake Time: %f - %f", m_fAwakeTime1, m_fAwakeTime2);
  
  m_fSleepTime1 = _xmlEmiter.GetFloatProperty("sleep_time1", 0, false);
  m_fSleepTime2 = _xmlEmiter.GetFloatProperty("sleep_time2", m_fSleepTime2, false);
  LOGGER->AddNewLog(ELL_INFORMATION, "Sleep Time: %f - %f", m_fSleepTime1, m_fSleepTime2);


  m_fRatioY = _xmlEmiter.GetFloatProperty("ratio_y", 1, false);
  LOGGER->AddNewLog(ELL_INFORMATION, "Ratio Y: %f", m_fRatioY);

  
  m_fStartingAngle1 = _xmlEmiter.GetFloatProperty("starting_angle1", 0, true) * FLOAT_PI_VALUE / 180;
  m_fStartingAngle2 = _xmlEmiter.GetFloatProperty("starting_angle2", 0, true) * FLOAT_PI_VALUE / 180;
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Angle: %f - %f", m_fStartingAngle1, m_fStartingAngle2);
  
  m_vStartingSpeed1 = _xmlEmiter.GetVect3fProperty("starting_speed1", Vect3f(0), !l_bParsingBillboar);
  m_vStartingSpeed2 = _xmlEmiter.GetVect3fProperty("starting_speed2", Vect3f(0), !l_bParsingBillboar);
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Speed: (%f,%f,%f) - (%f,%f,%f)", 
                                      m_vStartingSpeed1.x, m_vStartingSpeed1.y, m_vStartingSpeed1.z,
                                      m_vStartingSpeed2.x, m_vStartingSpeed2.y, m_vStartingSpeed2.z);
  
  m_fStartingDirectionAngle = _xmlEmiter.GetFloatProperty("starting_direction_angle", 0, false) * FLOAT_PI_VALUE / 180;
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Direction Angle: %f", m_fStartingDirectionAngle);

  m_fStartingAngularSpeed1 = _xmlEmiter.GetFloatProperty("starting_angular_speed1", 0, !l_bParsingBillboar) * FLOAT_PI_VALUE / 180;
  m_fStartingAngularSpeed2 = _xmlEmiter.GetFloatProperty("starting_angular_speed2", 0, !l_bParsingBillboar) * FLOAT_PI_VALUE / 180;
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Angular Speed: %f - %f", m_fStartingAngularSpeed1, m_fStartingAngularSpeed2);
  
  m_vAcceleration1 =  _xmlEmiter.GetVect3fProperty("acceleration1", Vect3f(0), !l_bParsingBillboar);
  m_vAcceleration2 =  _xmlEmiter.GetVect3fProperty("acceleration2", Vect3f(0), !l_bParsingBillboar);
  LOGGER->AddNewLog(ELL_INFORMATION, "Acceleration: (%f,%f,%f) - (%f,%f,%f)", 
                                      m_vAcceleration1.x, m_vAcceleration1.y, m_vAcceleration1.z,
                                      m_vAcceleration2.x, m_vAcceleration2.y, m_vAcceleration2.z);
  
  m_fAccelerationDirectionAngle = _xmlEmiter.GetFloatProperty("acceleration_direction_angle", 0, false) * FLOAT_PI_VALUE / 180;
  LOGGER->AddNewLog(ELL_INFORMATION, "Acceleration Direction Angle: %f", m_fStartingDirectionAngle);

  m_fAngularAcceleration1 =  _xmlEmiter.GetFloatProperty("angular_acceleration1", 0, !l_bParsingBillboar) * FLOAT_PI_VALUE / 180;
  m_fAngularAcceleration2 =  _xmlEmiter.GetFloatProperty("angular_acceleration2", 0, !l_bParsingBillboar) * FLOAT_PI_VALUE / 180;
  LOGGER->AddNewLog(ELL_INFORMATION, "Angular Acceleration: %f - %f", m_fAngularAcceleration1, m_fAngularAcceleration2);
  
  // Colors ----------------------------------------------------------------------------------
  CColor l_Color1(_xmlEmiter.GetVect4fProperty("color1", Vect4f(0), true));
  CColor l_Color2(_xmlEmiter.GetVect4fProperty("color2", Vect4f(0), true));
  m_ColorAnimations.clear();
  m_ColorAnimations.push_back(SColorAnimation(l_Color1, 0.f, l_Color2, 0.f));
  LOGGER->AddNewLog(ELL_INFORMATION, "Initial Color: (%f,%f,%f,%f) - (%f,%f,%f,%f)", 
                                      l_Color1.GetRed(), l_Color1.GetGreen(), l_Color1.GetBlue(), l_Color1.GetAlpha(), 
                                      l_Color2.GetRed(), l_Color2.GetGreen(), l_Color2.GetBlue(), l_Color2.GetAlpha());
  {
    CXMLTreeNode l_xmlColors = _xmlEmiter.GetChild("Colors");



    if(l_xmlColors.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "Parsing Colors");

      m_bResetColorAnimation = l_xmlColors.GetBoolProperty("reset_animation", false, false);
      m_fResetColorAnimationTime1 = l_xmlColors.GetFloatProperty("reset_animation_time1", 0, m_bResetColorAnimation);
      m_fResetColorAnimationTime2 = l_xmlColors.GetFloatProperty("reset_animation_time2", 0, m_bResetColorAnimation);

      if(m_bResetColorAnimation)
      {
        LOGGER->AddNewLog(ELL_INFORMATION, "Reset Color Animation: %f - %f", m_fResetColorAnimationTime1, m_fResetColorAnimationTime2);
      }

      int l_iNumChildren = l_xmlColors.GetNumChildren();
      for(int i = 0; i < l_iNumChildren; ++i)
      {
        CXMLTreeNode l_xmlColor = l_xmlColors(i);
        if(strcmp("Color", l_xmlColor.GetName()) == 0)
        {
          CColor l_Color1(l_xmlColor.GetVect4fProperty("color1", Vect4f(0), true));
          CColor l_Color2(l_xmlColor.GetVect4fProperty("color2", Vect4f(0), true));

          float l_fControlTime1 = l_xmlColor.GetFloatProperty("control_time1", 1.f, true);
          float l_fControlTime2 = l_xmlColor.GetFloatProperty("control_time2", 1.f, true);

          m_ColorAnimations.push_back(SColorAnimation(l_Color1, l_fControlTime1, l_Color2, l_fControlTime2));
          LOGGER->AddNewLog(ELL_INFORMATION, "Color at time (%f - %f): (%f,%f,%f,%f) - (%f,%f,%f,%f)", 
                                              l_fControlTime1, l_fControlTime2,
                                              l_Color1.GetRed(), l_Color1.GetGreen(), l_Color1.GetBlue(), l_Color1.GetAlpha(), 
                                              l_Color2.GetRed(), l_Color2.GetGreen(), l_Color2.GetBlue(), l_Color2.GetAlpha());
        }
        else if(!l_xmlColor.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING, "Unknown element: %s", l_xmlColor.GetName());
        }
      }
    }
  }

  // Sizes ----------------------------------------------------------------------------------------------
  float l_fSize1 = _xmlEmiter.GetFloatProperty("size1", 1.f, true);
  float l_fSize2 = _xmlEmiter.GetFloatProperty("size2", 1.f, true);
  m_SizeAnimations.clear();
  m_SizeAnimations.push_back(SSizeAnimation(l_fSize1, 0.f, l_fSize2, 0.f));

  {
    CXMLTreeNode l_xmlSizes = _xmlEmiter.GetChild("Sizes");

    if(l_xmlSizes.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "Parsing Sizes");

      m_bResetSizeAnimation = l_xmlSizes.GetBoolProperty("reset__animation", false, false);
      m_fResetSizeAnimationTime1 = l_xmlSizes.GetFloatProperty("reset_animation_time1", 0, m_bResetColorAnimation);
      m_fResetSizeAnimationTime2 = l_xmlSizes.GetFloatProperty("reset_animation_time2", 0, m_bResetColorAnimation);

      if(m_bResetSizeAnimation)
      {
        LOGGER->AddNewLog(ELL_INFORMATION, "Reset Size Animation: %f - %f", m_fResetSizeAnimationTime1, m_fResetSizeAnimationTime2);
      }

      int l_iNumChildren = l_xmlSizes.GetNumChildren();
      for(int i = 0; i < l_iNumChildren; ++i)
      {
        CXMLTreeNode l_xmlSize = l_xmlSizes(i);
        if(strcmp("Size", l_xmlSize.GetName()) == 0)
        {
          float l_fSize1 = l_xmlSize.GetFloatProperty("size1", 1.f, true);
          float l_fSize2 = l_xmlSize.GetFloatProperty("size2", 1.f, true);

          float l_fControlTime1 = l_xmlSize.GetFloatProperty("control_time1", 1.f, true);
          float l_fControlTime2 = l_xmlSize.GetFloatProperty("control_time2", 1.f, true);

          m_SizeAnimations.push_back(SSizeAnimation(l_fSize1, l_fControlTime1, l_fSize2, l_fControlTime2));
          LOGGER->AddNewLog(ELL_INFORMATION, "Size at time (%f - %f): %f - %f", 
                                              l_fControlTime1, l_fControlTime2,
                                              l_fSize1, l_fSize2);
        }
        else if(!l_xmlSize.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING, "Unknown element: %s", l_xmlSize.GetName());
        }
      }
    }
  }

  m_bSprite = _xmlEmiter.GetBoolProperty("is_sprite", false, false);
  if(m_bSprite)
  {
    m_iNumRows      = _xmlEmiter.GetIntProperty("num_rows",    1, true);
    m_iNumColumns   = _xmlEmiter.GetIntProperty("num_columns", 1, true);
    m_iNumFrames    = _xmlEmiter.GetIntProperty("num_frames", m_iNumRows * m_iNumColumns, false);
    m_fTimePerFrame = _xmlEmiter.GetFloatProperty("time_per_frame", 1, true);
    m_bLoop         = _xmlEmiter.GetBoolProperty("loop", false, false);

    LOGGER->AddNewLog(ELL_INFORMATION, "SPRITE    Rows: %d, Columns: %d, Frames: %d, Time per frame: %f",
                                       m_iNumRows, m_iNumColumns, m_iNumFrames, m_fTimePerFrame);

    if(m_bLoop)
      LOGGER->AddNewLog(ELL_INFORMATION, "SPRITE loop");
    else
      LOGGER->AddNewLog(ELL_INFORMATION, "SPRITE no loop");
  }
  else
  {
    m_iNumRows      = 1;
    m_iNumColumns   = 1;
    m_iNumFrames    = 1;
    m_fTimePerFrame = (m_fLife1 > m_fLife2)? m_fLife1: m_fLife2;
  }



  string   l_szTexture = _xmlEmiter.GetPszISOProperty("texture");
  CTexture* l_pTexture = CORE->GetTextureManager()->GetResource(l_szTexture);
  
  CHECKED_DELETE(m_pMaterial);
  m_pMaterial = new CStaticMeshEmptyMaterial();
  m_pMaterial = new CSpritePropertyDecorator(m_pMaterial, Vect2f( 1.f / m_iNumColumns, 1.f / m_iNumRows));
  m_pMaterial = new CDiffuseTextureDecorator(m_pMaterial,l_pTexture);

  if(_xmlEmiter.GetBoolProperty("glow", false, false))
  {
    float    l_fGlowintensity = _xmlEmiter.GetFloatProperty("glow_intensity");
    string   l_szGlowTexture  = _xmlEmiter.GetPszISOProperty("glow_texture");
    CTexture* l_pGlowTexture  = CORE->GetTextureManager()->GetResource(l_szGlowTexture);
    
    m_pMaterial = new CGlowPropertyDecorator(m_pMaterial, l_fGlowintensity);
    m_pMaterial = new CGlowTextureDecorator(m_pMaterial, l_pGlowTexture);
  }


  SetOk(true);
  return IsOk();
}

bool CSimpleEmiterCore::Init()
{
  SetName("default");
  
  m_fEmitRate1 = 0;
  m_fEmitRate2 = 0;
  
  m_fLife1 = 0;
  m_fLife2 = 0;
  
  m_fAwakeTime1 = 1;
  m_fAwakeTime2 = 1;
  
  m_fSleepTime1 = 0;
  m_fSleepTime2 = 0;


  m_fStartingAngle1 = 0;
  m_fStartingAngle2 = 0;
  
  m_vStartingSpeed1 = Vect3f(0);
  m_vStartingSpeed2 = Vect3f(0);
  
  m_fStartingAngularSpeed1 = 0;
  m_fStartingAngularSpeed2 = 0;
  
  m_vAcceleration1 = Vect3f(0);
  m_vAcceleration2 = Vect3f(0);
  
  m_fAngularAcceleration1 = 0;
  m_fAngularAcceleration2 = 0;
  
  // Colors ----------------------------------------------------------------------------------
  m_ColorAnimations.clear();
  m_ColorAnimations.push_back(SColorAnimation(CColor(Vect4f(0)), 0.f, CColor(Vect4f(0)), 0.f));

  m_SizeAnimations.clear();
  m_SizeAnimations.push_back(SSizeAnimation(1, 0.f, 1, 0.f));

  m_bSprite = false;
  m_iNumRows      = 1;
  m_iNumColumns   = 1;
  m_iNumFrames    = 1;
  m_fTimePerFrame = 0;

  CTexture* l_pTexture = CORE->GetTextureManager()->GetResource("");
  
  CHECKED_DELETE(m_pMaterial);
  m_pMaterial = new CStaticMeshEmptyMaterial();
  m_pMaterial = new CSpritePropertyDecorator(m_pMaterial, Vect2f( 1.f / m_iNumColumns, 1.f / m_iNumRows));
  m_pMaterial = new CDiffuseTextureDecorator(m_pMaterial,l_pTexture);


  SetOk(l_pTexture != 0);

  return IsOk();
}

void CSimpleEmiterCore::Release() 
{
  CHECKED_DELETE(m_pMaterial);
}

float CSimpleEmiterCore::GetEmitRate() const
{
  float l_fRnd = Random01();
  float l_fEmitRate = SIMPLE_INTERPOLATION(m_fEmitRate1, m_fEmitRate2, l_fRnd);
  return l_fEmitRate;
}

float CSimpleEmiterCore::GetLife() const
{
  float l_fRnd = Random01();
  float l_fLife = SIMPLE_INTERPOLATION(m_fLife1, m_fLife2, l_fRnd);
  return l_fLife;
}

float CSimpleEmiterCore::GetAwakeTime() const
{
  float l_fRnd = Random01();
  float l_fAwakeTime = SIMPLE_INTERPOLATION(m_fAwakeTime1, m_fAwakeTime2, l_fRnd);
  return l_fAwakeTime;
}

float CSimpleEmiterCore::GetSleepTime() const
{
  float l_fRnd = Random01();
  float l_fSleepTime = SIMPLE_INTERPOLATION(m_fSleepTime1, m_fSleepTime2, l_fRnd);
  return l_fSleepTime;
}

float CSimpleEmiterCore::GetStartingAngle() const
{
  float l_fRnd = Random01();
  float l_fStartingAngle = SIMPLE_INTERPOLATION(m_fStartingAngle1, m_fStartingAngle2, l_fRnd);
  return l_fStartingAngle;
}

Vect3f CSimpleEmiterCore::GetStartingSpeed() const
{
  //Vect3f l_vRnd(Random01(),Random01(),Random01());
  //Vect3f l_v_1_Minus_Rnd(1.f - l_vRnd.x, 1.f - l_vRnd.y, 1.f - l_vRnd.z);
  //Vect3f l_vStartingSpeed = ( l_v_1_Minus_Rnd.Scale(m_vStartingSpeed1) ) + ( l_vRnd.Scale(m_vStartingSpeed2) );
  //return l_vStartingSpeed;

  
  float l_fRnd = Random01();
  Vect3f l_vStartingSpeed = SIMPLE_INTERPOLATION(m_vStartingSpeed1, m_vStartingSpeed2, l_fRnd);

  Mat33f l_mRotation = RandomRotationMatrix(m_fStartingDirectionAngle);

  Vect3f vStartingSpeed_ = l_mRotation * l_vStartingSpeed;

  return vStartingSpeed_;
}

float CSimpleEmiterCore::GetStartingAngularSpeed() const
{
  float l_fRnd = Random01();
  float l_fStartingAngularSpeed = SIMPLE_INTERPOLATION(m_fStartingAngularSpeed1, m_fStartingAngularSpeed2, l_fRnd);
  return l_fStartingAngularSpeed;
}


Vect3f CSimpleEmiterCore::GetAcceleration() const
{
  //Vect3f l_vRnd(Random01(),Random01(),Random01());
  //Vect3f l_v_1_Minus_Rnd(1.f - l_vRnd.x, 1.f - l_vRnd.y, 1.f - l_vRnd.z);
  //Vect3f l_vAcceleration = ( l_v_1_Minus_Rnd.Scale(m_vAcceleration1) ) + ( l_vRnd.Scale(m_vAcceleration2) );
  //return l_vAcceleration;

  
  float l_fRnd = Random01();
  Vect3f l_vAcceleration = SIMPLE_INTERPOLATION(m_vAcceleration1, m_vAcceleration2, l_fRnd);

  Mat33f l_mRotation = RandomRotationMatrix(m_fAccelerationDirectionAngle);

  Vect3f vAcceleration_ = l_mRotation * l_vAcceleration;
  return vAcceleration_;
}

float CSimpleEmiterCore::GetAngularAcceleration() const
{
  float l_fRnd = Random01();
  float l_fAngularAcceleration = SIMPLE_INTERPOLATION(m_fAngularAcceleration1, m_fAngularAcceleration2, l_fRnd);
  return l_fAngularAcceleration;
}


CColor CSimpleEmiterCore::GetColor(int _iColorFrame) const
{
  float l_fRnd = Random01();
  CColor l_Color = SIMPLE_INTERPOLATION(m_ColorAnimations[_iColorFrame].m_Color1, m_ColorAnimations[_iColorFrame].m_Color2, l_fRnd);
  return l_Color;
}

float  CSimpleEmiterCore::GetSize(int _iSizeFrame) const
{
  float l_fRnd = Random01();
  float l_fSize = SIMPLE_INTERPOLATION(m_SizeAnimations[_iSizeFrame].m_fSize1, m_SizeAnimations[_iSizeFrame].m_fSize2, l_fRnd);
  return l_fSize;
}


float CSimpleEmiterCore::GetColorControlTime(int _iColorFrame) const
{
  float l_fRnd = Random01();
  float l_fControlTime = SIMPLE_INTERPOLATION(m_ColorAnimations[_iColorFrame].m_fControlTime1, m_ColorAnimations[_iColorFrame].m_fControlTime2, l_fRnd);
  return l_fControlTime;
}

float CSimpleEmiterCore::GetSizeControlTime(int _iSizeFrame) const
{
  float l_fRnd = Random01();
  float l_fControlTime = SIMPLE_INTERPOLATION(m_SizeAnimations[_iSizeFrame].m_fControlTime1, m_SizeAnimations[_iSizeFrame].m_fControlTime2, l_fRnd);
  return l_fControlTime;
}

float CSimpleEmiterCore::GetResetColorAnimationTime() const
{
  float l_fRnd = Random01();
  float l_fResetColorAnimationTime = SIMPLE_INTERPOLATION(m_fResetColorAnimationTime1, m_fResetColorAnimationTime2, l_fRnd);
  return l_fResetColorAnimationTime;
}

float CSimpleEmiterCore::GetResetSizeAnimationTime() const
{
  float l_fRnd = Random01();
  float l_fResetSizeAnimationTime = SIMPLE_INTERPOLATION(m_fResetSizeAnimationTime1, m_fResetSizeAnimationTime2, l_fRnd);
  return l_fResetSizeAnimationTime;
}
