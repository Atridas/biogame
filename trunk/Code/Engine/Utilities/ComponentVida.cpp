#include "ComponentVida.h"


bool CComponentVida::Init(CGameEntity* _pEntity, float _fVidaInicial)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_fVida = _fVidaInicial;

  SetOk(true);
  return IsOk();
}


void CComponentVida::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    assert(_Event.Info[0].Type == SEventInfo::STI_FLOAT);

    m_fVida -= _Event.Info[0].f;

    if(m_fVida <= 0.f)
    {
      SEvent l_morir;
      l_morir.Msg = SEvent::MORIR;
      l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
      GetEntity()->ReceiveEvent(l_morir);//TODO usar un manager
    }
  }
}