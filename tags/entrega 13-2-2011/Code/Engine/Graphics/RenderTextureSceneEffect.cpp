#include "RenderTextureSceneEffect.h"
#include "RenderManager.h"
#include "Core.h"
#include "TextureManager.h"
#include "Texture.h"
#include "EffectMaterial.h"
#include "XML/XMLTreeNode.h"

bool CRenderTextureSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderTextureSceneEffect::Init  Initializing CRenderTextureSceneEffect.");

  if(!CSceneEffect::Init(_params))
    return false;


  m_pTexture = new CTexture();
  string l_szTexture = _params.GetPszISOProperty("texture","");
  uint32 l_iWidth, l_iHeight;
  if(_params.GetBoolProperty("texture_size_as_frame_buffer",false))
  {
    float l_fScaleSize = _params.GetFloatProperty("scale_size", 1.0f);
    l_iWidth  = (uint32)((float)RENDER_MANAGER->GetScreenWidth()  * l_fScaleSize);
    l_iHeight = (uint32)((float)RENDER_MANAGER->GetScreenHeight() * l_fScaleSize);
  } else
  {
    l_iWidth  = _params.GetIntProperty("width");
    l_iHeight = _params.GetIntProperty("height");
  }
  string l_szformat_type = _params.GetPszISOProperty("format_type","");

  if(m_pTexture->Create(  l_szTexture,
                          l_iWidth,
                          l_iHeight,
                          1,
                          CTexture::RENDERTARGET,
                          CTexture::DEFAULT,
                          CTexture::GetFormatTypeFromString(l_szformat_type)
                      ))
  {
    m_pSurface = m_pTexture->GetSurface();
  } else
  {
    m_pSurface = 0;
  }
  //patillada total
  //if(m_pTexture->Create(_Atts.name, _Atts.width, _Atts.Height, _Atts.MipMaps, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8))
  //  m_pSurface = m_pTexture->GetSurface();
  //else
  //  m_pSurface = 0;
  
  if(m_pTexture->IsOk() && m_pSurface)
  {
    SetOk(true);
    RENDER_MANAGER->GetTextureManager()->AddResource(l_szTexture, m_pTexture);
  } else
  {
    if(!m_pTexture->IsOk())
      LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CRenderTextureSceneEffect  Texture is not Ok.");
    if(!m_pSurface)
      LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CRenderTextureSceneEffect  Surface is not Ok.");
    SetOk(false);
    CHECKED_DELETE(m_pTexture);
  }

  if(IsOk())
  {
    m_pEffectMaterial = new CEffectMaterial();
    m_pEffectMaterial->Init();
    m_pEffectMaterial->SetTextureWidth(l_iWidth);
    m_pEffectMaterial->SetTextureHeight(l_iHeight);
  }

  return IsOk();
}

void CRenderTextureSceneEffect::CaptureFrameBuffers(CRenderManager* _pRM)
{
  if(IsOk())
  {
    LPDIRECT3DSURFACE9 l_pRenderTarget = 0;
    LPDIRECT3DDEVICE9  l_pDevice = _pRM->GetDevice();
    HRESULT hr = l_pDevice->GetRenderTarget(0,&l_pRenderTarget);
    assert(SUCCEEDED(hr) && "CRenderTextureSceneEffect::CaptureFrameBuffers error getting render target");

    hr = l_pDevice->StretchRect( l_pRenderTarget, NULL, m_pSurface, NULL, D3DTEXF_NONE);
    assert(SUCCEEDED(hr) && "CRenderTextureSceneEffect::CaptureFrameBuffers error stretching texture");

    l_pRenderTarget->Release();
  }else
    LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CaptureFrameBuffers  CRenderTextureSceneEffect is not Ok.");
}

void CRenderTextureSceneEffect::Release()
{
  CSceneEffect::Release();
  CHECKED_DELETE(m_pEffectMaterial);
}