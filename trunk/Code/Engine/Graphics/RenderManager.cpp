#include "Utils/Logger.h"
#include "Utils/Exception.h"
#include "RenderManager.h"
#include "Camera.h"
#include "params.h"
#include "VertexsStructs.h"
#include "Texture.h"
#include "EffectManager.h"


bool CRenderManager::Init(HWND _hWnd, const SRenderManagerParams& _params)
{

	LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: Inicializando la libreria Direct3D");

	// Create the D3D object.
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	
  SetOk(m_pD3D != NULL);

  if (IsOk())
	{
		// Set up the structure used to create the D3DDevice
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );


		//if(fullscreenMode)
    if(_params.bFullscreen)
		{
			d3dpp.Windowed          = FALSE;
      d3dpp.BackBufferWidth   = _params.v2iResolution.x;
      d3dpp.BackBufferHeight  = _params.v2iResolution.y;
			d3dpp.BackBufferFormat  = D3DFMT_R5G6B5;
		}
		else
		{
			d3dpp.Windowed          = TRUE;
			d3dpp.BackBufferFormat	= D3DFMT_UNKNOWN;
		}

		d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil	= TRUE;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
		d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

		// Create the D3DDevice
		SetOk(!FAILED(	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd,
												D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) ));

		if (!IsOk())
		{
			SetOk(!FAILED(	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd,
													D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) ));

			if (IsOk())
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_SOFTWARE_VERTEXPROCESSING");
			}
		}
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING");
		}


		

		if (IsOk())
		{
			// Turn off culling, so we see the front and back of the triangle
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
			m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


			m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
			m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
			m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

			// Turn off D3D lighting, since we are providing our own vertex colors
			m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		

			//if (fullscreenMode)
      if(_params.bFullscreen)
			{
        m_uWidth	= _params.v2iResolution.x;
        m_uHeight	= _params.v2iResolution.y;
			}
			else
			{
				GetWindowRect(_hWnd);
			}
			LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);
			
		}
	}

	if (!IsOk())
	{
		std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}
	
  STEXTUREDVERTEX::GetVertexDeclaration();
  STEXTURED2VERTEX::GetVertexDeclaration();
  SNORMALTEXTUREDVERTEX::GetVertexDeclaration();
  SNORMALTEXTURED2VERTEX::GetVertexDeclaration();
  SDIFFUSEVERTEX::GetVertexDeclaration();
  SNORMALDIFFUSEVERTEX::GetVertexDeclaration();
  TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexDeclaration();
  TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexDeclaration();
  TCAL3D_HW_VERTEX::GetVertexDeclaration();

#ifdef _DEBUG // Clear the backbuffer to magenta color in a Debug mode
  m_cClearColor = colMAGENTA;
#else // Clear the backbuffer to black color in a Release mode
  m_cClearColor = colBLACK;
#endif

	return IsOk();
}


void CRenderManager::GetWindowRect( HWND hwnd )
{
  assert(IsOk());
	RECT rec_window;
	GetClientRect(	hwnd, &rec_window);
	m_uWidth	= rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}


void CRenderManager::Release(void)
{
  assert(IsOk());
  LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager::Release",m_uWidth,m_uHeight);

  STEXTUREDVERTEX::ReleaseVertexDeclaration();
  STEXTURED2VERTEX::ReleaseVertexDeclaration();
  SNORMALTEXTUREDVERTEX::ReleaseVertexDeclaration();
  SNORMALTEXTURED2VERTEX::ReleaseVertexDeclaration();
  SDIFFUSEVERTEX::ReleaseVertexDeclaration();
  SNORMALDIFFUSEVERTEX::ReleaseVertexDeclaration();
  TNORMALTANGENTBINORMALTEXTUREDVERTEX::ReleaseVertexDeclaration();
  TNORMALTANGENTBINORMALTEXTURED2VERTEX::ReleaseVertexDeclaration();
  TCAL3D_HW_VERTEX::ReleaseVertexDeclaration();

	//Release main devices of render
	CHECKED_RELEASE(m_pD3DDevice)
	CHECKED_RELEASE(m_pD3D)
}


// RENDERING STUFF:
void CRenderManager::BeginRendering ()
{
  assert(IsOk());

	uint32 red		= (uint32) (m_cClearColor.GetRed() * 255);
	uint32 green	= (uint32) (m_cClearColor.GetGreen() * 255);
	uint32 blue		= (uint32) (m_cClearColor.GetBlue() * 255);

	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, red, green, blue), 1.0f, 0 );

	// Begin the scene
	HRESULT hr = m_pD3DDevice->BeginScene();
	assert( SUCCEEDED( hr ) );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	//if(m_bPaintSolid)
	if(true)
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
	}
	else
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
	}
}

void CRenderManager::EndRendering    ()
{
  assert(IsOk());
	m_pD3DDevice->EndScene();
}

