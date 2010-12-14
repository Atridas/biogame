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


CTexture* CTextureManager::GetResource(const std::string &_szName)
{
  CTexture* l_pTexture = CMapManager::GetResource(_szName);
  if(l_pTexture != 0)
    return l_pTexture;


  l_pTexture = new CTexture();
  if(!l_pTexture->Load(_szName))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTextureManager::GetResource -> veure log anterior.");
    return 0;
  }
  CMapManager::AddResource(_szName,l_pTexture);
  return l_pTexture;
}