#include "ComponentBillboard.h"
#include "ComponentObject3D.h"
#include "EmiterManager.h"
#include "EmiterInstance.h"
#include "Core.h"

CComponentBillboard* CComponentBillboard::AddToEntity(CGameEntity* _pEntity, const string& _szCore)
{
  CComponentBillboard *l_pComp = new CComponentBillboard();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szCore))
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

bool CComponentBillboard::Init(CGameEntity* _pEntity, const string& _szCore)
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

  m_pEmiterInstance = CORE->GetEmiterManager()->CreateBillboard(m_szEmiterName, _szCore, CObject3D());
  m_pEmiterInstance->SetReference(l_pCO3D);

  SetOk(true);
  return IsOk();
}

void CComponentBillboard::Release()
{
  CORE->GetEmiterManager()->DeleteEmiter(m_szEmiterName);

  m_pEmiterInstance = 0;
  m_szEmiterName = "";
}

void CComponentBillboard::ChangeCore(const string& _szCore)
{
  m_pEmiterInstance->Reset(_szCore);
}

void CComponentBillboard::Enable()
{
  m_pEmiterInstance->SetVisible(true);
}

void CComponentBillboard::Disable()
{
  m_pEmiterInstance->SetVisible(false);
}
