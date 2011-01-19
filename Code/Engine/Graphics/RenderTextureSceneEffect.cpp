#include "RenderTextureSceneEffect.h"
#include "RenderManager.h"
#include "Texture.h"

CRenderTextureSceneEffect::CRenderTextureSceneEffect(MKeyValue& _Atts):
m_pTexture(0), m_pSurface(0)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderTextureSceneEffect::CRenderTextureSceneEffect  Initializing CRenderTextureSceneEffect.");

  m_pTexture = new CTexture();

  //patillada total
  //if(m_pTexture->Create(_Atts.name, _Atts.width, _Atts.Height, _Atts.MipMaps, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8))
  //  m_pSurface = m_pTexture->GetSurface();
  //else
  //  m_pSurface = 0;
  
  if(m_pTexture->IsOk() && m_pSurface)
    SetOk(true);
  else
  {
    if(!m_pTexture->IsOk())
      LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CRenderTextureSceneEffect  Texture is not Ok.");
    if(!m_pSurface)
      LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CRenderTextureSceneEffect  Surface is not Ok.");
  }
}

void CRenderTextureSceneEffect::CaptureFrameBuffers(CRenderManager* _pRM)
{
  if(IsOk())
  {
    LPDIRECT3DSURFACE9 l_pRenderTarget = 0;

    //PSRender.GetDevice()->GetRenderTarget(0,&l_pRenderTarget);
    //PSRender.GetDevice()->StretchRect( l_pRenderTarget, NULL, m_pSurface, NULL, D3DTEXF_NONE);

    l_pRenderTarget->Release();
  }else
    LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CaptureFrameBuffers  CRenderTextureSceneEffect is not Ok.");
}