void CRenderManager::Present()
{
  // Present the backbuffer contents to the display
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CRenderManager::SetupMatrices(CCamera* _pCamera, bool _bOrtho)
{
  assert(IsOk());
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;
  Vect3f eye;

	if(!_pCamera)
	{
		//Set default view and projection matrix

		//Setup Matrix view
    eye=Vect3f(0.0f,0.0f,-1.0f);
		D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

		//Setup Matrix projection
    if(_bOrtho)
    {
      D3DXMatrixOrthoLH(
                    &m_matProject,
                    (float)m_uWidth,
                    (float)m_uHeight,
                     0.0f,
                     1.0f);
    } else {
		  D3DXMatrixPerspectiveFovLH( 
        &m_matProject, 
        45.0f * D3DX_PI / 180.0f,               //angle de visió
        ((float)m_uWidth)/((float)m_uHeight),   //aspect ratio
        1.0f,                                   //z near
        100.0f                                  //z far
        );
    }
	}
	else
	{
		eye = _pCamera->GetEye();
		D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z);
		Vect3f lookat = _pCamera->GetLookAt();
		D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
		Vect3f vup = _pCamera->GetVecUp();
		D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);
		//Setup Matrix view
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

		//Setup Matrix projection
    if(_bOrtho)
    {
      D3DXMatrixOrthoLH(
                    &m_matProject,
                    (float)m_uWidth,
                    (float)m_uHeight,
                     _pCamera->GetZn(),
                     _pCamera->GetZf());
    } else {
		  D3DXMatrixPerspectiveFovLH(	&m_matProject, _pCamera->GetFov(), _pCamera->GetAspectRatio(),
			  _pCamera->GetZn(), _pCamera->GetZf());
    }
	}


	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
  CORE->GetEffectManager()->ActivateCamera(m_matView, m_matProject, eye);
  /*m_pEffectManager->SetProjectionMatrix(m_matProject);
  m_pEffectManager->SetViewMatrix(m_matView);
  */
}

void CRenderManager::Setup2DCamera()
{
  assert(IsOk());
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;
  Vect3f eye;

	//Set default view and projection matrix
  float l_fCenterW = m_uWidth /2.0f;
  float l_fCenterH = m_uHeight/2.0f;
	//Setup Matrix view
  eye=Vect3f(l_fCenterW,l_fCenterH,-1.0f);
	D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z), l_LookAt(l_fCenterW,l_fCenterH,0.0f), l_VUP(0.0f,1.0f,0.0f);
	D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

	//Setup Matrix projection
  D3DXMatrixOrthoLH(
                &m_matProject,
                (float)m_uWidth,
                (float)m_uHeight,
                  0.0f,
                  2.0f);
    


	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
  CORE->GetEffectManager()->ActivateCamera(m_matView, m_matProject, eye);
  /*m_pEffectManager->SetProjectionMatrix(m_matProject);
  m_pEffectManager->SetViewMatrix(m_matView);
  */
}

void CRenderManager::SetTransform(D3DXMATRIX& matrix)
{
  assert(IsOk());
  assert(!"Method not supported setworldmatrix needs mat44f no d3dxmatrix");
    m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);

    CORE->GetEffectManager()->SetWorldMatrix(matrix);
}

void CRenderManager::SetTransform(Mat44f& m)
{
  assert(IsOk());
  D3DXMATRIX matrix(m.m00, m.m10, m.m20, m.m30,
                    m.m01, m.m11, m.m21, m.m31,
                    m.m02, m.m12, m.m22, m.m32,
                    m.m03, m.m13, m.m23, m.m33);

  m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
    
  CORE->GetEffectManager()->SetWorldMatrix(m);
}

void CRenderManager::EnableAlphaBlend ()
{
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  // render el quad de difuminacion....

  m_pD3DDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA  );
  m_pD3DDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  //// render el quad de difuminacion....
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
}

void CRenderManager::DisableAlphaBlend ()
{
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );    
}

void CRenderManager::CalculateAlignment (uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, Vect2i& _vfinalPos)
{
  assert(IsOk());
  switch (_Alignment)
  {
    case CENTER:
    {
      _vfinalPos.x -= (uint32)(_uiW*0.5f);
      _vfinalPos.y -= (uint32)(_uiH*0.5f);
    }
    break;
    case UPPER_LEFT:
    {
      //Por defecto ya est alienado de esta manera :)
    }
    break;
    case UPPER_RIGHT:
    {
      _vfinalPos.x -= _uiW;
    }
    break;
    case LOWER_RIGHT:
    {
      _vfinalPos.x -= _uiW;
      _vfinalPos.y -= _uiH;
    }
    break;
    case LOWER_LEFT:
    {
      _vfinalPos.y -= _uiH;
    }
    break;
    default:
    {
      LOGGER->AddNewLog(ELL_ERROR, "RenderManager::CalculateAlignment Se está intentado renderizar un quad2d con una alineacion desconocida");
    }
    break;
  }

}


