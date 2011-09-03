#include "ComponentAnimation.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"


CComponentAnimation* CComponentAnimation::AddToEntity(CGameEntity *_pEntity)
{
  CComponentAnimation *l_pComp = new CComponentAnimation();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentAnimation::Init(CGameEntity* _pEntity)
{
  CComponentRenderableObject *l_pComponentRO = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  assert(l_pComponentRO); //TODO fer missatges d'error més elavorats
  m_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
  assert(m_pAnimatedModel); //TODO fer missatges d'error més elavorats

  m_stActiveCycles.clear();
  m_stInactiveCycles.clear();

  SetOk(true);
  return IsOk();
}

void CComponentAnimation::PreUpdate(float _fDeltaTime)
{
  assert(m_pAnimatedModel);
  m_pAnimatedModel->Update(_fDeltaTime);
}

void CComponentAnimation::Play(const string& _szAnimation, float _fBlend, float _fWeight, bool _bLockEnd)
{
  int id = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimationId(_szAnimation);

  m_pAnimatedModel->GetAnimatedInstanceModel()->ExecuteAction(id, _fBlend, _fWeight, _bLockEnd);
}

void CComponentAnimation::Stop(const string& _szAnimation)
{
  int id = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimationId(_szAnimation);

  m_pAnimatedModel->GetAnimatedInstanceModel()->RemoveAction(id);
}

void CComponentAnimation::PlayCycle(const string& _szAnimation, float _fBlendTime)
{
  pair<set<string>::iterator,bool> l_pairResult;

  l_pairResult = m_stActiveCycles.insert(_szAnimation);

  if(l_pairResult.second)
  {
    m_stInactiveCycles.erase(_szAnimation);
    m_pAnimatedModel->GetAnimatedInstanceModel()->BlendCycle(_szAnimation,_fBlendTime);
  }
}

void CComponentAnimation::StopCycle(const string& _szAnimation, float _fBlendTime)
{
  pair<set<string>::iterator,bool> l_pairResult;

  l_pairResult = m_stInactiveCycles.insert(_szAnimation);

  if(l_pairResult.second)
  {
    m_stActiveCycles.erase(_szAnimation);
    m_pAnimatedModel->GetAnimatedInstanceModel()->BlendCycle(_szAnimation, _fBlendTime, 0.0f);
  }
}

void CComponentAnimation::ClearCycle  (float _fBlend)
{
  //if(m_stCycles.erase())
  //{
  //  string l_szCurrentCycle = m_lCycles.back();
  //  m_lCycles.pop_back();

  //  m_pAnimatedModel->GetAnimatedInstanceModel()->ClearCycle(_fBlend);
  //  //m_szCurrentCycle = "";
  //}
}


void CComponentAnimation::ClearCycle  (const string& _szAnimation, float _fBlend)
{
  m_stActiveCycles.erase(_szAnimation);
  m_stInactiveCycles.erase(_szAnimation);

  m_pAnimatedModel->GetAnimatedInstanceModel()->ClearCycle(_szAnimation, _fBlend);
  
}

void CComponentAnimation::ClearAllCycles(float _fBlend)
{
  set<string>::iterator l_stIt;

  for(l_stIt = m_stActiveCycles.begin(); l_stIt != m_stActiveCycles.end(); ++l_stIt)
  {
    m_pAnimatedModel->GetAnimatedInstanceModel()->ClearCycle(*l_stIt, _fBlend);
  }

  m_stActiveCycles.clear();

  for(l_stIt = m_stInactiveCycles.begin(); l_stIt != m_stInactiveCycles.end(); ++l_stIt)
  {
    m_pAnimatedModel->GetAnimatedInstanceModel()->ClearCycle(*l_stIt, _fBlend);
  }

  m_stInactiveCycles.clear();
}