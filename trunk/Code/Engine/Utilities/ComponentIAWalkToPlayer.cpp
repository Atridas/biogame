#include "ComponentIAWalkToPlayer.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"

bool CComponentIAWalkToPlayer::Init(CGameEntity *_pEntity,
            const string& _szPlayerEntityName,
            float _fWalkSpeed)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pMovement = dynamic_cast<CComponentMovement*>(_pEntity->GetComponent(ECT_MOVEMENT));
  assert(m_pMovement); //TODO fer missatges d'error m�s elavorats

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error m�s elavorats


  m_fWalkSpeed  = _fWalkSpeed;

  CGameEntity* l_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);
  m_pPlayerPosition = dynamic_cast<CComponentObject3D*>(l_pPlayer->GetComponent(ECT_OBJECT_3D));

  SetOk(true);
  return IsOk();
}

void CComponentIAWalkToPlayer::Update(float _fDeltaTime)
{
  assert(IsOk());
  
  Vect3f l_MyPosition     = m_pObject3D->GetPosition();
  Vect3f l_PlayerPosition = m_pPlayerPosition->GetPosition();

  Vect3f l_Direction = (l_PlayerPosition - l_MyPosition).Normalize();

  m_pMovement->m_vMovement += l_Direction * m_fWalkSpeed * _fDeltaTime;
}
