#pragma once

#include <d3dx9.h>
#include <string>
#include "Base.h"
#include "Math/Color.h"

//forward declarations ----------------------------------------------
struct SRenderManagerParams;
class CCamera;
class CTextureManager;
class CStaticMeshManager;
class CAnimatedModelManager;
//-------------------------------------------------------------------

class CRenderManager:
  public CBaseControl
{
 
public:
	                      CRenderManager    (void):	m_pD3D(0), m_pD3DDevice(0),
												                          m_uWidth(0), m_uHeight(0),
                                                  m_pCamera(0), m_pTextureManager(0),
                                                  m_pStaticMeshManager(0),m_pAnimatedModelManager(0)
                                                                                  {};


  virtual               ~CRenderManager   (void)                                  {Done();};

  bool                  Init              (HWND hWnd, const SRenderManagerParams& _params);

  void                  BeginRendering    ();
  void                  EndRendering      ();

  //----Matrix Functions-----------------------------------------------
  void                  SetupMatrices     (CCamera* _pCamera);
  void                  SetTransform      (D3DXMATRIX& matrix);
  void                  SetTransform      (Mat44f& m);
  //-------------------------------------------------------------------

	//----DebugRender Functions-------------------------------------------
	void								  DrawLine					(const Vect3f &_PosA, const Vect3f &_PosB, const CColor& _Color);
  void                  DrawAxis          ();
  void                  DrawCube          (const Vect3f &_Pos, float _fSize, const CColor& _Color);
  void                  DrawCube          (float _fSize, const CColor& _Color);
  void                  DrawCamera        (CCamera* camera);
 	void									DrawGrid				  (float Size, CColor Color=colWHITE, int GridX=10, int32 GridZ=10 );

	//--------------------------------------------------------------------

  //----Getters / Setters ----------------------------------------------
  uint32                  GetScreenWidth          () const {return m_uWidth;};
  uint32                  GetScreenHeight         () const {return m_uHeight;};

  LPDIRECT3DDEVICE9       GetDevice               () const {return m_pD3DDevice;};
  CTextureManager*        GetTextureManager       () const {return m_pTextureManager;};
  CStaticMeshManager*     GetStaticMeshManager    () const {return m_pStaticMeshManager;};
  CAnimatedModelManager*  GetAnimatedModelManager () const {return m_pAnimatedModelManager;};

public:
	void                    Release					  ();

private:
	void									  GetWindowRect		  (HWND hWnd);

	LPDIRECT3D9						  m_pD3D; // direct3d interface
	LPDIRECT3DDEVICE9	  	  m_pD3DDevice;	   					 // direct3d device

	uint32								  m_uWidth;
	uint32								  m_uHeight;

  CCamera*                m_pCamera;
  CTextureManager*        m_pTextureManager;
  CStaticMeshManager*     m_pStaticMeshManager;
  CAnimatedModelManager*  m_pAnimatedModelManager;
};

