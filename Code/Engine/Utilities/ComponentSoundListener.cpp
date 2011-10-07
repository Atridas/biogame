#include "ComponentSoundListener.h"
#include "ComponentObject3D.h"
#include "Component3rdPSCamera.h"

#include "Core.h"
#include "SoundManager.h"
#include "Camera.h"

CComponentSoundListener* CComponentSoundListener::AddToEntity(CGameEntity* _pEntity)
{
  CComponentSoundListener *l_pComp = new CComponentSoundListener();
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

bool CComponentSoundListener::Init(CGameEntity* _pEntity)
{

  m_pO3D = _pEntity->GetComponent<CComponentObject3D>();
  m_pCamera = _pEntity->GetComponent<CComponent3rdPSCamera>();

  if(!m_pO3D || !m_pCamera)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CComponentSoundListener::Init necessita una entitat amb object3d i 3rdPSCamera");
    SetOk(false);
  }
  else
  {
    SetOk(true);
  }

  return IsOk();
}

void CComponentSoundListener::Update(float _fDeltaTime)
{
  CORE->GetSoundManager()->UpdateSound3DSystem(m_pO3D->GetPosition(), m_pCamera->GetCamera()->GetDirection());
}
