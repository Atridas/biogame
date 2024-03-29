#include "RenderTarget.h"
#include "Core.h"
#include "RenderManager.h"


void CRenderTarget::Init(int m_iWidth, int m_iHeight)
{
  if(m_iWidth == RENDER_MANAGER->GetScreenWidth() && m_iHeight == RENDER_MANAGER->GetScreenHeight())
  {
    Init();
  }
  else
  {
    RENDER_MANAGER->GetDevice()->CreateDepthStencilSurface(m_iWidth,
                                                           m_iHeight,
                                                           D3DFMT_D24S8,D3DMULTISAMPLE_NONE,
                                                           0,
                                                           TRUE,
                                                           &m_pDepthStencilSurface,
                                                           NULL);
    assert(m_pDepthStencilSurface);
  }
}

void CRenderTarget::Init()
{
  m_pDepthStencilSurface = RENDER_MANAGER->GetDepthStencilBuffer();
  m_pDepthStencilSurface->AddRef();
}

void CRenderTarget::Release()
{
  CHECKED_RELEASE(m_pDepthStencilSurface);
}

/*void CRenderTarget::Activate(CRenderManager* l_pRM)
{
  LPDIRECT3DDEVICE9 l_pDevice=l_pRM->GetDevice();

  if(m_iIndex == 0)
  {
    l_pDevice->GetRenderTarget(0,&m_pPrevSurface);
    l_pDevice->GetDepthStencilSurface(&m_pPrevDepthStencilSurface);
    
    //TODO: Un sol DepthStencil per tots els render_targets actius
    l_pDevice->SetDepthStencilSurface(m_pDepthStencilSurface);
    //l_pRM->SetDepthStencilBuffer(m_pDepthStencilSurface);
  }


  l_pDevice->SetRenderTarget(m_iIndex, m_pSurface);
  //l_pRM->SetRenderTarget(m_iIndex, m_pSurface);
}

void CRenderTarget::Deactivate(CRenderManager* l_pRM)
{
  LPDIRECT3DDEVICE9 l_pDevice=l_pRM->GetDevice();

  if(m_iIndex == 0)
  {
    l_pDevice->SetRenderTarget(0,m_pPrevSurface);
    l_pDevice->SetDepthStencilSurface(m_pPrevDepthStencilSurface);
    //
    CHECKED_RELEASE(m_pPrevSurface);
    CHECKED_RELEASE(m_pPrevDepthStencilSurface);
  }
  else
  {
    l_pDevice->SetRenderTarget(m_iIndex,0); //ho posem a null
  }
}*/

//void CRenderTarget::Release()
//{
  //CHECKED_RELEASE(m_pSurface);
  //CHECKED_RELEASE(m_pDepthStencilSurface);
//}