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