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
  ~CComponentAnimation() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_ANIMATION;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_ANIMATION;};

  static CComponentAnimation* AddToEntity(CGameEntity* _pEntity);
  
  void Play(const string& _szAnimation, float _fBlend, float _fWeight = 1.0f, bool _bLockEnd = false);
  void Stop(const string& _szAnimation);
  void PlayCycle(const string& _szAnimation, float _fBlendTime);
  void PlayCycle(const string& _szAnimation, float _fBlendTime, float _fWeight);
  void StopCycle(const string& _szAnimation, float _fBlendTime);
  //void ClearCycle(float _fBlend);
  void ClearCycle(const string& _szAnimation, float _fTime);
  void ClearAllCycles(float _fTime);

  
  void SetAnimationState(const string& _szAnimationState);
  void SetAnimationParameter(float _fAnimationParameter);

  virtual void PreUpdate(float _fDeltaTime);

protected:
  virtual void Release() { m_stActiveCycles.clear(); m_stInactiveCycles.clear(); };
private:
  
  CComponentAnimation(): m_pAnimatedModel(0), m_szCurrentCycle("") {};
  bool Init(CGameEntity* _pEntity);

  CRenderableAnimatedInstanceModel* m_pAnimatedModel;
  string                            m_szCurrentCycle;
  set<string>                       m_stActiveCycles;
  set<string>                       m_stInactiveCycles;
};

#endif