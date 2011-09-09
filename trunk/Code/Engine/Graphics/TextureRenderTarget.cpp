#include "TextureRenderTarget.h"
#include "XML/XMLTreeNode.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Core.h"
#include "RenderManager.h"

bool CTextureRenderTarget::Init(CXMLTreeNode& _treeRenderTarget, int _iWidth, int _iHeight, const string& _szFormat)
{
  int l_iIndex = _treeRenderTarget.GetIntProperty("index",0,false);
  string l_szTextureName = _treeRenderTarget.GetPszISOProperty("texture","",false);

  m_iWidth = _iWidth;
  m_iHeight = _iHeight;

  if(l_szTextureName != "")
  {
    m_szTextureName = l_szTextureName;
    CRenderTarget::Init(l_iIndex);

    m_pTexture = new CTexture();

    if(m_pTexture->Create(l_szTextureName,
                          m_iWidth,
                          m_iHeight,
                          1,
                          CTexture::RENDERTARGET,
                          CTexture::DEFAULT,
                          CTexture::GetFormatTypeFromString(_szFormat)))
    {
      m_pSurface = m_pTexture->GetSurface();
      m_pDepthStencilSurface = m_pTexture->GetDepthStencilSurface();
    } else
    {
      m_pSurface = 0;
      m_pDepthStencilSurface = 0;
    }

    if(m_pTexture->IsOk() && m_pSurface && m_pDepthStencilSurface)
    {
      SetOk(true);
      CORE->GetTextureManager()->AddResource(l_szTextureName, m_pTexture);
    } else
    {
      if(!m_pTexture->IsOk())
        LOGGER->AddNewLog(ELL_ERROR, "CTextureRenderTarget::Init  Texture is not Ok.");
      if(!m_pSurface)
        LOGGER->AddNewLog(ELL_ERROR, "CTextureRenderTarget::Init  Surface is not Ok.");
      if(!m_pDepthStencilSurface)
        LOGGER->AddNewLog(ELL_ERROR, "CTextureRenderTarget::Init  DepthStencilSurface is not Ok.");
      SetOk(false);
      CHECKED_DELETE(m_pTexture);
    }

  }else{
    LOGGER->AddNewLog(ELL_WARNING,"CTextureRenderTarget::Init render_target sense textura");
    SetOk(false);
  }
          
  return IsOk();
}

void CTextureRenderTarget::Activate()
{
  CRenderManager* l_pRM = RENDER_MANAGER;
  CColor l_Color = l_pRM->GetClearColor();
  uint32 l_uiRed		= (uint32) (l_Color.GetRed() * 255);
	uint32 l_uiGreen	= (uint32) (l_Color.GetGreen() * 255);
	uint32 l_uiBlue		= (uint32) (l_Color.GetBlue() * 255);

  l_pRM->GetDevice()->ColorFill( m_pSurface, 0, D3DCOLOR_ARGB(0,l_uiRed,l_uiGreen,l_uiBlue));

  CRenderTarget::Activate();
}

void CTextureRenderTarget::Release()
{
  CRenderTarget::Release();
}