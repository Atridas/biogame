#include "FrameBufferInputSampler.h"
#include <d3dx9.h>
#include "Core.h"
#include "RenderManager.h"
#include "Texture.h"

bool CFrameBufferInputSampler::Init(int _iIndex)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CFrameBufferInputSampler::Init iniciant CFrameBufferInputSampler a index %d.",_iIndex);

  CInputSampler::Init(_iIndex);

  int l_iWidth = RENDER_MANAGER->GetScreenWidth();
  int l_iHeight = RENDER_MANAGER->GetScreenHeight();

  m_pTexture = new CTexture();

  if(m_pTexture->Create("back_buffer",
                        l_iWidth,
                        l_iHeight,
                        1,
                        CTexture::RENDERTARGET,
                        CTexture::DEFAULT,
                        CTexture::GetFormatTypeFromString("A8R8G8B8")))
  {
    SetOk(true);
  }else{
    LOGGER->AddNewLog(ELL_ERROR,"CFrameBufferInputSampler::Init no s'ha pogut crear la textura.");
    SetOk(false);
  }

  return IsOk();
}

void CFrameBufferInputSampler::Release()
{
  CInputSampler::Release();

  CHECKED_DELETE(m_pTexture);
}

void CFrameBufferInputSampler::Activate()
{
  LPDIRECT3DSURFACE9 l_pRenderTarget = 0;
  LPDIRECT3DDEVICE9  l_pDevice = RENDER_MANAGER->GetDevice();
  HRESULT hr = l_pDevice->GetRenderTarget(0,&l_pRenderTarget);
  assert(SUCCEEDED(hr) && "CFrameBufferInputSampler::Activate error getting render target");

  hr = l_pDevice->StretchRect( l_pRenderTarget, NULL, m_pTexture->GetSurface(), NULL, D3DTEXF_NONE);
  assert(SUCCEEDED(hr) && "CFrameBufferInputSampler::Activate error stretching texture");

  l_pRenderTarget->Release();

  CInputSampler::Activate();
}