#include "RenderTextureSceneEffect.h"
#include "RenderManager.h"
#include "Core.h"
#include "TextureManager.h"
#include "Texture.h"
#include "XML/XMLTreeNode.h"

bool CRenderTextureSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderTextureSceneEffect::Init  Initializing CRenderTextureSceneEffect.");

  m_pTexture = new CTexture();

  string l_szName = _params.GetPszISOProperty("name","");
  SetName(l_szName);
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderTextureSceneEffect::Init  \"%s\".", l_szName.c_str());
  string l_szTexture = _params.GetPszISOProperty("texture","");
  uint32 l_iWidth = _params.GetIntProperty("width");
  uint32 l_iHeight = _params.GetIntProperty("height");
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
    CHECKED_DELETE(m_pTexture)
  }

  return IsOk();
}

void CRenderTextureSceneEffect::CaptureFrameBuffers(CRenderManager* _pRM)
{
  if(IsOk())
  {
    LPDIRECT3DSURFACE9 l_pRenderTarget = 0;

    _pRM->GetDevice()->GetRenderTarget(0,&l_pRenderTarget);
    _pRM->GetDevice()->StretchRect( l_pRenderTarget, NULL, m_pSurface, NULL, D3DTEXF_NONE);

    l_pRenderTarget->Release();
  }else
    LOGGER->AddNewLog(ELL_ERROR, "CRenderTextureSceneEffect::CaptureFrameBuffers  CRenderTextureSceneEffect is not Ok.");
}