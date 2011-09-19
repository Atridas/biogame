#include "TextureRenderTarget.h"
#include "XML/XMLTreeNode.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Core.h"
#include "RenderManager.h"

bool CTextureRenderTarget::Init(CXMLTreeNode& _treeRenderTarget)
{
  string l_szName =  _treeRenderTarget.GetPszISOProperty("name","",false);
  string l_szTextureName = _treeRenderTarget.GetPszISOProperty("texture","",false);
  string l_szFormat = _treeRenderTarget.GetPszISOProperty("format","A8R8G8B8");

  int l_iWidth = (int)CORE->GetRenderManager()->GetScreenWidth();

  m_iWidth = _treeRenderTarget.GetIntProperty("width",(int)RENDER_MANAGER->GetScreenWidth(),false);
  m_iHeight = _treeRenderTarget.GetIntProperty("height",(int)RENDER_MANAGER->GetScreenHeight(),false);

  CRenderTarget::Init(m_iWidth, m_iHeight);

  SetOk(true);

  if(l_szName != "")
  {
    SetName(l_szName);

    if(l_szTextureName != "")
    {
      m_szTextureName = l_szTextureName;

      CTexture* l_pTexture = new CTexture();

      if(l_pTexture->Create(l_szTextureName,
                            m_iWidth,
                            m_iHeight,
                            1,
                            CTexture::RENDERTARGET,
                            CTexture::DEFAULT,
                            CTexture::GetFormatTypeFromString(l_szFormat)))
      {
        m_pSurface = l_pTexture->GetSurface();
        m_pSurface->AddRef();
      } else
      {
        m_pSurface = 0;
      }

      if(l_pTexture->IsOk() && m_pSurface)
      {
        SetOk(true);
        CORE->GetTextureManager()->AddResource(l_szTextureName, l_pTexture);
      } else
      {
        if(!l_pTexture->IsOk())
          LOGGER->AddNewLog(ELL_ERROR, "CTextureRenderTarget::Init  Texture is not Ok.");
        if(!m_pSurface)
          LOGGER->AddNewLog(ELL_ERROR, "CTextureRenderTarget::Init  Surface is not Ok.");
        SetOk(false);
        CHECKED_DELETE(l_pTexture);
      }

    }else{
      LOGGER->AddNewLog(ELL_WARNING,"CTextureRenderTarget::Init render_target sense textura");
      SetOk(false);
    }
  }else{
    LOGGER->AddNewLog(ELL_WARNING,"CTextureRenderTarget::Init render_target sense nom");
    SetOk(false);
  }
          
  return IsOk();
}

//void CTextureRenderTarget::Activate(CRenderManager* l_pRM, int _iIndex)
//{
//  CColor l_Color = l_pRM->GetClearColor();
//  uint32 l_uiRed		= (uint32) (l_Color.GetRed() * 255);
//	uint32 l_uiGreen	= (uint32) (l_Color.GetGreen() * 255);
//	uint32 l_uiBlue		= (uint32) (l_Color.GetBlue() * 255);
//
//  l_pRM->GetDevice()->ColorFill( m_pSurface, 0, D3DCOLOR_ARGB(0,l_uiRed,l_uiGreen,l_uiBlue));
//
//  CSingleRenderTarget::Activate(l_pRM, _iIndex);
//}

void CTextureRenderTarget::Release()
{
  m_szTextureName = "";
  m_iWidth = 0;
  m_iHeight = 0;

  CSingleRenderTarget::Release();
}