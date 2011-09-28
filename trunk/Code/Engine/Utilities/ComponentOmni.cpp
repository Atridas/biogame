#include "ComponentOmni.h"
#include "ComponentObject3D.h"
#include "Core.h"
#include "LightManager.h"
#include "OmniLight.h"

CComponentOmni* CComponentOmni::AddToEntity(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt)
{
  CComponentOmni *l_pComp = new CComponentOmni();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vOffsetPosition, _vColor, _fStartRangeAtt, _fEndRangeAtt))
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

bool CComponentOmni::Init(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt)
{
  m_vOffset = _vOffsetPosition;

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>();

  if(!m_pObject3D)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentOmni::Init no s'ha trobat el component Object3D");
    SetOk(false);
    return IsOk();
  }

  m_pOmni = CORE->GetLightManager()->CreateOmniLight(_pEntity->GetName().append("omni"), m_pObject3D->GetPosition() + _vOffsetPosition, _vColor, _fStartRangeAtt, _fEndRangeAtt);

  if(!m_pOmni)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentOmni::Init no s'ha pogut crear la omni \"%s\"", _pEntity->GetName().c_str());
    SetOk(false);
    return IsOk();
  }

  m_pOmni->SetActive(true);

  SetOk(true);
  return IsOk();
}

void CComponentOmni::Update(float _fDeltaTime)
{
  m_pOmni->SetPosition(m_pObject3D->GetPosition() + m_vOffset);
}

void CComponentOmni::Release()
{
  CORE->GetLightManager()->Remove(m_pOmni->GetName());
}