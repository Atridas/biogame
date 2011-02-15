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
class CEffectManager;
class CTexture;
//-------------------------------------------------------------------

typedef enum eTypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };



typedef enum ETypeFlip { NONE_FLIP, FLIP_X, FLIP_Y};


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
  void                  EnableAlphaBlend  ();
  void                  DisableAlphaBlend ();


  //----Matrix Functions-----------------------------------------------
  void                  Setup2DCamera     ();
  void                  SetupMatrices     (CCamera* _pCamera, bool m_bOrtho = false);
  void                  SetTransform      (D3DXMATRIX& matrix);
  void                  SetTransform      (Mat44f& m);
  //-------------------------------------------------------------------

  //----Utility Functions----------------------------------------------
  void GetRay (const Vect2i& mousePos, Vect3f& posRay, Vect3f& dirRay);
  //-------------------------------------------------------------------

  //----Render Functions ----------------------------------------------
  void CalculateAlignment (uint32 _uiW, uint32 _uiH, eTypeAlignment _Alignment, Vect2i& _vfinalPos);
  void DrawQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, eTypeAlignment _Alignment, CColor _Color);
  void DrawTexturedQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, eTypeAlignment _Alignment, CTexture* _pTexture = 0);
  void DrawColoredTexturedQuad2D(const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, eTypeAlignment _Alignment, CColor _Color, CTexture* _pTexture = 0);
  void DrawRectangle2D(const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,  uint32 edge_w, uint32 edge_h, CColor& edgeColor);
  //-------------------------------------------------------------------

	//----DebugRender Functions-------------------------------------------
	void								  DrawLine					(const Vect3f &_PosA, const Vect3f &_PosB, const CColor& _Color);
  void                  DrawAxis          ();
  void                  DrawCube          (const Vect3f &_Pos, float _fSize, const CColor& _Color);
  void                  DrawCube          (float _fSize, const CColor& _Color);
  void                  DrawCamera        (CCamera* camera);
 	void									DrawGrid				  (float Size, CColor Color=colWHITE, int GridX=10, int32 GridZ=10 );
  //void                  EnableAlphaBlend  ();
  //void                  DisbaleAlphaBlend ();

	//--------------------------------------------------------------------

  //----Getters / Setters ----------------------------------------------
  uint32                  GetScreenWidth          () const {return m_uWidth;};
  uint32                  GetScreenHeight         () const {return m_uHeight;};

  LPDIRECT3DDEVICE9       GetDevice               () const {return m_pD3DDevice;};
  CCamera*                GetCamera               () const {return m_pCamera;};
  CTextureManager*        GetTextureManager       () const {return m_pTextureManager;};
  CStaticMeshManager*     GetStaticMeshManager    () const {return m_pStaticMeshManager;};
  CAnimatedModelManager*  GetAnimatedModelManager () const {return m_pAnimatedModelManager;};
  CEffectManager*         GetEffectManager        () const {return m_pEffectManager;};

  

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
  CEffectManager*         m_pEffectManager;
};
