#include "BackBufferRenderTarget.h"
#include "Core.h"
#include "RenderManager.h"

bool CBackBufferRenderTarget::Init()
{
  CRenderTarget::Init(0);

  RENDER_MANAGER->GetDevice()->GetRenderTarget(0,&m_pSurface);
  RENDER_MANAGER->GetDevice()->GetDepthStencilSurface(&m_pDepthStencilSurface);

  SetOk(true);
  return IsOk();
}

void CBackBufferRenderTarget::Release()
{
  CRenderTarget::Release();
  CHECKED_RELEASE(m_pSurface);
  CHECKED_RELEASE(m_pDepthStencilSurface);
}