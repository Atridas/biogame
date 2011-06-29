#pragma once

#include <d3dx9.h>
#include <string>
#include "base.h"
#include "Math/Color.h"
#include "Frustum.h"

//forward declarations ----------------------------------------------
struct SRenderManagerParams;
class CCamera;
class CTexture;
class CBoundingBox;
class CBoundingSphere;
//-------------------------------------------------------------------

typedef enum ETypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };



typedef enum ETypeFlip { NONE_FLIP, FLIP_X, FLIP_Y};


class CRenderManager:
  public CBaseControl
{
 
public:
	                      CRenderManager    (void):	m_pD3D(0), m_pD3DDevice(0),
												                          m_uWidth(0), m_uHeight(0),
                                                  m_pCamera(0)
                                                                                  {};


  virtual               ~CRenderManager   (void)                                  {Done();};

  bool                  Init              (HWND hWnd, const SRenderManagerParams& _params);

  void                  BeginRendering    ();
  void                  EndRendering      ();
  void                  Present           ();
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
  void CalculateAlignment (uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, Vect2i& _vfinalPos);

  //TODO FLIP!!!!!!!!!!!!
  void DrawQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, CColor _Color, ETypeFlip _bFlip = NONE_FLIP);
  void DrawTexturedQuad2D (const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, CTexture* _pTexture = 0, ETypeFlip _bFlip = NONE_FLIP);
  void DrawColoredTexturedQuad2D(const Vect2i& _vPos, uint32 _uiW, uint32 _uiH, ETypeAlignment _Alignment, CColor _Color, CTexture* _pTexture = 0, ETypeFlip _bFlip = NONE_FLIP);
  void DrawRectangle2D(const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,  uint32 edge_w, uint32 edge_h, CColor& edgeColor);

  //----QuadRender Functions----------------------------------------------
   //void    DrawQuad2D     (const Vect2i& pos, uint32 w, uint32 h, eTypeAlignment alignment, CTexture* texture, SRectangle2D& coordText);
   //void    DrawQuad2D     (const Vect2i& pos, uint32 w, uint32 h, eTypeAlignment alignment, CTexture* texture, eTypeFlip flip = NONE_FLIP);    
  //-------------------------------------------------------------------

	//----DebugRender Functions-------------------------------------------
	void								  DrawLine					   (const Vect3f &_PosA, const Vect3f &_PosB, const CColor& _Color);
  void                  DrawAxis             ();
  void                  DrawCube             (const Vect3f &_Pos, float _fSize, const CColor& _Color);
  void                  DrawCube             (const Vect3f &_Pos, const Vect3f &_Size, const CColor& _Color);
  void                  DrawCube             (float _fSize, const CColor& _Color);
  void                  DrawCube             (Vect3f &_fSize, const CColor& _Color);
  void                  DrawCamera           (CCamera* camera);
  void                  DrawFrustum          (CFrustum* frustum, const CColor& _Color);
 	void									DrawGrid				     (float Size, CColor Color=colWHITE, int GridX=10, int32 GridZ=10 );
  void                  RenderBoundingBox    (CBoundingBox* _pBBox);
  void                  RenderBoundingSphere (CBoundingSphere* _pBSphere);
  void                  DrawPlane         (float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ ); 
  void                  DrawSphere        (float Radius, const CColor& Color, int Aristas);
  //void                  DrawSphere        (float Radius, CColor Color=colWHITE, uint32 Aristas=10,  ETypeModePaint mode = PAINT_WIREFRAME,  EtypeSphere typeSphere = COMPLETE);
  //void                  EnableAlphaBlend  ();
  //void                  DisbaleAlphaBlend ();

	//--------------------------------------------------------------------

  //----Getters / Setters ----------------------------------------------
  uint32                  GetScreenWidth          () const {return m_uWidth;};
  uint32                  GetScreenHeight         () const {return m_uHeight;};

  LPDIRECT3DDEVICE9       GetDevice               () const {return m_pD3DDevice;};
  CCamera*                GetCamera               () const {return m_pCamera;};
  const CFrustum&         GetFrustum              () const {return m_Frustum;};
  
  void                    SetClearColor           (const CColor& _cColor) {m_cClearColor = _cColor;};
  CColor                  GetClearColor           () const                {return m_cClearColor;};

private:
	virtual void            Release					  ();

	void									  GetWindowRect		  (HWND hWnd);

  CColor                  m_cClearColor;
	LPDIRECT3D9						  m_pD3D; // direct3d interface
	LPDIRECT3DDEVICE9	  	  m_pD3DDevice;	   					 // direct3d device

	uint32								  m_uWidth;
	uint32								  m_uHeight;

  CCamera*                m_pCamera;
  CFrustum                m_Frustum;

};
