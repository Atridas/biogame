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

class CEmiterCore:
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
  CEmiterCore():
      CNamed("") {};

  virtual bool Init(CXMLTreeNode& _xmlEmiter) = 0;
      
  virtual bool IsSimpleEmiter() = 0;
  virtual bool IsAggregateEmiter() = 0;
};


#endif
