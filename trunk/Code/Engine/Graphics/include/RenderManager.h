#pragma once

#include <d3dx9.h>
#include <string>
#include "Base.h"
#include "Math/Color.h"

class CRenderManager
{
 
public:
	CRenderManager(void):	m_bIsOk(false), m_pD3D(0), m_pD3DDevice(0),
												m_uWidth(0), m_uHeight(0) {};

  virtual ~CRenderManager(void) {Done();};

  bool                  Init(HWND hWnd);

  void                  Done            ()        {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk            () const  {return m_bIsOk;};

  void                  BeginRendering  ();
  void                  EndRendering    ();
  void                  SetupMatrices   ();
	
	//----DebugRender Functions-------------------------------------------
	void								  DrawLine					(const Vect3f &PosA, const Vect3f &PosB, CColor Color);
  void                  DrawAxis          ();
	//--------------------------------------------------------------------

private:
	void                  Relase					();
	void									GetWindowRect		(HWND hWnd);

private:
  bool                  m_bIsOk;
	LPDIRECT3D9						m_pD3D; // direct3d interface
	LPDIRECT3DDEVICE9	  	m_pD3DDevice;	   					 // direct3d device

	uint32								m_uWidth;
	uint32								m_uHeight;
};

