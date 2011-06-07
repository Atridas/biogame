#include "ComponentLaser.h"
#include "RenderManager.h"
#include "Core.h"


bool CComponentLaser::Init(CGameEntity* _pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd, float _fTime)
{
  assert(_pEntity);
  SetEntity(_pEntity);

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