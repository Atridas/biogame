#include "BackBufferRenderTarget.h"
#include "Core.h"
#include "RenderManager.h"

bool CBackBufferRenderTarget::Init(const string& _szName)
{
  m_pSurface = RENDER_MANAGER->GetBackBuffer();
  m_pSurface->AddRef();

  SetName(_szName);

  CRenderTarget::Init();

  if(m_pSurface)
  {
    SetOk(true);
  }else{
    SetOk(false);
  }
  return IsOk();
}

int CBackBufferRenderTarget::GetWidth ()
{
  return RENDER_MANAGER->GetScreenWidth();
}

int CBackBufferRenderTarget::GetHeight()
{
  return RENDER_MANAGER->GetScreenHeight();
}