#include "RenderManager.h"

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


bool CRenderManager::Init(HWND hWnd)
{

	//LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: Inicializando la libreria Direct3D");

	// Create the D3D object.
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	
	m_bIsOk = m_pD3D != NULL;

	if (m_bIsOk)
	{
		// Set up the structure used to create the D3DDevice
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );


		//if(fullscreenMode)
		if(false)
		{
			d3dpp.Windowed          = FALSE;
			/*d3dpp.BackBufferWidth   = widthScreen;
			d3dpp.BackBufferHeight  = heightScreen;*/
			d3dpp.BackBufferWidth   = 800;
			d3dpp.BackBufferHeight  = 600;
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
		m_bIsOk = !FAILED(	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
												D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

		if (!m_bIsOk)
		{
			m_bIsOk = !FAILED(	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
													D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

			if (m_bIsOk)
			{
				//LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_SOFTWARE_VERTEXPROCESSING");
			}
		}
		else
		{
			//LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING");
		}


		

		if (m_bIsOk)
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
			if(false)
			{
				m_uWidth	= 800;
				m_uHeight	= 600;
			}
			else
			{
				GetWindowRect(hWnd);
			}
			//LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);
			
		}
	}

	if (!m_bIsOk)
	{
		std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
		//LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Relase();
		//throw CException(__FILE__, __LINE__, msg_error);
	}

	
	return m_bIsOk;
}


void CRenderManager::GetWindowRect( HWND hwnd )
{
	RECT rec_window;
	GetClientRect(	hwnd, &rec_window);
	m_uWidth	= rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}


void CRenderManager::Relase(void)
{
	//Relase main devices of render
	CHECKED_RELEASE(m_pD3DDevice);
	CHECKED_RELEASE(m_pD3D);
}


// RENDERING STUFF:
void CRenderManager::BeginRendering ()
{

#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
	uint32 red		= (uint32) (0.f * 255);
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
		D3DXVECTOR3 l_Eye(5.0f,5.0f,-5.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
		D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

		//Setup Matrix projection
		D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, 1.0f, 1.0f, 100.0f );
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

void CRenderManager::DrawLine ( const Vect3f &PosA, const Vect3f &PosB, CColor Color)
{
	DWORD color_aux = Color.GetUint32Argb();

	CUSTOMVERTEX v[2] =
	{
		{PosA.x, PosA.y, PosA.z, color_aux},
		{PosB.x, PosB.y, PosB.z, color_aux},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}

void CRenderManager::DrawAxis ()
{
  //TODO açó és una guarrada, caldrà arreglar-ho
  CColor red(1.f,0.f,0.f);
  CColor green(0.f,1.f,0.f);
  CColor blue(1.f,0.f,1.f);

	DWORD color_red   = red  .GetUint32Argb();
	DWORD color_green = green.GetUint32Argb();
	DWORD color_blue  = blue .GetUint32Argb();

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