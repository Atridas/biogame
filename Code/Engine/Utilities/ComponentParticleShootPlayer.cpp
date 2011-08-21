#include "ComponentParticleShootPlayer.h"
//#include "RenderableObjectsManager.h"
#include "Core.h"
#include "BillBoardManager.h"

//---------------------------------------------
/*#include "AnimatedInstanceModel.h"
#include "cal3d/cal3d.h"
#include "ComponentRenderableObject.h"
#include "Component3rdPSCamera.h"
#include "PhysicsManager.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "ComponentMovement.h"*/



CComponentParticleShootPlayer* CComponentParticleShootPlayer::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd)
{

  CComponentParticleShootPlayer *l_pComp = new CComponentParticleShootPlayer();
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


bool CComponentParticleShootPlayer::Init(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd)
{
  
  m_vPosInit = _vPosInit;
  m_vPosActual = _vPosInit;
  m_vPosEnd  = _vPosEnd ;
  m_pBillboard = CORE->GetBillBoardManager()->GetBillBoard("shoot");
  m_pBillboard->SetPos(m_vPosActual);
  m_pBillboard->SetActive(true);
  m_bActive=true;
  m_vPosDiferencial1 = (0.2f, 0.2f, 0.2f);
  

  SetOk(true);
  return IsOk();
}


void CComponentParticleShootPlayer::Release()
{

}


void CComponentParticleShootPlayer::Update(float _fDeltaTime)
{
  if(!m_bActive)
    return;
  m_vPosActual = InterPolaterNumber(m_vPosActual,m_vPosEnd,0.1f,_fDeltaTime);
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
        m_pBillboard = CORE->GetBillBoardManager()->GetBillBoard("impacteShootPlayer");
        m_pBillboard->SetPos(m_vPosActual);
        m_pBillboard->SetActive(true);
        m_bActive=false;
      }
    }
  }
}

