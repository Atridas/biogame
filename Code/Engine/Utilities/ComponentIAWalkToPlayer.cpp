#include "ComponentIAWalkToPlayer.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"

bool CComponentIAWalkToPlayer::Init(CGameEntity *_pEntity,
            const string& _szPlayerEntityName,
            float _fWalkSpeed,
            const string& _szWalkAnimation,
            const string& _szRebreImpacte)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pMovement = _pEntity->GetComponent<CComponentMovement>(ECT_MOVEMENT);
  assert(m_pMovement); //TODO fer missatges d'error més elavorats

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats


  m_fWalkSpeed  = _fWalkSpeed;

  CGameEntity* l_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);
  m_pPlayerPosition = l_pPlayer->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);


  // Animacions
  CComponentRenderableObject *l_pComponentRO = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  assert(l_pComponentRO); //TODO fer missatges d'error més elavorats
  CRenderableAnimatedInstanceModel* l_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
  assert(l_pAnimatedModel); //TODO fer missatges d'error més elavorats
  
  l_pAnimatedModel->GetAnimatedInstanceModel()->BlendCycle(_szWalkAnimation,0.f);
  // ----------------

  m_szRebreImpacte = _szRebreImpacte;

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


  //Calculem l'angle
  Vect2f l_vAux = l_Direction.GetProjZX().Normalize();
  float cosinus = l_vAux * Vect2f(0,1);
  float y       = (l_Direction ^ Vect3f(1,0,0)).y;

  float angle = acos(cosinus);
  if(y < 0)
  {
    angle = -angle;
  }

  m_pObject3D->SetYaw(angle);
}

void CComponentIAWalkToPlayer::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    CComponentRenderableObject *l_pComponentRO = GetEntity()->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
    assert(l_pComponentRO); //TODO fer missatges d'error més elavorats
    CRenderableAnimatedInstanceModel* l_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
    assert(l_pAnimatedModel); //TODO fer missatges d'error més elavorats

    
    int l_iAnim = l_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimationId(m_szRebreImpacte);
    l_pAnimatedModel->GetAnimatedInstanceModel()->ExecuteAction(l_iAnim,0.2f);
  }
}