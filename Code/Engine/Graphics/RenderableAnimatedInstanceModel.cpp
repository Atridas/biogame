#include "RenderableAnimatedInstanceModel.h"
#include "Core.h"
#include "RenderManager.h"
#include "base.h"
#include "AnimatedModelManager.h"


CRenderableAnimatedInstanceModel::CRenderableAnimatedInstanceModel(const string& _szName): CRenderableObject(_szName)
{
  
}

bool CRenderableAnimatedInstanceModel::Init(const string& _szCoreName, const string& _szDefaultAnimation)
{
  if(_szCoreName != "") 
  {
    m_pAnimatedInstanceModel = ANIMATED_MANAGER->GetInstance(_szCoreName);
    if(m_pAnimatedInstanceModel != 0)
    {
      if(_szDefaultAnimation != "")
      {
        m_pAnimatedInstanceModel->BlendCycle(_szDefaultAnimation,0);
      }
      
        SetOk(true);
     
    }
  } else {
    SetOk(true);
  }

  return IsOk();
}


void CRenderableAnimatedInstanceModel::RenderRenderableObject(CRenderManager* _pRM)
{
  if(m_pAnimatedInstanceModel)
    m_pAnimatedInstanceModel->Render(_pRM);
}



void CRenderableAnimatedInstanceModel::Release()
{
  CHECKED_DELETE(m_pAnimatedInstanceModel);
}

void CRenderableAnimatedInstanceModel::Update(float _fElapsedTime)
{
  if(m_pAnimatedInstanceModel)
  {
    m_pAnimatedInstanceModel->Update(_fElapsedTime);
    CRenderableObject::Update(_fElapsedTime);
  }
}

