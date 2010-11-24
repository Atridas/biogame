#include "Texture.h"

#include "RenderManager.h"
#include "Core.h"


bool CTexture::LoadFile() 
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Load \"%s\"",m_szFileName.c_str());

  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();

  HRESULT l_Result = D3DXCreateTextureFromFile( l_pDevice, m_szFileName.c_str(), &m_pTexture);

  if(l_Result == D3D_OK )
    return true;

  //TODO fer logs més millors

  if(l_Result == D3DERR_NOTAVAILABLE) {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load D3DERR_NOTAVAILABLE");
  } else if(l_Result == D3DERR_OUTOFVIDEOMEMORY) {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load D3DERR_OUTOFVIDEOMEMORY");
  } else if(l_Result == D3DERR_NOTAVAILABLE) {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load D3DERR_INVALIDCALL");
  } else if(l_Result == D3DERR_NOTAVAILABLE) {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load D3DERR_INVALIDDATA");
  } else if(l_Result == E_OUTOFMEMORY) {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load E_OUTOFMEMORY");
  }

  return false;
}

void CTexture::Unload()
{
   LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Unload");

   CHECKED_RELEASE(m_pTexture)
}

bool CTexture::Load(const std::string &_szFileName)
{
  if(m_szFileName != "")
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTexture::Load carregant \"%s\" quan ja hi ha \"%s\" carregada",_szFileName.c_str(),m_szFileName.c_str());
    Unload();
  }
  m_szFileName = _szFileName;
  return LoadFile();
}

bool CTexture::Reload()
{
  Unload();
  return LoadFile();
}

void CTexture::Activate(size_t _StageId)
{
  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();

  HRESULT l_Result = l_pDevice->SetTexture(_StageId,m_pTexture);

  if(l_Result == D3D_OK)
    return;
  if(l_Result == D3DERR_INVALIDCALL) 
  {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Activate D3DERR_INVALIDCALL");
    //TODO exception?
  }
}