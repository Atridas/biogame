#include "TextureManager.h"
#include "Texture.h"

void CTextureManager::Reload ()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CTextureManager::Reload");

  TMapResource::iterator l_End = m_Resources.end();
  for(TMapResource::iterator l_It = m_Resources.begin(); l_It != l_End; ++l_It)
  {
    l_It->second->Reload();
  }
}

void CTextureManager::Init ()
{
  m_pDefaultTexture = new CTexture;
	//m_pDefaultTexture->SetAsDefaultTexture();
  bool isOk = false;
  if(m_pDefaultTexture->Create("DefaultTexture",50,50,0,CTexture::TUsageType::RENDERTARGET,CTexture::TPoolType::DEFAULT,CTexture::TFormatType::A8R8G8B8))
  {
    if(m_pDefaultTexture->FillTextureWithColor(50, 50, colGREEN))
    {
      isOk = true;
    }
  }

  if(!isOk)
  {
		std::string msg_error = "CTextureManager::Init-> Error al intentar crear la defualtTexture en la inicialización de CTextureManager";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    //throw CException(__FILE__, __LINE__, msg_error);
  }
}

CTexture* CTextureManager::GetResource(const std::string &_szName)
{
  CTexture* l_pTexture = CMapManager::GetResource(_szName);
  if(l_pTexture != 0)
    return l_pTexture;

  l_pTexture = new CTexture();
  if(!l_pTexture->Load(_szName))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTextureManager::GetResource -> Textura no trobada.");
    CHECKED_DELETE(l_pTexture);
    return m_pDefaultTexture;
  }
  CMapManager::AddResource(_szName,l_pTexture);
  return l_pTexture;
}