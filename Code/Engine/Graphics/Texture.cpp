#include "Texture.h"

#include "RenderManager.h"
#include "Core.h"


bool CTexture::LoadFile(bool _bCubeTexture) 
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Load \"%s\"",m_szFileName.c_str());

  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();

  HRESULT l_Result;
  
  if(_bCubeTexture)
  {
    l_Result = D3DXCreateCubeTextureFromFile(l_pDevice, m_szFileName.c_str(),&m_pCubeTexture);
  }
  else
  {
    l_Result  = D3DXCreateTextureFromFile( l_pDevice, m_szFileName.c_str(), &m_pTexture);
  }
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
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Load Error no reconegut[#%d] (probablement, fitxer no trobat).", l_Result);
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
   CHECKED_RELEASE(m_pTextureSurface0);
   CHECKED_RELEASE(m_pCubeTexture);
   CHECKED_RELEASE(m_pDepthStencilRenderTargetTexture);
}

bool CTexture::Load(const string &_szFileName, bool _bCubeTexture)
{
  if(m_szFileName != "")
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTexture::Load carregant \"%s\" quan ja hi ha \"%s\" carregada",_szFileName.c_str(),m_szFileName.c_str());
    Release();
  }
  m_szFileName = _szFileName;
  SetName(_szFileName);
  return LoadFile(_bCubeTexture);
}

bool CTexture::Reload()
{
  bool l_bCube = m_pCubeTexture != 0;
  Release();
  return LoadFile(l_bCube);
}

void CTexture::Activate(size_t _StageId)
{
  assert(IsOk());
  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();

  HRESULT l_Result = l_pDevice->SetTexture(_StageId,
                                  (m_pTexture)? (IDirect3DBaseTexture9*)m_pTexture : m_pCubeTexture);

  if(l_Result == D3D_OK)
    return;
  if(l_Result == D3DERR_INVALIDCALL) 
  {
    LOGGER->AddNewLog(ELL_ERROR, "CTexture::Activate D3DERR_INVALIDCALL");
    //TODO exception?
    assert(0 && "CTexture::Activate D3DERR_INVALIDCALL");
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
    //l_CreateDepthStencilSurface=true;
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
  case R16G16F:
    l_Format=D3DFMT_G16R16F;
    break;
  case R32G32F:
    l_Format=D3DFMT_G32R32F;
    break;
  case A16B16G16R16F:
    l_Format = D3DFMT_A16B16G16R16F;
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

  HRESULT hr2 = m_pTexture->GetSurfaceLevel(0,&m_pTextureSurface0);

  assert(hr2==D3D_OK);

  m_uiWidth=_uiWidth;
  m_uiHeight=_uiHeight;

  if(SUCCEEDED(hr) && SUCCEEDED(hr2))
    SetOk(true);


  return IsOk();
}

bool CTexture::CreateDefaultCube(CTexture* _pDefaultTexture)
{
  assert(!IsOk());
  LPDIRECT3DDEVICE9 l_pDevice = RENDER_MANAGER->GetDevice();
  
  HRESULT hr;
  
  hr = l_pDevice->CreateCubeTexture(32, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pCubeTexture, 0);

  if(FAILED(hr))
    return false;

  LPDIRECT3DSURFACE9 l_pFace[6];
  m_pCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &l_pFace[0]);
  m_pCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &l_pFace[1]);
  m_pCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &l_pFace[2]);
  m_pCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &l_pFace[3]);
  m_pCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &l_pFace[4]);
  m_pCubeTexture->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &l_pFace[5]);

  for(int i = 0; i < 6; i++)
  {
    hr = l_pDevice->StretchRect(_pDefaultTexture->m_pTextureSurface0, 0, l_pFace[i], 0, D3DTEXF_NONE);
    
    if(FAILED(hr))
      return false;

    l_pFace[i]->Release();
  }

  SetOk(true);
  return true;
}

void CTexture::Deactivate(size_t Stage)
{
  assert(IsOk());
  RENDER_MANAGER->GetDevice()->SetTexture((DWORD)Stage,NULL);
}

