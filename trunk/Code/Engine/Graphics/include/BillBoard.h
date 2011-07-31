#pragma once

#include <d3dx9.h>
#include "vector"
#include "Camera.h"
#include "Texture.h"

class CRenderManager;

class CBillBoard
{
public:

	CBillBoard();
  ~CBillBoard(){};

	void Init         (CRenderManager* rm);
  void Update	      (float fTimeDelta, CCamera *camera );
	void Render	      (CRenderManager* _pRM);
  void Release      ();
	


  //  Set
  
  void SetId                (string _szId)                          {m_szId = _szId;};
  void SetPos		            ( const Vect3f & position )             {m_vPosition = position;}
	void SetSizeX	            ( const float &sizeX)                   {m_fSizeX = sizeX;}
  void SetSizeY	            ( const float &sizeY)                   {m_fSizeY = sizeY;}
  void SetTexture           (CTexture* _pTexParticle)               {m_pTexParticle = _pTexParticle;};

  void SetAnimated          (const bool &_bAnimated)                {m_bAnimated = _bAnimated;};
  void SetTimeDiapo         (const float &_fTimeAnimationDiapo)     {m_fTimeAnimationDiapo = _fTimeAnimationDiapo;};
  void SetNumFiles          (const int &_iTexNumFiles)              {m_iTexNumFiles = _iTexNumFiles;};
  void SetColumnes          (const int &_iTexNumColumnes)           {m_iTexNumColumnes = _iTexNumColumnes;};

  //Get

  string                          GetId               () const {return m_szId;};
  Vect3f                          GetPos              () const {return m_vPosition;};
  float                           GetSizeX            () const {return m_fSizeX;};
  float                           GetSizeY            () const {return m_fSizeY;};
  CTexture*                       GetTexture          () const {return m_pTexParticle;};
  float                           GetTimeDiapo        () const {return m_fTimeAnimationDiapo;};
  int                             GetNumFiles         () const {return m_iTexNumFiles;};
  int                             GetNumColumnes      () const {return m_iTexNumColumnes;};
  bool                            GetAnimated         () const {return m_bAnimated;};

private:
  LPDIRECT3DVERTEXBUFFER9                   m_vBillboards;
  string    m_szId;
	Vect3f		m_vPosition;
	Vect3f		m_PointA, m_PointB, m_PointC, m_PointD;
	float			m_fSizeX, m_fSizeY;
	CTexture*  m_pTexParticle; 
  int        m_iTexNumFiles;// numero de files que te la texture
  int        m_iTexNumColumnes;// numero de columnes que te la textura
  float      m_fTimeAnimationActual;//temps k porta durant l'animacio
  float      m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
  int        m_iNumDiapo;//numero de diapositiva a la que ens trobem
  float      m_fIncrementV; //alçada de cada diapositiva 
  float      m_fIncrementU; //amplada de cada diapositiva
  int        m_iTotalDiapos;
  bool       m_bAnimated; //indica si el billboard es animat
  // cordenades de textura per a cada punt.
  float      m_fAU;
  float      m_fAV;
  float      m_fBU;
  float      m_fBV;
  float      m_fCU;
  float      m_fCV;
  float      m_fDU;
  float      m_fDV;
  D3DXCOLOR       m_Color1;
  D3DXCOLOR       m_Color2;
  D3DXCOLOR       m_Color3;
  D3DXCOLOR       m_Color4;
};

