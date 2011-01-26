#include "Texture.h"

#include "RenderManager.h"
#include "Core.h"


bool CTexture::LoadFile() 
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Load \"%s\"",m_szFileName.c_str());

  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();

  HRESULT l_Result = D3DXCreateTextureFromFile( l_pDevice, m_szFileName.c_str(), &m_pTexture);

  D3DXIMAGE_INFO l_pTexInfo;

  HRESULT l_InfoResult = D3DXGetImageInfoFromFile(m_szFileName.c_str(),&l_pTexInfo);

  m_uiWidth = l_pTexInfo.Width;

  m_uiHeight = l_pTexInfo.Height;

  if(l_Result == D3D_OK && l_InfoResult == D3D_OK)
  {
    SetOk(true);
    return IsOk();
  }
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
  } else {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load unrecognized error");
  }

  if(l_InfoResult == D3DERR_INVALIDCALL) {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load error getting texture information from file");
  }

  return false;
}

void CTexture::Release()
{
   LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Unload");

   CHECKED_RELEASE(m_pTexture);
   CHECKED_RELEASE(m_pDepthStencilRenderTargetTexture);
}

bool CTexture::Load(const string &_szFileName)
{
  if(m_szFileName != "")
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTexture::Load carregant \"%s\" quan ja hi ha \"%s\" carregada",_szFileName.c_str(),m_szFileName.c_str());
    Release();
  }
  m_szFileName = _szFileName;
  return LoadFile();
}

bool CTexture::Reload()
{
  Release();
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

bool CTexture::Create(const string& _szName,
            unsigned int _uiWidth,
            unsigned int _uiHeight,
            unsigned int _uiMipMaps,
            TUsageType _UsageType,
            TPoolType _PoolType,
            TFormatType _FormatType)
{
  SetName(_szName);
  D3DPOOL l_Pool=D3DPOOL_DEFAULT;
  DWORD l_UsageType=D3DUSAGE_DYNAMIC;
  D3DFORMAT l_Format=D3DFMT_A8R8G8B8;
  bool l_CreateDepthStencilSurface=false;

  switch(_UsageType)
  {
  case DYNAMIC:
    l_UsageType=D3DUSAGE_DYNAMIC;
    break;
  case RENDERTARGET:
    l_CreateDepthStencilSurface=true;
    l_UsageType=D3DUSAGE_RENDERTARGET;
    break;
  }

  switch(_PoolType)
  {
  case DEFAULT:
    l_Pool=D3DPOOL_DEFAULT;
    break;
  case SYSTEMMEM:
    l_Pool=D3DPOOL_SYSTEMMEM;
    break;
  }

  switch(_FormatType)
  {
  case A8R8G8B8:
    l_Format=D3DFMT_A8R8G8B8;
    break;
  case R8G8B8:
    l_Format=D3DFMT_R8G8B8;
    break;
  case X8R8G8B8:
    l_Format=D3DFMT_X8R8G8B8;
    break;
  case R32F:
    l_Format=D3DFMT_R32F;
    break;
  }

  HRESULT hr=RENDER_MANAGER->GetDevice()->CreateTexture(_uiWidth,
                                                        _uiHeight,
                                                        _uiMipMaps,
                                                        l_UsageType,
                                                        l_Format,
                                                        l_Pool,
                                                        &m_pTexture,NULL);

  if(l_CreateDepthStencilSurface)
  {
    RENDER_MANAGER->GetDevice()->CreateDepthStencilSurface(_uiWidth,
                                                           _uiHeight,
                                                           D3DFMT_D24S8,D3DMULTISAMPLE_NONE,
                                                           0,
                                                           TRUE,
                                                           &m_pDepthStencilRenderTargetTexture,
                                                           NULL);
    assert(m_pDepthStencilRenderTargetTexture!=NULL);
  }

  assert(m_pTexture!=NULL);
  assert(hr==D3D_OK);

  m_uiWidth=_uiWidth;
  m_uiHeight=_uiHeight;

  if(SUCCEEDED(hr))
    SetOk(true);

  return IsOk();
}

void CTexture::Deactivate(size_t Stage)
{
  RENDER_MANAGER->GetDevice()->SetTexture((DWORD)Stage,NULL);
}

bool CTexture::SetAsRenderTarget()
{
  //TODO: Falten mil variables membre
  
  LPDIRECT3DDEVICE9 l_pDevice=RENDER_MANAGER->GetDevice();

  l_pDevice->GetRenderTarget(0, &m_pOldRenderTarget);
  
  if(FAILED( m_pTexture->GetSurfaceLevel( 0, &m_pRenderTargetTexture )  ) )
    return false;

  l_pDevice->SetRenderTarget( 0, m_pRenderTargetTexture );

  CHECKED_RELEASE(m_pRenderTargetTexture);

  if(FAILED( l_pDevice->GetDepthStencilSurface( &m_pOldDepthStencilRenderTarget ) ) )
    return false;

  l_pDevice->SetDepthStencilSurface( m_pDepthStencilRenderTargetTexture );
  
  return true;
}

void CTexture::UnsetAsRenderTarget()
{
  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();
  l_pDevice->SetDepthStencilSurface(m_pOldDepthStencilRenderTarget);
  CHECKED_RELEASE(m_pOldDepthStencilRenderTarget);
  l_pDevice->SetRenderTarget(0, m_pOldRenderTarget);
  CHECKED_RELEASE(m_pOldRenderTarget);
}

CTexture::TFormatType CTexture::GetFormatTypeFromString(const string &FormatType)
{
  if(FormatType=="R32F")
    return CTexture::R32F;
  else if(FormatType=="A8R8G8B8")
    return CTexture::A8R8G8B8;
  else if(FormatType=="R8G8B8")
   return CTexture::R8G8B8;
  else if(FormatType=="X8R8G8B8")
    return CTexture::X8R8G8B8;
  else
    //LOG("Format Type '%s' not recognized", FormatType.c_str());

  return CTexture::A8R8G8B8;
}