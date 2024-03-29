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

bool CTextureManager::Init ()
{
  m_pDefaultTexture = new CTexture();
  SetOk(false);
  if(m_pDefaultTexture->Create("DefaultTexture",
      32,
      32,
      0,
      CTexture::DYNAMIC,
      CTexture::DEFAULT,
      CTexture::A8R8G8B8))
  {
    if(m_pDefaultTexture->FillDefaultTexture())
    {
      SetOk( true );
    }
  }

  if(IsOk())
  {
    m_pDefaultCubeTexture = new CTexture();
    if(!m_pDefaultCubeTexture->CreateDefaultCube(m_pDefaultTexture))
    {
      SetOk(false);
    }
  }

  if(!IsOk())
  {
		std::string msg_error = "CTextureManager::Init-> Error al intentar crear la defualtTexture en la inicializaci�n de CTextureManager";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    Release();
    //throw CException(__FILE__, __LINE__, msg_error);
  }
  else
  {
    AddResource("default", m_pDefaultTexture);
    AddResource("default cube", m_pDefaultCubeTexture);
  }
  return IsOk();
}

CTexture* CTextureManager::GetResource(const std::string &_szName)
{
  //return m_pDefaultTexture;

  CTexture* l_pTexture = CMapManager::GetResource(_szName);
  if(l_pTexture != 0)
  {
    if(!l_pTexture->IsCube())
      return l_pTexture;
    else
      return m_pDefaultTexture;
  }

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

//TODO potser millorar aix� una mica, no?
CTexture* CTextureManager::GetCubeTexture(const std::string &_szName)
{
  CTexture* l_pTexture = CMapManager::GetResource(_szName);
  if(l_pTexture != 0)
  {
    if(l_pTexture->IsCube())
      return l_pTexture;
    else
      return m_pDefaultCubeTexture;
  }

  l_pTexture = new CTexture();
  if(!l_pTexture->Load(_szName,true))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTextureManager::GetResource -> Textura no trobada.");
    CHECKED_DELETE(l_pTexture);
    return m_pDefaultCubeTexture;
  }
  CMapManager::AddResource(_szName,l_pTexture);
  return l_pTexture;
}

void CTextureManager::Release()
{
  //CHECKED_DELETE(m_pDefaultTexture);
  //CHECKED_DELETE(m_pDefaultCubeTexture);
  CMapManager::Release();
}