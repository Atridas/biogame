#include "SingleRenderTarget.h"
#include "RenderManager.h"

void CSingleRenderTarget::Activate(CRenderManager* l_pRM)
{
  l_pRM->SetDepthStencilBuffer(m_pDepthStencilSurface);
  Activate(l_pRM,0);
}

void CSingleRenderTarget::Activate(CRenderManager* l_pRM, int _iIndex)
{
  l_pRM->SetRenderTarget(_iIndex, m_pSurface);
}

void CSingleRenderTarget::Release()
{
  CHECKED_RELEASE(m_pSurface);
  CRenderTarget::Release();
}