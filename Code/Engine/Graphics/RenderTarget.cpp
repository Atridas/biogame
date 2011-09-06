#include "RenderTarget.h"
#include "Core.h"
#include "RenderManager.h"

void CRenderTarget::Activate()
{
  LPDIRECT3DDEVICE9 l_pDevice=RENDER_MANAGER->GetDevice();

  if(m_iIndex == 0)
  {
    l_pDevice->GetRenderTarget(0,&m_pPrevSurface);
    l_pDevice->GetDepthStencilSurface(&m_pPrevDepthStencilSurface);
    
    //TODO: Un sol DepthStencil per tots els render_targets actius
    l_pDevice->SetDepthStencilSurface(m_pDepthStencilSurface);
  }


  l_pDevice->SetRenderTarget(m_iIndex, m_pSurface);
}

void CRenderTarget::Deactivate()
{
  LPDIRECT3DDEVICE9 l_pDevice=RENDER_MANAGER->GetDevice();

  if(m_iIndex == 0)
  {
    l_pDevice->SetRenderTarget(0,m_pPrevSurface);
    l_pDevice->SetDepthStencilSurface(m_pPrevDepthStencilSurface);
    
    CHECKED_RELEASE(m_pPrevSurface);
    CHECKED_RELEASE(m_pPrevDepthStencilSurface);
  }
  else
  {
    l_pDevice->SetRenderTarget(m_iIndex,0); //ho posem a null
  }
}

void CRenderTarget::Release()
{
  //CHECKED_RELEASE(m_pSurface);
  //CHECKED_RELEASE(m_pDepthStencilSurface);
}