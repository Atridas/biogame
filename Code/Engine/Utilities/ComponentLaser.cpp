#include "ComponentLaser.h"
#include "RenderManager.h"
#include "Core.h"


CComponentLaser* CComponentLaser::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd, float _fTime)
{
  CComponentLaser *l_pComp = new CComponentLaser();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vPosInit, _vPosEnd, _fTime))
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

bool CComponentLaser::Init(CGameEntity* _pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd, float _fTime)
{
  m_vPosInit = _vPosInit;
  m_vPosEnd  = _vPosEnd ;
  m_fTime    = _fTime   ;

  SetOk(true);
  return IsOk();
}

void CComponentLaser::Update(float _fDeltaTime)
{
  m_fTime -= _fDeltaTime;
  if(m_fTime < 0.f)
  {
    CORE->GetEntityManager()->RemoveEntity(GetEntity());
  }
}

void CComponentLaser::DebugRender(CRenderManager* _pRM)
{
  Mat44f mat;
  mat.SetIdentity();

  _pRM->SetTransform(mat);

  _pRM->DrawLine(m_vPosInit, m_vPosEnd, colRED);
}