bool CTexture::SetAsRenderTarget()
{
  assert(IsOk());
  
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
  assert(IsOk());
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
  else if(FormatType=="R16G16F")
    return CTexture::R16G16F;
  else if(FormatType=="R32G32F")
    return CTexture::R32G32F;
  else if(FormatType=="A16B16G16R16F")
    return CTexture::A16B16G16R16F;
  else
    LOGGER->AddNewLog(ELL_WARNING,"CTexture::GetFormatTypeFromString Format Type '%s' not recognized", FormatType.c_str());

  return CTexture::A8R8G8B8;
}

bool CTexture::FillTextureWithColor (uint32 width, uint32 height, const CColor& color)
{
  if (m_pTexture==0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor-> m_pTexture no ha sido creado aun ");
    return false;
  }

  HRESULT hr;
  D3DLOCKED_RECT lock;
  hr=m_pTexture->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
  if(hr==D3D_OK)
  {
    uint8 *pTxtBuffer; // Bitmap buffer, texture buffer
    pTxtBuffer = (uint8*)lock.pBits;

		
    uint32 j = 0;
    for( uint32 cont = 0; cont< width * height; cont++)
    {
      //BLUE
      pTxtBuffer[cont*4 + 0] = (uint8)(color.GetBlue()*255);
      //GREEN
      pTxtBuffer[cont*4 + 1] = (uint8)(color.GetGreen()*255);
      //RED
      pTxtBuffer[cont*4 + 2] = (uint8)(color.GetRed()*255);
      //ALPHA
      pTxtBuffer[cont*4 + 3] = (uint8)(color.GetAlpha()*255);
    }
    hr=m_pTexture->UnlockRect(0);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor->Error en la llamada lockRect");
    return false;
  }

	m_uiWidth	= width;
	m_uiHeight	= height;

  return true;
}

bool CTexture::FillDefaultTexture ()
{
  if (m_pTexture==0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor-> m_pTexture no ha sido creado aun ");
    return false;
  }

  HRESULT hr;
  D3DLOCKED_RECT lock;
  hr=m_pTexture->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
  if(hr==D3D_OK)
  {
    uint8 *pTxtBuffer; // Bitmap buffer, texture buffer
    pTxtBuffer = (uint8*)lock.pBits;

    uint32 cont = 0;
    for( uint32 i = 0; i< 32; i++)
    {
      for( uint32 j = 0; j < 32; j++)
      {
        //BLUE
        pTxtBuffer[cont*4 + 0] = (uint8)255;
        //GREEN
        pTxtBuffer[cont*4 + 1] = (uint8)(i % 2)*255;
        //RED
        pTxtBuffer[cont*4 + 2] = (uint8)(j % 2)*255;
        //ALPHA
        pTxtBuffer[cont*4 + 3] = (uint8)255;

        cont++;
      }
    }
    hr=m_pTexture->UnlockRect(0);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor->Error en la llamada lockRect");
    return false;
  }

	m_uiWidth	= 32;
	m_uiHeight	= 32;

  return true;
}

bool CTexture::FillTextureFromBuffer (uint32 width, uint32 height, uint8* pImgBuffer)
{
  if (m_pTexture==0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureFromBuffer-> m_Texture no ha sido creado aun ");
    return false;
  }

  HRESULT hr;
  D3DLOCKED_RECT lock;
  hr=m_pTexture->LockRect(0,&lock,0,D3DLOCK_DISCARD);
  if(hr==D3D_OK)
  {
    uint8 *pTxtBuffer; // Bitmap buffer, texture buffer
    pTxtBuffer = (uint8*)lock.pBits;

    uint32 j = 0;
    for( uint32 cont = 0; cont< width * height; cont++)
    {
      //BLUE
      pTxtBuffer[cont*4 + 0] = pImgBuffer[j]; j++;
      //GREEN
      pTxtBuffer[cont*4 + 1] = pImgBuffer[j]; j++; 			
      //RED
      pTxtBuffer[cont*4 + 2] = pImgBuffer[j]; j++;
      //ALPHA
      pTxtBuffer[cont*4 + 3] = 255;
    }
    hr=m_pTexture->UnlockRect(0);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureFromBuffer->Error en la llamada lockRect");
    return false;
  }

	m_uiWidth	= width;
	m_uiHeight	= height;

  return true;
}