void CRenderManager::DrawLine ( const Vect3f &_PosA, const Vect3f &_PosB, const CColor& _Color)
{
  assert(IsOk());
	DWORD l_color_aux = _Color.GetUint32Argb();

	SDIFFUSEVERTEX v[2] =
	{
		{_PosA.x, _PosA.y, _PosA.z, l_color_aux},
		{_PosB.x, _PosB.y, _PosB.z, l_color_aux},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(SDIFFUSEVERTEX::GetFVF());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(SDIFFUSEVERTEX));
}

void CRenderManager::DrawAxis ()
{
  assert(IsOk());
	DWORD color_red   = colRED.GetUint32Argb();
	DWORD color_green = colGREEN.GetUint32Argb();
	DWORD color_blue  = colBLUE.GetUint32Argb();

	SDIFFUSEVERTEX v[6] =
	{
		{0.f,0.f,0.f, color_red},
		{1.f,0.f,0.f, color_red},

		{0.f,0.f,0.f, color_green},
		{0.f,1.f,0.f, color_green},

		{0.f,0.f,0.f, color_blue},
		{0.f,0.f,1.f, color_blue}
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(SDIFFUSEVERTEX::GetFVF());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,3, v,sizeof(SDIFFUSEVERTEX));
}

void CRenderManager::DrawCube(float _fSize, const CColor& _Color)
{
  assert(IsOk());
  DrawCube(Vect3f(0.0f,0.0f,0.0f),_fSize,_Color);
}

void CRenderManager::DrawCube(Vect3f &_fSize, const CColor& _Color)
{
  assert(IsOk());
  DrawCube(Vect3f(0.0f,0.0f,0.0f),_fSize,_Color);
}

void CRenderManager::DrawCube(const Vect3f &_Pos, float _fSize, const CColor& _Color)
{
  assert(IsOk());
  float l_fC = _fSize/2.f;
  DWORD l_Color   = _Color.GetUint32Argb();

  SDIFFUSEVERTEX v[24] =
	{
		{_Pos.x - l_fC,_Pos.y - l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y - l_fC,_Pos.z - l_fC, l_Color},

		{_Pos.x - l_fC,_Pos.y - l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x - l_fC,_Pos.y + l_fC,_Pos.z - l_fC, l_Color},

		{_Pos.x - l_fC,_Pos.y - l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x - l_fC,_Pos.y - l_fC,_Pos.z + l_fC, l_Color},
    
    //---------------------------------------------------
		{_Pos.x + l_fC,_Pos.y - l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y + l_fC,_Pos.z - l_fC, l_Color},
    
		{_Pos.x + l_fC,_Pos.y - l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y - l_fC,_Pos.z + l_fC, l_Color},
    
    //---------------------------------------------------
		{_Pos.x - l_fC,_Pos.y + l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y + l_fC,_Pos.z - l_fC, l_Color},

		{_Pos.x - l_fC,_Pos.y + l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x - l_fC,_Pos.y + l_fC,_Pos.z + l_fC, l_Color},
    
    //---------------------------------------------------
		{_Pos.x - l_fC,_Pos.y - l_fC,_Pos.z + l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y - l_fC,_Pos.z + l_fC, l_Color},
    
		{_Pos.x - l_fC,_Pos.y - l_fC,_Pos.z + l_fC, l_Color},
		{_Pos.x - l_fC,_Pos.y + l_fC,_Pos.z + l_fC, l_Color},
    
    //---------------------------------------------------
		{_Pos.x + l_fC,_Pos.y + l_fC,_Pos.z - l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y + l_fC,_Pos.z + l_fC, l_Color},
    
    //---------------------------------------------------
		{_Pos.x + l_fC,_Pos.y - l_fC,_Pos.z + l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y + l_fC,_Pos.z + l_fC, l_Color},
    
    //---------------------------------------------------
		{_Pos.x - l_fC,_Pos.y + l_fC,_Pos.z + l_fC, l_Color},
		{_Pos.x + l_fC,_Pos.y + l_fC,_Pos.z + l_fC, l_Color},
	};
  
	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(SDIFFUSEVERTEX::GetFVF());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,12, v,sizeof(SDIFFUSEVERTEX));
}


