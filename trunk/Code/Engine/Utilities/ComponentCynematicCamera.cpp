#include "ComponentCynematicCamera.h"
#include "ComponentObject3D.h"
#include "Core.h"

#include "Renderer.h"
#include "RenderManager.h"


CComponentCynematicCamera* CComponentCynematicCamera::AddToEntity(CGameEntity* _pEntity, const Vect3f& _vTarget, float _fFov, float _fNear, float _fFar)
{
  CComponentCynematicCamera *l_pComp = new CComponentCynematicCamera();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vTarget, _fFov, _fNear, _fFar))
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

bool CComponentCynematicCamera::Init(CGameEntity* _pEntity, const Vect3f& _vTarget, float _fFov, float _fNear, float _fFar)
{
  CComponentObject3D *m_pO3D = _pEntity->GetComponent<CComponentObject3D>();

  if(!m_pO3D)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CComponentCynematicCamera::Init needs a CComponentObject3D");
    return false;
  }

  Mat44f m;
  m.SetIdentity();
  m.Translate(_vTarget);

  m_Target.SetMat44(m);


  m_pCamera = new CSphereCamera(_fNear, 
                                _fFar, 
                                _fFov, 
                                ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                                m_pO3D,
                                &m_Target);

  SetOk(true);
  return IsOk();
}

void CComponentCynematicCamera::Release()
{
  CHECKED_DELETE(m_pCamera);
}

void CComponentCynematicCamera::ActivateCamera() const
{
  CORE->GetRenderer()->SetActiveCamera(m_pCamera);
}

void CComponentCynematicCamera::DeactivateCamera() const
{
  CORE->GetRenderer()->SetActiveCamera(0);
}