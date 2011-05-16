#include "Explosion.h"


CExplosion::CExplosion(void)
{
}

void CExplosion::Update(float fElapsedTime, CCamera* camera)
{

}
  
void CExplosion::Init(CRenderManager* rm)
{

}
  
void CExplosion::Release()
{

}
  
void CExplosion::Render(CRenderManager* _pRM)
{

}
  
void CExplosion::SetAttributes(SExplosionInfo* _info)
{
  m_szId  = _info->m_szId;
  m_vPosition = _info->m_Position;
  m_fSizeX = _info->m_SizeX; 
  m_fSizeY = _info->m_SizeY;
  m_pTexExplosion = _info->m_pTexExplosion; 
  m_iTexNumFiles = _info->m_iTexNumFiles;
  m_iTexNumColumnes = _info->m_iTexNumColumnes;
  m_fTimeAnimationDiapo = _info->m_fTimeAnimationDiapo;
}