void CRenderManager::DrawCube(const Vect3f &_Pos, const Vect3f &_Size, const CColor& _Color)
{
  assert(IsOk());
  Vect3f l_fC = _Size/2.f;
  DWORD l_Color   = _Color.GetUint32Argb();

  SDIFFUSEVERTEX v[24] =
	{
		{_Pos.x - l_fC.x,_Pos.y - l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y - l_fC.y,_Pos.z - l_fC.z, l_Color},
                                             
		{_Pos.x - l_fC.x,_Pos.y - l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x - l_fC.x,_Pos.y + l_fC.y,_Pos.z - l_fC.z, l_Color},
                                             
		{_Pos.x - l_fC.x,_Pos.y - l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x - l_fC.x,_Pos.y - l_fC.y,_Pos.z + l_fC.z, l_Color},
                                             
    //----------------------------------------l_fC.z-------
		{_Pos.x + l_fC.x,_Pos.y - l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y + l_fC.y,_Pos.z - l_fC.z, l_Color},
                                              
		{_Pos.x + l_fC.x,_Pos.y - l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y - l_fC.y,_Pos.z + l_fC.z, l_Color},
                                             
    //----------------------------------------l_fC.z-------
		{_Pos.x - l_fC.x,_Pos.y + l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y + l_fC.y,_Pos.z - l_fC.z, l_Color},
                                             
		{_Pos.x - l_fC.x,_Pos.y + l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x - l_fC.x,_Pos.y + l_fC.y,_Pos.z + l_fC.z, l_Color},
                                             
    //----------------------------------------l_fC.z-------
		{_Pos.x - l_fC.x,_Pos.y - l_fC.y,_Pos.z + l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y - l_fC.y,_Pos.z + l_fC.z, l_Color},
                                             
		{_Pos.x - l_fC.x,_Pos.y - l_fC.y,_Pos.z + l_fC.z, l_Color},
		{_Pos.x - l_fC.x,_Pos.y + l_fC.y,_Pos.z + l_fC.z, l_Color},
                                             
    //----------------------------------------l_fC.z-------
		{_Pos.x + l_fC.x,_Pos.y + l_fC.y,_Pos.z - l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y + l_fC.y,_Pos.z + l_fC.z, l_Color},
                                             
    //----------------------------------------l_fC.z-------
		{_Pos.x + l_fC.x,_Pos.y - l_fC.y,_Pos.z + l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y + l_fC.y,_Pos.z + l_fC.z, l_Color},
                                              
    //----------------------------------------l_fC.z-------
		{_Pos.x - l_fC.x,_Pos.y + l_fC.y,_Pos.z + l_fC.z, l_Color},
		{_Pos.x + l_fC.x,_Pos.y + l_fC.y,_Pos.z + l_fC.z, l_Color},
	};
  
	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(SDIFFUSEVERTEX::GetFVF());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,12, v,sizeof(SDIFFUSEVERTEX));
}

void CRenderManager::DrawCamera(CCamera* camera)
{
  assert(IsOk());

	D3DXMATRIX matrix;
	D3DXMATRIX translation;
	if (camera->GetTypeCamera() == CCamera::TC_THPS)
	{
		Vect3f camera_CenterPos = camera->GetObject3D()->GetPosition();
		D3DXMatrixTranslation( &translation, camera_CenterPos.x ,camera_CenterPos.y ,camera_CenterPos.z );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &translation );

		DrawCube(0.5f,colCYAN);
		D3DXMatrixTranslation( &matrix, 0, 0, 0 );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
	}

	//---------PINTAMOS EL FRUSTUM-------------//
	D3DXMATRIX translation2;
	Vect3f eye_aux = camera->GetEye();
	D3DXVECTOR3 eye(eye_aux.x, eye_aux.y, eye_aux.z);
	D3DXMatrixTranslation( &translation, eye.x ,eye.y ,eye.z );

	D3DXMATRIX rotation;
	D3DXMATRIX rotation2;

	float yaw = camera->GetObject3D()->GetYaw();
	float pitch = camera->GetObject3D()->GetPitch();
	D3DXMatrixRotationY ( &rotation,  -yaw);
	D3DXMatrixRotationZ ( &rotation2, +pitch);

	matrix = rotation2 * rotation * translation;

	// Cambiar el sistema de coordenadas
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
	
	SDIFFUSEVERTEX pts[9];

	float fov = camera->GetFov();
	float aspect = camera->GetAspectRatio();
	float d = camera->GetViewD();
	float zNear = camera->GetZn();
	float zFar = camera->GetZf();



	float h_near = zNear * tan ( fov * 0.5f );
	float k_near = h_near * aspect;

	float h_far = d * tan ( fov * 0.5f );
	float k_far = h_far * aspect;

	pts[ 0 ].x = pts[ 0 ].y = pts[ 0 ].z = 0;
	pts[ 0 ].color = 0xffffffff;
  
	pts[ 1 ].x = d;
	pts[ 1 ].y = h_far;
	pts[ 1 ].z = k_far;
	pts[ 1 ].color = 0xffffffff;
	pts[ 2 ].x = d;
	pts[ 2 ].y = h_far;
	pts[ 2 ].z = -k_far;
	pts[ 2 ].color = 0xffffffff;
	pts[ 3 ].x = d;
	pts[ 3 ].y = -h_far;
	pts[ 3 ].z = -k_far;
	pts[ 3 ].color = 0xffffffff;
	pts[ 4 ].x = d;
	pts[ 4 ].y = -h_far;
	pts[ 4 ].z =  k_far;
	pts[ 4 ].color = 0xffffffff;

	pts[ 5 ].x = zNear;
	pts[ 5 ].y = h_near;
	pts[ 5 ].z = k_near;
	pts[ 5 ].color = 0xffffffff;
	pts[ 6 ].x = zNear;
	pts[ 6 ].y = h_near;
	pts[ 6 ].z = -k_near;
	pts[ 6 ].color = 0xffffffff;
	pts[ 7 ].x = zNear;
	pts[ 7 ].y = -h_near;
	pts[ 7 ].z = -k_near;
	pts[ 7 ].color = 0xffffffff;
	pts[ 8 ].x = zNear;
	pts[ 8 ].y = -h_near;
	pts[ 8 ].z = k_near;
	pts[ 8 ].color = 0xffffffff;

	// Decimos el tipo de vertice que vamos a proporcionar...
	m_pD3DDevice->SetFVF( SDIFFUSEVERTEX::GetFVF() );

	// Desactivar la textura
	m_pD3DDevice->SetTexture (0, NULL);

	m_pD3DDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 9, pts, sizeof( SDIFFUSEVERTEX ) );
	static short int indexes[] =  {
		0,1, 0,2, 0,3, 0,4,
		1,2, 2,3, 3,4, 4,1,
		5,6, 6,7, 7,8, 8,5
	};
	m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 9, 12, indexes, D3DFMT_INDEX16, pts, sizeof( SDIFFUSEVERTEX ) );

	D3DXMatrixTranslation( &matrix, 0, 0, 0 );
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );



	//---------PINTAMOS LA BOX Y LOS EJES------------------//
	D3DXMatrixTranslation( &translation2, -1.0f, 0.0f, 0.0f );
	matrix = translation2 * rotation2 * rotation * translation;
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );

	SDIFFUSEVERTEX v[6] =
	{
		//EJE X
		{0, 0, 0, 0xffff0000},
		{3, 0, 0, 0xffff0000},
		//EJE Y
		{0, 0, 0, 0xff00ff00},
		{0, 3, 0, 0xff00ff00},
		//EJE Z
		{0, 0, 0, 0xff0000ff},
		{0, 0, 3, 0xff0000ff},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(SDIFFUSEVERTEX::GetFVF());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, v,sizeof(SDIFFUSEVERTEX));

	DrawCube(0.5f,colWHITE);

	D3DXMatrixTranslation( &matrix, 0, 0, 0 );
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
}

