#pragma once

#include <d3dx9.h>
#include <string>
#include "Base.h"
#include "Math/Color.h"

//forward declarations ----------------------------------------------
struct SRenderManagerParams;
//-------------------------------------------------------------------

class CRenderManager:
  public CBaseControl
{
 
public:
	                      CRenderManager    (void):	m_pD3D(0), m_pD3DDevice(0),
												                          m_uWidth(0), m_uHeight(0)       {};
  virtual               ~CRenderManager   (void)                                  {Done();};

  bool                  Init(HWND hWnd, const SRenderManagerParams& _params);

  void                  BeginRendering    ();
  void                  EndRendering      ();
  void                  SetupMatrices     ();
	
	//----DebugRender Functions-------------------------------------------
	void								  DrawLine					(const Vect3f &_PosA, const Vect3f &_PosB, const CColor& _Color);
  void                  DrawAxis          ();
  void                  DrawCube          (const Vect3f &_Pos, float _fSize, const CColor& _Color);
	//--------------------------------------------------------------------

  //----Getters / Setters ----------------------------------------------
  LPDIRECT3DDEVICE9     GetDevice         () {return m_pD3DDevice;};

public:
	void                  Release					  ();

private:
	void									GetWindowRect		  (HWND hWnd);

	LPDIRECT3D9						m_pD3D; // direct3d interface
	LPDIRECT3DDEVICE9	  	m_pD3DDevice;	   					 // direct3d device

	uint32								m_uWidth;
	uint32								m_uHeight;
};

