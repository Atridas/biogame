#include "ComponentEmiter.h"
#include "ComponentObject3D.h"
#include "EmiterManager.h"
#include "EmiterInstance.h"
#include "Core.h"

CComponentEmiter* CComponentEmiter::AddToEntity(CGameEntity* _pEntity, const string& _szCore, const Vect3f& _vVolume)
{
  CComponentEmiter *l_pComp = new CComponentEmiter();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szCore, _vVolume))
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

bool CComponentEmiter::Init(CGameEntity* _pEntity, const string& _szCore, const Vect3f& _vVolume)
{
  CComponentObject3D* l_pCO3D = _pEntity->GetComponent<CComponentObject3D>();
  if(!l_pCO3D)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentEmiter::Init needs component object3D");
    SetOk(false);
    return false;
  }

  m_szEmiterName  = "Entity Emiter ";
  m_szEmiterName += _pEntity->GetGUID();

  m_pEmiterInstance = CORE->GetEmiterManager()->CreateEmiter(m_szEmiterName, _szCore, *l_pCO3D, _vVolume);
  m_pEmiterInstance->SetReference(l_pCO3D);

  SetOk(true);
  return IsOk();
}

void CComponentEmiter::Release()
{
  CORE->GetEmiterManager()->DeleteEmiter(m_szEmiterName);

  m_pEmiterInstance = 0;
  m_szEmiterName = "";
}

void CComponentEmiter::ChangeCore(const string& _szCore)
{
  m_pEmiterInstance->Reset(_szCore);
}

void CComponentEmiter::ChangeVolume(const Vect3f& _vVolume)
{
  m_pEmiterInstance->Reset(_vVolume);
}