void CRenderManager::DrawGrid(float Size, CColor Color, int GridX, int32 GridZ )
{
  assert(IsOk());
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);		

	if(GridX <= 0)
		GridX = 1;

	if(GridZ <= 0)
		GridZ = 1;

	float l_fSliceX = Size/(float)GridX;
	float l_fSliceZ = Size/(float)GridZ;

	Vect3f l_vTopLeft = Vect3f(-Size/2,0,-Size/2);
	Vect3f l_vTopRight = Vect3f(Size/2,0,-Size/2);
	Vect3f l_vBottomLeft = Vect3f(-Size/2,0,Size/2);

	Vect3f l_vIncX = Vect3f(l_fSliceX,0,0);
	Vect3f l_vIncZ = Vect3f(0,0,l_fSliceZ);

	Vect3f l_vCurrentLeft = l_vTopLeft;
	Vect3f l_vCurrentRight = l_vTopRight;

	int l_iCount = 0;
	for(l_iCount = 0; l_iCount <= GridZ;l_iCount++)
	{
		DrawLine(l_vCurrentLeft,l_vCurrentRight,Color);
		l_vCurrentLeft += l_vIncZ;
		l_vCurrentRight += l_vIncZ;
	}

	l_vCurrentLeft = l_vTopLeft;
	l_vCurrentRight = l_vBottomLeft;

	for(l_iCount = 0; l_iCount <= GridX;l_iCount++)
	{
		DrawLine(l_vCurrentLeft,l_vCurrentRight,Color);
		l_vCurrentLeft += l_vIncX;
		l_vCurrentRight += l_vIncX;
	}

}
void CRenderManager::DrawQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, CColor _Color, ETypeFlip _bFlip)
{
  assert(IsOk());
  DWORD l_Color = _Color.GetUint32Argb(); //si no va, usar: D3DCOLOR_COLORVALUE(_Color.GetRed(), _Color.GetGreen(), _Color.GetBlue(), _Color.GetAlpha())
  Vect2i l_vFinalPos = _vPos;
  CalculateAlignment(_uiW, _uiH, _Alignment, l_vFinalPos);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  unsigned short _usIndices[6]={0,2,1,1,2,3};
  SDIFFUSESCREENVERTEX l_Vtx[4] =
  {
     { (float)l_vFinalPos.x,      (float)l_vFinalPos.y,       0, 1, l_Color} //(x,y) sup_esq.
    ,{ (float)l_vFinalPos.x,      (float)l_vFinalPos.y+_uiH,  0, 1, l_Color} //(x,y) inf_esq.
    ,{ (float)l_vFinalPos.x+_uiW, (float)l_vFinalPos.y,       0, 1, l_Color} //(x,y) sup_dr.
    ,{ (float)l_vFinalPos.x+_uiW, (float)l_vFinalPos.y+_uiH,  0, 1, l_Color} //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SDIFFUSESCREENVERTEX::GetFVF() );
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2, _usIndices, D3DFMT_INDEX16, l_Vtx, sizeof( SDIFFUSESCREENVERTEX ) );
}

