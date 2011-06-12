#include "ComponentAnimation.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"

bool CComponentAnimation::Init(CGameEntity* _pEntity)
{
  assert(_pEntity && _pEntity->IsOk());
  SetEntity(_pEntity);

  CComponentRenderableObject *l_pComponentRO = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  assert(l_pComponentRO); //TODO fer missatges d'error més elavorats
  m_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
  assert(m_pAnimatedModel); //TODO fer missatges d'error més elavorats

  SetOk(true);
  return IsOk();
}



void CComponentAnimation::ClearCycle  (float _fBlend)
{
  m_pAnimatedModel->GetAnimatedInstanceModel()->ClearCycle(_fBlend);
  m_szCurrentCycle = "";
}


void CComponentAnimation::ClearCycle  (const string& _szAnimation, float _fBlend)
{
  m_pAnimatedModel->GetAnimatedInstanceModel()->ClearCycle(_szAnimation, _fBlend);
  m_szCurrentCycle = "";
}
  
void CComponentAnimation::SetCycle    (const string& _szAnimation, float _fBlend)
{
  if(_szAnimation != m_szCurrentCycle)
  {
    m_pAnimatedModel->GetAnimatedInstanceModel()->BlendCycle(_szAnimation,_fBlend);

    //ClearCycle(_fBlend);
    m_szCurrentCycle = _szAnimation;
  }

}

void CComponentAnimation::SetAnimation(const string& _szAnimation, float _fBlend)
{
  int id = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimationId(_szAnimation);

  m_pAnimatedModel->GetAnimatedInstanceModel()->ExecuteAction(id, _fBlend);
}
