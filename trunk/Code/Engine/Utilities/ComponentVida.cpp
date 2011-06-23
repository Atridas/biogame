#include "ComponentVida.h"
#include "Core.h"

CComponentVida* CComponentVida::AddToEntity(CGameEntity* _pEntity, float _fVidaInicial)
{
  CComponentVida *l_pComp = new CComponentVida();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fVidaInicial))
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

bool CComponentVida::Init(CGameEntity* _pEntity, float _fVidaInicial)
{
  m_fVida = _fVidaInicial;

  SetOk(true);
  return IsOk();
}


void CComponentVida::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE && !m_bImmortal)
  {
    assert(_Event.Info[0].Type == SEventInfo::FLOAT);

    m_fVida -= _Event.Info[0].f;

    if(m_fVida <= 0.f)
    {
      SEvent l_morir;
      l_morir.Msg = SEvent::MORIR;
      l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
      CORE->GetEntityManager()->SendEvent(l_morir);
      //GetEntity()->ReceiveEvent(l_morir);//TODO usar un manager
    }
  }
}