void CRenderManager::DrawTexturedQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, CTexture* _pTexture, ETypeFlip _bFlip)
{
  assert(IsOk());
  Vect2i l_vFinalPos = _vPos;
  CalculateAlignment(_uiW, _uiH, _Alignment, l_vFinalPos);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  unsigned short _usIndices[6]={0,2,1,1,2,3};
  STEXTUREDSCREENVERTEX l_Vtx[4] =
  {
     { (float)l_vFinalPos.x      -0.5f, (float)l_vFinalPos.y      -0.5f,  0, 1, 0.0f, 0.0f } //(x,y) sup_esq.
    ,{ (float)l_vFinalPos.x      -0.5f, (float)l_vFinalPos.y+_uiH -0.5f,  0, 1, 0.0f, 1.0f } //(x,y) inf_esq.
    ,{ (float)l_vFinalPos.x+_uiW -0.5f, (float)l_vFinalPos.y      -0.5f,  0, 1, 1.0f, 0.0f } //(x,y) sup_dr.
    ,{ (float)l_vFinalPos.x+_uiW -0.5f, (float)l_vFinalPos.y+_uiH -0.5f,  0, 1, 1.0f, 1.0f } //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( STEXTUREDSCREENVERTEX::GetFVF() );
  if(_pTexture)
    _pTexture->Activate(0);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2, _usIndices, D3DFMT_INDEX16, l_Vtx, sizeof( STEXTUREDSCREENVERTEX ) );
}

void CRenderManager::DrawColoredTexturedQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, CColor _Color, CTexture* _pTexture, ETypeFlip _bFlip)
{
  assert(IsOk());
  DWORD l_Color = _Color.GetUint32Argb(); //si no va, usar: D3DCOLOR_COLORVALUE(_Color.GetRed(), _Color.GetGreen(), _Color.GetBlue(), _Color.GetAlpha())
  Vect2i l_vFinalPos = _vPos;
  CalculateAlignment(_uiW, _uiH, _Alignment, l_vFinalPos);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  unsigned short _usIndices[6]={0,2,1,1,2,3};
  SDIFFUSETEXTUREDSCREENVERTEX l_Vtx[4] =
  {
     { (float)l_vFinalPos.x      -0.5f, (float)l_vFinalPos.y      -0.5f,  0, 1, l_Color, 0.0f, 0.0f } //(x,y) sup_esq.
    ,{ (float)l_vFinalPos.x      -0.5f, (float)l_vFinalPos.y+_uiH -0.5f,  0, 1, l_Color, 0.0f, 1.0f } //(x,y) inf_esq.
    ,{ (float)l_vFinalPos.x+_uiW -0.5f, (float)l_vFinalPos.y      -0.5f,  0, 1, l_Color, 1.0f, 0.0f } //(x,y) sup_dr.
    ,{ (float)l_vFinalPos.x+_uiW -0.5f, (float)l_vFinalPos.y+_uiH -0.5f,  0, 1, l_Color, 1.0f, 1.0f } //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SDIFFUSETEXTUREDSCREENVERTEX::GetFVF() );
  if(_pTexture)
    _pTexture->Activate(0);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2, _usIndices, D3DFMT_INDEX16, l_Vtx, sizeof( SDIFFUSETEXTUREDSCREENVERTEX ) );
}

void CRenderManager::GetRay (const Vect2i& mousePos, Vect3f& posRay, Vect3f& dirRay)
{
  assert(IsOk());
  D3DXMATRIX projectionMatrix, viewMatrix, worldViewInverse, worldMatrix;

  m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);
  m_pD3DDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
  m_pD3DDevice->GetTransform(D3DTS_WORLD, &worldMatrix);

  float angle_x = (((2.0f * mousePos.x) / m_uWidth) - 1.0f) / projectionMatrix(0,0);
  float angle_y = (((-2.0f * mousePos.y) / m_uHeight) + 1.0f) / projectionMatrix(1,1);

  D3DXVECTOR3 ray_org = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 ray_dir = D3DXVECTOR3(angle_x, angle_y, 1.0f);

  D3DXMATRIX m = worldMatrix * viewMatrix;
  D3DXMatrixInverse(&worldViewInverse, 0, &m);
  D3DXVec3TransformCoord(&ray_org, &ray_org, &worldViewInverse);
  D3DXVec3TransformNormal(&ray_dir, &ray_dir, &worldViewInverse);
  D3DXVec3Normalize(&ray_dir, &ray_dir);

  posRay.x = ray_org.x;
  posRay.y = ray_org.y;
  posRay.z = ray_org.z;

  dirRay.x = ray_dir.x;
  dirRay.y = ray_dir.y;
  dirRay.z = ray_dir.z;
}

