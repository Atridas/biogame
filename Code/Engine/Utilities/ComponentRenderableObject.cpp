#include "ComponentRenderableObject.h"
#include "ComponentObject3D.h"
#include "RenderableObjectsManager.h"
#include "Core.h"

bool CComponentRenderableObject::Init(CGameEntity *_pEntity, const string& _szName)
{
  assert(_pEntity->IsOk());

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();

  m_pRenderableObject = l_pROM->GetResource(_szName);

  if(!m_pRenderableObject)
  {
    SetOk(false);
  } else {
    SetOk(true);

    Mat44f l_Matrix;
    m_pRenderableObject->GetMat44(l_Matrix);
    m_pObject3D->SetMat44(l_Matrix);
  }

  if(IsOk())
  {
    SetEntity(_pEntity);
  }

  return IsOk();
}

void CComponentRenderableObject::PostUpdate(float _fDeltaTime)
{
  assert(IsOk());
  Mat44f l_Matrix;
  
  m_pObject3D->GetMat44(l_Matrix);
  m_pRenderableObject->SetMat44(l_Matrix);
}
