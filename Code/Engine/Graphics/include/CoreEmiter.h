#pragma once
#ifndef __CORE_EMITER_H__
#define __CORE_EMITER_H__
#include "base.h"
#include "Named.h"
#include "ParticleConstants.h"


// ----------------------------
class CXMLTreeNode;
class CMaterial;
// ----------------------------

class CCoreEmiter:
   public CBaseControl,
   public CNamed
{
public:
  
  struct SColorAnimation {
    float m_fControlTime1, m_fControlTime2;
    CColor m_Color1, m_Color2;

    SColorAnimation(const CColor& _Color1, float _fControlTime1, const CColor& _Color2, float _fControlTime2):
      m_fControlTime1(_fControlTime1),m_fControlTime2(_fControlTime2),m_Color1(_Color1),m_Color2(_Color2){};
  };
  struct SSizeAnimation {
    float m_fControlTime1, m_fControlTime2;
    float m_fSize1, m_fSize2;

    SSizeAnimation(float _fSize1, float _fControlTime1, float _fSize2, float _fControlTime2):
      m_fControlTime1(_fControlTime1),m_fControlTime2(_fControlTime2),m_fSize1(_fSize1),m_fSize2(_fSize2){};
  };

public:
  CCoreEmiter():
      CNamed(""),
      m_fEmitRate1(0.f), m_fEmitRate2(0.f),
      m_fLife1(0.f), m_fLife2(0.f),
      m_bBucle(false),
      m_fStartingAngle1(0.f), m_fStartingAngle2(0.f),
      m_vStartingSpeed1(0.f), m_vStartingSpeed2(0.f),
      m_fStartingAngularSpeed1(0.f), m_fStartingAngularSpeed2(0.f),
      m_vAcceleration1(0.f), m_vAcceleration2(0.f),
      m_fAngularAcceleration1(0.f), m_fAngularAcceleration2(0.f),
      //vector<SColorAnimation> m_ColorAnimations;
      //vector<SSizeAnimation>  m_SizeAnimations;
      m_bSprite(false),
      m_iNumRows(1), m_iNumColumns(1),
      m_iNumFrames(1),
      m_fTimePerFrame(1.f),
      m_pMaterial(0) {};
  virtual ~CCoreEmiter() {Done();};

  bool Init(CXMLTreeNode& _xmlEmiter);
  void Init();
  
  float GetEmitRate() const;
  float GetLife() const;
  bool  IsBucle() const { return m_bBucle; };
  
  float  GetStartingAngle() const;

  Vect3f GetStartingSpeed() const;
  float  GetStartingAngularSpeed() const;

  Vect3f GetAcceleration() const;
  float  GetAngularAcceleration() const;
  
  int GetNumColorFrames() const { return m_ColorAnimations.size(); }
  int GetNumSizeFrames() const { return m_ColorAnimations.size(); }

  CColor GetColor(int _iColorFrame) const;
  float  GetSize(int _iSizeFrame) const;

  float GetColorControlTime(int _iColorFrame) const;
  float GetSizeControlTime(int _iSizeFrame) const;
  
  bool  IsSprite()        const { return m_bSprite; };
  int   GetNumFrames()    const { return m_iNumFrames; };
  float GetTimePerFrame() const { return m_fTimePerFrame; };


  CMaterial* GetMaterial() const { return m_pMaterial; };

protected:
  virtual void Release();

private:
  
  float m_fEmitRate1, m_fEmitRate2;
  float m_fLife1    , m_fLife2;
  bool  m_bBucle;
  
  float m_fStartingAngle1, m_fStartingAngle2;

  Vect3f m_vStartingSpeed1       , m_vStartingSpeed2;
  float  m_fStartingAngularSpeed1, m_fStartingAngularSpeed2;
  
  Vect3f m_vAcceleration1       , m_vAcceleration2;
  float  m_fAngularAcceleration1, m_fAngularAcceleration2;

  vector<SColorAnimation> m_ColorAnimations;
  vector<SSizeAnimation>  m_SizeAnimations;


  bool  m_bSprite;
  int   m_iNumRows, m_iNumColumns;
  int   m_iNumFrames;
  float m_fTimePerFrame;

  CMaterial* m_pMaterial;
};


#endif
