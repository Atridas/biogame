#pragma once
#ifndef __CORE_SIMPLE_EMITER_H__
#define __CORE_SIMPLE_EMITER_H__

#include "base.h"
#include "Named.h"
#include "ParticleConstants.h"
#include "EmiterCore.h"

// ----------------------------
class CXMLTreeNode;
class CMaterial;
// ----------------------------

class CSimpleEmiterCore:
   public CEmiterCore
{
public:
  CSimpleEmiterCore():
      m_fEmitRate1(0.f), m_fEmitRate2(0.f),
      m_bEmitAbsolute(false),
      m_fLife1(0.f), m_fLife2(0.f),
      m_fAwakeTime1(1.f), m_fAwakeTime2(1.f),
      m_fSleepTime1(0.f), m_fSleepTime2(0.f),
      m_fRatioY(1.f),
      m_fStartingAngle1(0.f), m_fStartingAngle2(0.f),
      m_vStartingSpeed1(0.f), m_vStartingSpeed2(0.f),
      m_fStartingDirectionAngle(0.f),
      m_fStartingAngularSpeed1(0.f), m_fStartingAngularSpeed2(0.f),
      m_vAcceleration1(0.f), m_vAcceleration2(0.f),
      m_fAccelerationDirectionAngle(0.f),
      m_fAngularAcceleration1(0.f), m_fAngularAcceleration2(0.f),
      //vector<SColorAnimation> m_ColorAnimations;
      //vector<SSizeAnimation>  m_SizeAnimations;
      m_bSprite(false),
      m_iNumRows(1), m_iNumColumns(1),
      m_iNumFrames(1),
      m_fTimePerFrame(1.f),
      m_bLoop(false),
      m_bResetColorAnimation(false),
      m_fResetColorAnimationTime1(0), m_fResetColorAnimationTime2(0),
      m_bResetSizeAnimation(false),
      m_fResetSizeAnimationTime1(0), m_fResetSizeAnimationTime2(0),
      m_pMaterial(0) {};
  virtual ~CSimpleEmiterCore() {Done();};

  bool Init(CXMLTreeNode& _xmlEmiter);
  bool Init();
  
  float GetEmitRate() const;
  bool  GetEmitAbsolute() const {return m_bEmitAbsolute;}
  float GetLife() const;
  float GetAwakeTime() const;
  float GetSleepTime() const;
  
  float  GetStartingAngle() const;

  Vect3f GetStartingSpeed() const;
  float  GetStartingAngularSpeed() const;

  Vect3f GetAcceleration() const;
  float  GetAngularAcceleration() const;

  float GetRatioY() const {return m_fRatioY;};
  
  int GetNumColorFrames() const { return m_ColorAnimations.size(); }
  int GetNumSizeFrames() const { return m_SizeAnimations.size(); }

  CColor GetColor(int _iColorFrame) const;
  float  GetSize(int _iSizeFrame) const;

  float GetColorControlTime(int _iColorFrame) const;
  float GetSizeControlTime(int _iSizeFrame) const;
  
  bool  HasResetColorAnimation() const { return m_bResetColorAnimation; };
  float GetResetColorAnimationTime() const;
  bool  HasResetSizeAnimation() const { return m_bResetSizeAnimation; };
  float GetResetSizeAnimationTime() const;
  
  bool  IsSprite()        const { return m_bSprite; };
  int   GetNumFrames()    const { return m_iNumFrames; };
  float GetTimePerFrame() const { return m_fTimePerFrame; };
  bool  IsLoop()          const { return m_bLoop; };

  CMaterial* GetMaterial() const { return m_pMaterial; };
  
  virtual bool IsSimpleEmiter() const { return true; };
  virtual bool IsAggregateEmiter() const { return false; };

protected:
  virtual void Release();

private:
  
  float m_fEmitRate1, m_fEmitRate2;
  bool m_bEmitAbsolute;
  float m_fLife1    , m_fLife2;
  
  float m_fAwakeTime1, m_fAwakeTime2;
  float m_fSleepTime1, m_fSleepTime2;

  float m_fRatioY;
  
  float m_fStartingAngle1, m_fStartingAngle2;

  Vect3f m_vStartingSpeed1       , m_vStartingSpeed2;
  float  m_fStartingDirectionAngle;
  float  m_fStartingAngularSpeed1, m_fStartingAngularSpeed2;
  

  Vect3f m_vAcceleration1       , m_vAcceleration2;
  float  m_fAccelerationDirectionAngle;
  float  m_fAngularAcceleration1, m_fAngularAcceleration2;

  bool m_bResetColorAnimation;
  float m_fResetColorAnimationTime1, m_fResetColorAnimationTime2;
  vector<SColorAnimation> m_ColorAnimations;
  bool m_bResetSizeAnimation;
  float m_fResetSizeAnimationTime1, m_fResetSizeAnimationTime2;
  vector<SSizeAnimation>  m_SizeAnimations;


  bool  m_bSprite;
  int   m_iNumRows, m_iNumColumns;
  int   m_iNumFrames;
  float m_fTimePerFrame;
  bool  m_bLoop;

  CMaterial* m_pMaterial;
};


#endif