void CRenderManager::DrawRectangle2D (const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor, uint32 edge_w, uint32 edge_h, CColor& edgeColor)
{
  assert(IsOk());
  //Draw background quad2D:
  DrawQuad2D(pos, w, h, UPPER_LEFT, backGroundColor);
   
  //Draw the four edges:

  //2 Horizontal:
  Vect2i pos_aux = pos;
  pos_aux.y -= edge_h;
  DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
  pos_aux = pos;
  pos_aux.y += h;
  DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
  
  //2 Vertical:
  pos_aux = pos;
  pos_aux.x -= edge_w;
  pos_aux.y -= edge_h;
  DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);
  pos_aux.x = pos.x + w;
  DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);    
}

void CRenderManager::RenderBoundingBox(CBoundingBox* _pBBox)
{
  assert(IsOk());
  const Vect3f *l_pVectBox = _pBBox->GetBox();

  //DrawLine(l_pVectBox[0],_pBBox->GetMiddlePoint(),colGREEN);

    /*            4                    5
               _____________________
              /|                  /|
             / |                 / |
            /  |                /  |
           /   |               /   |
          /    |              /    |
         /     |             /     |
      6 /___________________/ 7    |
        |      | 0          |      | 1
        |      |___________ |______|
        |     /             |     / 
        |    /              |    /  
        |   /               |   /   
        |  /                |  /    
        | /                 | /     
      2 |/__________________|/ 3 
  */

  DrawLine(l_pVectBox[0],l_pVectBox[1],colBLUE);
  DrawLine(l_pVectBox[0],l_pVectBox[2],colBLUE);
  DrawLine(l_pVectBox[0],l_pVectBox[4],colBLUE);
  DrawLine(l_pVectBox[3],l_pVectBox[1],colBLUE);
  DrawLine(l_pVectBox[3],l_pVectBox[2],colBLUE);
  DrawLine(l_pVectBox[3],l_pVectBox[7],colBLUE);
  DrawLine(l_pVectBox[4],l_pVectBox[5],colBLUE);
  DrawLine(l_pVectBox[4],l_pVectBox[6],colBLUE);
  DrawLine(l_pVectBox[5],l_pVectBox[1],colBLUE);
  DrawLine(l_pVectBox[5],l_pVectBox[7],colBLUE);
  DrawLine(l_pVectBox[6],l_pVectBox[2],colBLUE);
  DrawLine(l_pVectBox[6],l_pVectBox[7],colBLUE);
}

void CRenderManager::RenderBoundingSphere(CBoundingSphere* _pBSphere)
{
  assert(IsOk());

  //Traslladar al centre de l'esfera
  D3DMATRIX l_md3d;
  m_pD3DDevice->GetTransform(D3DTS_WORLD,&l_md3d);
  Mat44f l_mT = l_md3d;
  Vect3f l_vMid = _pBSphere->GetMiddlePoint();
  l_vMid += l_mT.GetTranslationVector();
  l_mT.Translate(l_vMid);

  SetTransform(l_mT);
  
  DrawSphere(_pBSphere->GetRadius(),colBLUE, 10);
  //DrawLine(_pBSphere->GetMiddlePoint(),Vect3f(0.0f),colGREEN);
  //DrawLine(Vect3f(0.0f),Vect3f(0.0f,0.1f,0.0f),colRED);
}


//void CRenderManager::DrawSphere (float Radius, CColor Color, uint32 Aristas, ETypeModePaint mode, EtypeSphere typeSphere)
//{
//   float l_fAngle    = 180.f;
//   float l_fAngle2    = 180.f;
//   
//   for(uint32 t=0;t<Aristas;++t)
//   {
//       float l_fLambda        = ((float)t)/((float)Aristas);
//       float l_fLambda2    = ((float)(t+1))/((float)Aristas);
//       switch (typeSphere)
//       {
//       case  HALF_TOP:
//           l_fAngle    = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 90.f, l_fLambda));
//           l_fAngle2    = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 90.f, l_fLambda2));
//           break;
//       case HALF_BOTTOM:
//           l_fAngle    = mathUtils::Deg2Rad(mathUtils::Lerp(90.f, 180.f, l_fLambda));
//           l_fAngle2    = mathUtils::Deg2Rad(mathUtils::Lerp(90.f, 180.f, l_fLambda2));
//           break;
//       case COMPLETE:
//           l_fAngle    = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 180.f, l_fLambda));
//           l_fAngle2    = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 180.f, l_fLambda2));
//           break;
//       }
//       
//       std::vector<Vect3f> l_ring;
//       float l_RadiusRing=Radius*sin(l_fAngle);
//       Vect3f l_PosAux = v3fZERO;
//       Vect3f l_PosAux2, l_PosAux3;
//       for(uint32 b=0;b<Aristas;++b)
//       {
//           Vect3f l_PosA(    l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
//                                           Radius*cos(l_fAngle),
//                                           l_RadiusRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))));
//
//           Vect3f l_PosB(    l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)(b+1))/((float)Aristas))),
//                                           Radius*cos(l_fAngle),
//                                           l_RadiusRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)(b+1))/((float)Aristas))));
//
//           
//
//           float l_RadiusNextRing=Radius*sin(l_fAngle2);
//
//           Vect3f l_PosC(    l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
//                                           Radius*cos(l_fAngle),
//                                           l_RadiusRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))));
//
//           Vect3f l_PosD(    l_RadiusNextRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
//                                           Radius*cos(l_fAngle2),
//                                           l_RadiusNextRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))));
//
//           if (mode != PAINT_WIREFRAME)
//           {
//               DrawTriangle3D(l_PosA, l_PosD, l_PosB, Color);
//               if(l_PosAux !=  v3fZERO )
//               {
//                   DrawTriangle3D(l_PosA, l_PosAux, l_PosD,Color);
//               }
//               else
//               {
//                   l_PosAux2 = l_PosA;
//                   l_PosAux3    = l_PosD;
//               }
//
//               if(b == Aristas-1)
//               {
//                   DrawTriangle3D(l_PosAux2, l_PosD, l_PosAux3,Color);
//               }
//               l_PosAux = l_PosD;
//           }
//           
//           if( mode != PAINT_SOLID)
//           {
//               CColor color_aux = Color;
//               if (mode == PAINT_BOTH)
//               {
//                   color_aux = colWHITE;
//               }
//               DrawLine(l_PosA,l_PosB,color_aux);
//               DrawLine(l_PosC,l_PosD,color_aux);
//           }
//       }
//   }
//}

