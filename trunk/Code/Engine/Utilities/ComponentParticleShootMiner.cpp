#include "ComponentParticleShootMiner.h"
#include "Core.h"
#include "BillBoardManager.h"



CComponentParticleShootMiner* CComponentParticleShootMiner::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd)
{

  CComponentParticleShootMiner *l_pComp = new CComponentParticleShootMiner();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vPosInit, _vPosEnd))
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


bool CComponentParticleShootMiner::Init(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd)
{
  
  m_vPosInit = _vPosInit;
  m_vPosActual = _vPosInit;
  m_vPosEnd  = _vPosEnd ;
  m_pBillboard = CORE->GetBillBoardManager()->CreateBillBorad("disparBuzzo","shootBuzzo1",m_vPosInit);
  m_pBillboard->SetPos(m_vPosActual);
  m_pBillboard->SetActive(true);
  m_bActive=true;
  m_vPosDiferencial1 = (0.2f, 0.2f, 0.2f);
  m_fTime= 0.0f;
  

  SetOk(true);
  return IsOk();
}


void CComponentParticleShootMiner::Release()
{

}


void CComponentParticleShootMiner::Update(float _fDeltaTime)
{
  if(!m_bActive)
    return;
  m_vPosActual = InterpolateNumber(m_vPosActual,m_vPosEnd,0.1f,_fDeltaTime);
  m_pBillboard->SetPos(m_vPosActual);
  float l_fCompara;
  l_fCompara = abs(m_vPosActual.x-m_vPosEnd.x);
  if(m_vPosDiferencial1.x>l_fCompara)
  {
    l_fCompara = abs(m_vPosActual.y-m_vPosEnd.y);
    if(m_vPosDiferencial1.y>l_fCompara)
    {
      l_fCompara = abs(m_vPosActual.z-m_vPosEnd.z);
      if(m_vPosDiferencial1.z>l_fCompara)
      {
        m_pBillboard->SetActive(false);
        m_pBillboard = CORE->GetBillBoardManager()->CreateBillBorad("impacte","impacteShootBuzzo1",m_vPosActual);
        //m_pBillboard->SetPos(m_vPosActual);
        m_pBillboard->SetActive(true);
        m_bActive=false;
      }
    }
  }
 /* m_fTime+=_fDeltaTime;
  m_pBillboard->SetPos(m_vPosActual);
  if(m_fTime>1)
  {
    m_pBillboard->SetActive(false);
    m_bActive=false;
    m_fTime=0.0f;
  }*/
}