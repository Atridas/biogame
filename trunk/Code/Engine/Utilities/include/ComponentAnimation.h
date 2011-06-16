#pragma once
#ifndef __COMPONENT_ANIMATION__
#define __COMPONENT_ANIMATION__

#include "base.h"
#include "EntityDefines.h"

// ----------------------------------------
class CRenderableAnimatedInstanceModel;
// ----------------------------------------


class CComponentAnimation:
  public CBaseComponent
{
public:
  CComponentAnimation(): m_pAnimatedModel(0), m_szCurrentCycle("") {};
  ~CComponentAnimation() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_ANIMATION;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_ANIMATION;};

  bool Init(CGameEntity* _pEntity);
  
  void ClearCycle  (float _fBlend);
  void ClearCycle  (const string& _szAnimation, float _fBlend);
  void SetCycle    (const string& _szAnimation, float _fBlend);
  void SetAnimation(const string& _szAnimation, float _fBlend);
  const string&    GetCurrentCycle() {return m_szCurrentCycle;}

  //void Play(const string& _szAnimation, bool _bLockEnd = false);
  //void Stop(const string& _szAnimation);
  //void PlayCycle(const string& _szAnimation, float _fBlendTime);
  //void StopCycle(const string& _szAnimation, float _fBlendTime);
  //void ClearCyle(const string& _szAnimation);
  //void ClearAllCycles();


protected:
  virtual void Release() {};
private:
  CRenderableAnimatedInstanceModel* m_pAnimatedModel;
  string                            m_szCurrentCycle;
};

#endif