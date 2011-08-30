#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "TextureManager.h"
#include "Texture.h"
#include "StaticMeshEmptyMaterial.h"
#include "DiffuseTextureDecorator.h"
#include "SpritePropertyDecorator.h"


// Main include
#include "CoreEmiter.h"

bool CCoreEmiter::Init(CXMLTreeNode& _xmlEmiter)
{
  SetName(_xmlEmiter.GetPszISOProperty("id"));
  
  m_fEmitRate1 = _xmlEmiter.GetFloatProperty("emit_rate1", 0, true);
  m_fEmitRate2 = _xmlEmiter.GetFloatProperty("emit_rate2", 0, true);
  LOGGER->AddNewLog(ELL_INFORMATION, "Emit rate: %f - %f", m_fEmitRate1, m_fEmitRate2);
  
  m_fLife1 = _xmlEmiter.GetFloatProperty("life1", 0, true);
  m_fLife2 = _xmlEmiter.GetFloatProperty("life2", 0, true);
  LOGGER->AddNewLog(ELL_INFORMATION, "Life: %f - %f", m_fLife1, m_fLife2);
  
  m_fAwakeTime1 = _xmlEmiter.GetFloatProperty("awake_time1", 1, false);
  m_fAwakeTime2 = _xmlEmiter.GetFloatProperty("awake_time2", m_fAwakeTime2, false);
  LOGGER->AddNewLog(ELL_INFORMATION, "Awake Time: %f - %f", m_fAwakeTime1, m_fAwakeTime2);
  
  m_fSleepTime1 = _xmlEmiter.GetFloatProperty("sleep_time1", 0, false);
  m_fSleepTime2 = _xmlEmiter.GetFloatProperty("sleep_time2", m_fSleepTime2, false);
  LOGGER->AddNewLog(ELL_INFORMATION, "Sleep Time: %f - %f", m_fSleepTime1, m_fSleepTime2);


  
  m_fStartingAngle1 = _xmlEmiter.GetFloatProperty("starting_angle1", 0, true);
  m_fStartingAngle2 = _xmlEmiter.GetFloatProperty("starting_angle2", 0, true);
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Angle: %f - %f", m_fStartingAngle1, m_fStartingAngle2);
  
  m_vStartingSpeed1 = _xmlEmiter.GetVect3fProperty("starting_speed1", Vect3f(0), true);
  m_vStartingSpeed2 = _xmlEmiter.GetVect3fProperty("starting_speed2", Vect3f(0), true);
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Speed: (%f,%f,%f) - (%f,%f,%f)", 
                                      m_vStartingSpeed1.x, m_vStartingSpeed1.y, m_vStartingSpeed1.z,
                                      m_vStartingSpeed2.x, m_vStartingSpeed2.y, m_vStartingSpeed2.z);
  
  m_fStartingAngularSpeed1 = _xmlEmiter.GetFloatProperty("starting_angular_speed1", 0, true);
  m_fStartingAngularSpeed2 = _xmlEmiter.GetFloatProperty("starting_angular_speed2", 0, true);
  LOGGER->AddNewLog(ELL_INFORMATION, "Starting Angular Speed: %f - %f", m_fStartingAngularSpeed1, m_fStartingAngularSpeed2);
  
  m_vAcceleration1 =  _xmlEmiter.GetVect3fProperty("acceleration1", Vect3f(0), true);
  m_vAcceleration2 =  _xmlEmiter.GetVect3fProperty("acceleration2", Vect3f(0), true);
  LOGGER->AddNewLog(ELL_INFORMATION, "Acceleration: (%f,%f,%f) - (%f,%f,%f)", 
                                      m_vAcceleration1.x, m_vAcceleration1.y, m_vAcceleration1.z,
                                      m_vAcceleration2.x, m_vAcceleration2.y, m_vAcceleration2.z);
  
  m_fAngularAcceleration1 =  _xmlEmiter.GetFloatProperty("angular_acceleration1", 0, true);
  m_fAngularAcceleration2 =  _xmlEmiter.GetFloatProperty("angular_acceleration2", 0, true);
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
    //CXMLTreeNode l_xmlColors = _xmlEmiter["Colors"];
    CXMLTreeNode l_xmlColors;
    int l_iNumChildren = _xmlEmiter.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; ++i)
    {
      if(strcmp("Colors", _xmlEmiter(i).GetName()) == 0)
      {
        l_xmlColors = _xmlEmiter(i);
        break;
      }
    }

    if(l_xmlColors.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "Parsing Colors");
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
    //CXMLTreeNode l_xmlSizes = _xmlEmiter["Sizes"];
    CXMLTreeNode l_xmlSizes;
    int l_iNumChildren = _xmlEmiter.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; ++i)
    {
      if(strcmp("Sizes", _xmlEmiter(i).GetName()) == 0)
      {
        l_xmlSizes = _xmlEmiter(i);
        break;
      }
    }

    if(l_xmlSizes.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "Parsing Sizes");
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

    LOGGER->AddNewLog(ELL_INFORMATION, "SPRITE    Rows: %d, Columns: %d, Frames: %d, Time per frame: %f",
                                       m_iNumRows, m_iNumColumns, m_iNumFrames, m_fTimePerFrame);
  }
  else
  {
    m_iNumRows      = 1;
    m_iNumColumns   = 1;
    m_iNumFrames    = 1;
    m_fTimePerFrame = (m_fLife1 > m_fLife2)? m_fLife1: m_fLife2;
  }



  string   l_szTexture = _xmlEmiter.GetPszISOProperty("texture", "", true);
  CTexture* l_pTexture = CORE->GetTextureManager()->GetResource(l_szTexture);
  
  CHECKED_DELETE(m_pMaterial);
  m_pMaterial = new CStaticMeshEmptyMaterial();
  m_pMaterial = new CSpritePropertyDecorator(m_pMaterial, Vect2f( 1.f / m_iNumColumns, 1.f / m_iNumRows));
  m_pMaterial = new CDiffuseTextureDecorator(m_pMaterial,l_pTexture);


  SetOk(true);
  return IsOk();
}