void CRenderManager::DrawPlane(float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ )
{
  assert(IsOk());
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);

	//Ax + By + Cz + D = 0
	//if (x,y) = (0,0) then z = -D/C
	//if (x,y) = (1,1) then z = (-D-A-B)/C
	//With two points we have a vector-->

	float A,B,C,D;
	A = normal.x; 
	B = normal.y; 
	C = normal.z; 
	D = distance; 
	//float pointAz;
	//float pointBz;

	Vect3f pointA, pointB;
	if( C!= 0)
	{
		pointA = Vect3f(0.f,0.f, -D/C);
		pointB = Vect3f(1.f,1.f, (D-A-B)/C);
	}
	else if( B!= 0)
	{
		pointA = Vect3f(0.f,-D/B, 0.f);
		pointB = Vect3f(1.f,(-D-A-C)/B,1.f);
	}
	else if( A != 0)
	{
		pointA = Vect3f(-D/A,0.f, 0.f);
		pointB = Vect3f((-D-B-C)/A,1.f,1.f);
	}
	else
	{
		//error.
	}


	Vect3f vectorA = pointB - pointA;
  vectorA.Normalize();
	Vect3f vectorB;
  vectorB = normal^vectorA;
  vectorB.Normalize();
	Vect3f initPoint = normal*distance;

	assert(GridX>0);
	assert(GridZ>0);
	//LINEAS EN Z
	Vect3f initPointA = initPoint - vectorB*size*0.5;
	for(int b=0;b<=GridX;++b)
	{		
		DrawLine(initPointA + vectorA*size*0.5, initPointA - vectorA*size*0.5, Color );

		initPointA += vectorB*size/(float)GridX;
	}
	initPointA = initPoint - vectorA*size*0.5;
	for(int b=0;b<=GridX;++b)
	{
		DrawLine(initPointA + vectorB*size*0.5, initPointA - vectorB*size*0.5, Color);
		initPointA += vectorA*size/(float)GridX;
	}
}


void CRenderManager::DrawSphere( float Radius, const CColor& Color, int Aristas)
{
  assert(IsOk());
  for(int t=0;t<Aristas;++t)
  {
    float l_RadiusRing=Radius*sin(mathUtils::Deg2Rad(180.0f*((float)t))/((float)Aristas));
    for(int b=0;b<Aristas;++b)
    {
      Vect3f l_PosA(  l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
                      Radius*cos(mathUtils::Deg2Rad((180.0f*((float)t))/((float)Aristas))),
                      l_RadiusRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)b)/((float)Aristas)))));

      Vect3f l_PosB(  l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)(b+1))/((float)Aristas))),
                      Radius*cos(mathUtils::Deg2Rad((180.0f*((float)t))/((float)Aristas))),
                      l_RadiusRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)(b+1))/((float)Aristas)))));

      DrawLine(l_PosA,l_PosB,Color);

      float l_RadiusNextRing=Radius*sin(mathUtils::Deg2Rad(180.0f*((float)(t+1)))/((float)Aristas));

      Vect3f l_PosC(  l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
                      Radius*cos(mathUtils::Deg2Rad((180.0f*((float)t))/((float)Aristas))),
                      l_RadiusRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)b)/((float)Aristas)))));
      Vect3f l_PosD(  l_RadiusNextRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
                      Radius*cos(mathUtils::Deg2Rad((180.0f*((float)(t+1)))/((float)Aristas))),
                      l_RadiusNextRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)b)/((float)Aristas)))));

      DrawLine(l_PosC,l_PosD,Color);
    }
  }
}

