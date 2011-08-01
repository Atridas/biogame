#include "ComponentRenderableObject.h"
#include "ComponentObject3D.h"
#include "RenderableObjectsManager.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentAnimation.h"
#include "ComponentPhysXActor.h"
#include "Core.h"


CComponentRenderableObject* CComponentRenderableObject::AddToEntity(CGameEntity *_pEntity, const string& _szName, const string& _szCore)
{
  CComponentRenderableObject *l_pComp = new CComponentRenderableObject();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szName, _szCore))
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


CComponentRenderableObject* CComponentRenderableObject::AddToEntityWithAnimatedModel(CGameEntity *_pEntity, const string& _szName, const string& _szCore)
{
  CComponentRenderableObject *l_pComp = new CComponentRenderableObject();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->InitAnimatedModel(_pEntity, _szName, _szCore))
  {
    l_pComp->SetEntity(_pEntity);
    //s'agrega el component d'animacions
    if(!CComponentAnimation::AddToEntity(_pEntity))
      LOGGER->AddNewLog(ELL_WARNING, "CComponentRenderableObject::InitAnimatedModel No s'ha pogut inicialitzar el component d'animació.");
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentRenderableObject::Init(CGameEntity *_pEntity, const string& _szName, const string& _szCore)
{

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  if(!m_pObject3D)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CComponentRenderableObject::Init necessita un Component Object 3D");
    return false;
  }

  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();

  m_szResourceName = _szName;
  m_pRenderableObject = l_pROM->AddMeshInstance(_szCore, _szName);
  //m_pRenderableObject = l_pROM->GetResource(_szCore);

  if(!m_pRenderableObject)
  {
    SetOk(false);
  } else {
    SetOk(true);

    Mat44f l_Matrix;
    m_pRenderableObject->GetMat44(l_Matrix);
    m_pObject3D->SetMat44(l_Matrix);
  }

  return IsOk();
}

bool CComponentRenderableObject::InitAnimatedModel(CGameEntity *_pEntity, const string& _szName, const string& _szCore)
{
  assert(_pEntity->IsOk());

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  if(!m_pObject3D)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CComponentRenderableObject::InitAnimatedModel necessita un Component Object 3D");
    return false;
  }

  //CRenderableAnimatedInstanceModel* l_pAnimatedModel = new CRenderableAnimatedInstanceModel(_szName);
  //if(!l_pAnimatedModel->Init(_szCore))
  //{
  //  LOGGER->AddNewLog(ELL_WARNING, "CComponentRenderableObject::InitAnimatedModel No s'ha pogut carregar el CRenderableAnimatedInstanceModel de la core \"%s\"", _szCore.c_str());
  //  delete l_pAnimatedModel;
  //  return false;
  //}

  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();

  m_szResourceName = _szName;
  m_pRenderableObject = l_pROM->AddAnimatedModel(_szCore, _szName);//l_pAnimatedModel;

  //l_pROM->AddResource(_szName, m_pRenderableObject);
  m_bRemoveRenderableObject = true;

  if(!m_pRenderableObject)
  {
    SetOk(false);
  } else {
    SetOk(true);
  }


  m_bActive = true;
  return IsOk();
}

void CComponentRenderableObject::SetYaw(float _fYaw)
{
  m_pRenderableObject->SetYaw(_fYaw);
}

void CComponentRenderableObject::PostUpdate(float _fDeltaTime)
{
  assert(IsOk());

  if(m_bActive)
  {
    if(m_bBlockPitchRoll || m_fHeightAdjustment != 0.f || m_fYawAdjustment != 0.f)
    {
      if(!m_bBlockPitchRoll)
      {
        m_pRenderableObject->SetPitch(m_pObject3D->GetPitch());
        m_pRenderableObject->SetRoll (m_pObject3D->GetRoll());
      }

      Vect3f l_vPosition = m_pObject3D->GetPosition();
      l_vPosition.y += m_fHeightAdjustment;
      m_pRenderableObject->SetPosition(l_vPosition);

      if(!m_bBlockYaw)
      {
        m_pRenderableObject->SetYaw (m_pObject3D->GetYaw() + m_fYawAdjustment);
      }
    }
    else
    {   
      Mat44f l_Matrix;
  
      m_pObject3D->GetMat44(l_Matrix);
      m_pRenderableObject->SetMat44(l_Matrix);
    }
  }
}

bool CComponentRenderableObject::ChangeInstance(const string& _szName)
{
  if(m_pRenderableObject)
  {
    if(m_pRenderableObject->ChangeInstance(_szName))
    {
      CComponentPhysXActor* l_pPsXActor = GetEntity()->GetComponent<CComponentPhysXActor>(ECT_PHYSX_ACTOR);
      if(l_pPsXActor)
        l_pPsXActor->Reload();
      return true;
    }
  }
  return false;
}


void CComponentRenderableObject::Release()
{
  if(m_bRemoveRenderableObject)
  {
    CORE->GetRenderableObjectsManager()->RemoveResource(m_szResourceName);
  }
}