void CCoreEmiter::Init()
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


  SetOk(true);
}

void CCoreEmiter::Release() 
{
  CHECKED_DELETE(m_pMaterial);
}

float CCoreEmiter::GetEmitRate() const
{
  float l_fRnd = Random01();
  float l_fEmitRate = SIMPLE_INTERPOLATION(m_fEmitRate1, m_fEmitRate2, l_fRnd);
  return l_fEmitRate;
}

float CCoreEmiter::GetLife() const
{
  float l_fRnd = Random01();
  float l_fLife = SIMPLE_INTERPOLATION(m_fLife1, m_fLife2, l_fRnd);
  return l_fLife;
}

float CCoreEmiter::GetAwakeTime() const
{
  float l_fRnd = Random01();
  float l_fAwakeTime = SIMPLE_INTERPOLATION(m_fAwakeTime1, m_fAwakeTime2, l_fRnd);
  return l_fAwakeTime;
}

float CCoreEmiter::GetSleepTime() const
{
  float l_fRnd = Random01();
  float l_fSleepTime = SIMPLE_INTERPOLATION(m_fSleepTime1, m_fSleepTime2, l_fRnd);
  return l_fSleepTime;
}

float CCoreEmiter::GetStartingAngle() const
{
  float l_fRnd = Random01();
  float l_fStartingAngle = SIMPLE_INTERPOLATION(m_fStartingAngle1, m_fStartingAngle2, l_fRnd);
  return l_fStartingAngle;
}

Vect3f CCoreEmiter::GetStartingSpeed() const
{
  float l_fRnd = Random01();
  Vect3f l_vStartingSpeed = SIMPLE_INTERPOLATION(m_vStartingSpeed1, m_vStartingSpeed2, l_fRnd);
  return l_vStartingSpeed;
}

float CCoreEmiter::GetStartingAngularSpeed() const
{
  float l_fRnd = Random01();
  float l_fStartingAngularSpeed = SIMPLE_INTERPOLATION(m_fStartingAngularSpeed1, m_fStartingAngularSpeed2, l_fRnd);
  return l_fStartingAngularSpeed;
}


Vect3f CCoreEmiter::GetAcceleration() const
{
  float l_fRnd = Random01();
  Vect3f l_vAcceleration = SIMPLE_INTERPOLATION(m_vAcceleration1, m_vAcceleration2, l_fRnd);
  return l_vAcceleration;
}

float CCoreEmiter::GetAngularAcceleration() const
{
  float l_fRnd = Random01();
  float l_fAngularAcceleration = SIMPLE_INTERPOLATION(m_fAngularAcceleration1, m_fAngularAcceleration2, l_fRnd);
  return l_fAngularAcceleration;
}


CColor CCoreEmiter::GetColor(int _iColorFrame) const
{
  float l_fRnd = Random01();
  CColor l_Color = SIMPLE_INTERPOLATION(m_ColorAnimations[_iColorFrame].m_Color1, m_ColorAnimations[_iColorFrame].m_Color2, l_fRnd);
  return l_Color;
}

float  CCoreEmiter::GetSize(int _iSizeFrame) const
{
  float l_fRnd = Random01();
  float l_fSize = SIMPLE_INTERPOLATION(m_SizeAnimations[_iSizeFrame].m_fSize1, m_SizeAnimations[_iSizeFrame].m_fSize2, l_fRnd);
  return l_fSize;
}


float CCoreEmiter::GetColorControlTime(int _iColorFrame) const
{
  float l_fRnd = Random01();
  float l_fControlTime = SIMPLE_INTERPOLATION(m_ColorAnimations[_iColorFrame].m_fControlTime1, m_ColorAnimations[_iColorFrame].m_fControlTime2, l_fRnd);
  return l_fControlTime;
}

float CCoreEmiter::GetSizeControlTime(int _iSizeFrame) const
{
  float l_fRnd = Random01();
  float l_fControlTime = SIMPLE_INTERPOLATION(m_SizeAnimations[_iSizeFrame].m_fControlTime1, m_SizeAnimations[_iSizeFrame].m_fControlTime2, l_fRnd);
  return l_fControlTime;
}
