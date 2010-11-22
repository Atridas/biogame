#include "Utils/Logger.h"
#include "Utils/Exception.h"
#include "RenderManager.h"
#include "params.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};


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
      d3dpp.BackBufferWidth   = _params.uiWidth;
      d3dpp.BackBufferHeight  = _params.uiHeight;
			d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
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
        m_uWidth	= _params.uiWidth;
        m_uHeight	= _params.uiHeight;
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
	
	return IsOk();
}


void CRenderManager::GetWindowRect( HWND hwnd )
{
	RECT rec_window;
	GetClientRect(	hwnd, &rec_window);
	m_uWidth	= rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}


void CRenderManager::Release(void)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager::Release",m_uWidth,m_uHeight);
	//Release main devices of render
	CHECKED_RELEASE(m_pD3DDevice);
	CHECKED_RELEASE(m_pD3D);
}


// RENDERING STUFF:
void CRenderManager::BeginRendering ()
{

#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
	uint32 red		= (uint32) (1.f * 255);
	uint32 green	= (uint32) (0.f * 255);
	uint32 blue		= (uint32) (1.f * 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
	uint32 red		= (uint32) (0.f * 255);
	uint32 green	= (uint32) (0.f * 255);
	uint32 blue		= (uint32) (0.f * 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#endif


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
	m_pD3DDevice->EndScene();
	// Present the backbuffer contents to the display
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CRenderManager::SetupMatrices   ()
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;

	//if(!camera)
	if(true)
	{
		//Set default view and projection matrix

		//Setup Matrix view
		D3DXVECTOR3 l_Eye(5.0f,2.0f,-1.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

		//Setup Matrix projection
		D3DXMatrixPerspectiveFovLH( 
      &m_matProject, 
      45.0f * D3DX_PI / 180.0f,               //angle de visió
      ((float)m_uWidth)/((float)m_uHeight),   //aspect ratio
      1.0f,                                   //z near
      100.0f                                  //z far
      );
	}
	else
	{
		//Vect3f eye = camera->GetEye();
		//D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z);
		//Vect3f lookat = camera->GetLookAt();
		//D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
		//Vect3f vup = camera->GetVecUp();
		//D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);
		////Setup Matrix view
		//D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

		////Setup Matrix projection
		//D3DXMatrixPerspectiveFovLH(	&m_matProject, camera->GetFov(), camera->GetAspectRatio(),
		//	camera->GetZn(), camera->GetZf());
	}


	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
}

void CRenderManager::SetTransform(D3DXMATRIX& matrix)
{
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
}

void CRenderManager::SetTransform(Mat44f& m)
{
  D3DXMATRIX aux(    m.m00, m.m10, m.m20, m.m30    , m.m01, m.m11, m.m21, m.m31
                   , m.m02, m.m12, m.m22, m.m32    , m.m03, m.m13, m.m23, m.m33);
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &aux);
}


// Draw functions

void CRenderManager::DrawLine ( const Vect3f &_PosA, const Vect3f &_PosB, const CColor& _Color)
{
	DWORD l_color_aux = _Color.GetUint32Argb();

	CUSTOMVERTEX v[2] =
	{
		{_PosA.x, _PosA.y, _PosA.z, l_color_aux},
		{_PosB.x, _PosB.y, _PosB.z, l_color_aux},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}

void CRenderManager::DrawAxis ()
{
	DWORD color_red   = colRED.GetUint32Argb();
	DWORD color_green = colGREEN.GetUint32Argb();
	DWORD color_blue  = colBLUE.GetUint32Argb();

	CUSTOMVERTEX v[6] =
	{
		{0.f,0.f,0.f, color_red},
		{1.f,0.f,0.f, color_red},

		{0.f,0.f,0.f, color_green},
		{0.f,1.f,0.f, color_green},

		{0.f,0.f,0.f, color_blue},
		{0.f,0.f,1.f, color_blue}
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,3, v,sizeof(CUSTOMVERTEX));
}


void CRenderManager::DrawCube(const Vect3f &_Pos, float _fSize, const CColor& _Color)
{
  float l_fC = _fSize/2.f;
  DWORD l_Color   = _Color.GetUint32Argb();

  CUSTOMVERTEX v[24] =
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
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,12, v,sizeof(CUSTOMVERTEX));
}


void CRenderManager::DrawCube(float _fSize, const CColor& _Color)
{
  float l_fC = _fSize/2.f;
  DWORD l_Color   = _Color.GetUint32Argb();

  CUSTOMVERTEX v[24] =
	{
		{- l_fC, - l_fC, - l_fC, l_Color},
		{+ l_fC, - l_fC, - l_fC, l_Color},

		{- l_fC, - l_fC, - l_fC, l_Color},
		{- l_fC, + l_fC, - l_fC, l_Color},

		{- l_fC, - l_fC, - l_fC, l_Color},
		{- l_fC, - l_fC, + l_fC, l_Color},
    
    //---------------------------------------------------
		{+ l_fC, - l_fC, - l_fC, l_Color},
		{+ l_fC, + l_fC, - l_fC, l_Color},
    
		{+ l_fC, - l_fC, - l_fC, l_Color},
		{+ l_fC, - l_fC, + l_fC, l_Color},
    
    //---------------------------------------------------
		{- l_fC, + l_fC, - l_fC, l_Color},
		{+ l_fC, + l_fC, - l_fC, l_Color},

		{- l_fC, + l_fC, - l_fC, l_Color},
		{- l_fC, + l_fC, + l_fC, l_Color},
    
    //---------------------------------------------------
		{- l_fC, - l_fC, + l_fC, l_Color},
		{+ l_fC, - l_fC, + l_fC, l_Color},
    
		{- l_fC, - l_fC, + l_fC, l_Color},
		{- l_fC, + l_fC, + l_fC, l_Color},
    
    //---------------------------------------------------
		{+ l_fC, + l_fC, - l_fC, l_Color},
		{+ l_fC, + l_fC, + l_fC, l_Color},
    
    //---------------------------------------------------
		{+ l_fC, - l_fC, + l_fC, l_Color},
		{+ l_fC, + l_fC, + l_fC, l_Color},
    
    //---------------------------------------------------
		{- l_fC, + l_fC, + l_fC, l_Color},
		{+ l_fC, + l_fC, + l_fC, l_Color},
	};
  
	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,12, v,sizeof(CUSTOMVERTEX));
}