#pragma once

#include <d3dx9.h>
#include "vector"
#include "Camera.h"
#include "Texture.h"

class CBillBoard
{
public:

	CBillBoard();
	~CBillBoard(void);

	void Update	(float fTimeDelta, CCamera *camera );
	void Render	( LPDIRECT3DDEVICE9 device, const LPDIRECT3DTEXTURE9 & texture );
	
	void SetPos		( const Vect3f & position ) {m_Position = position;}
	void SetSize	( const float &sizeX, const float &sizeY ) {m_SizeX = sizeX; m_SizeY = sizeY;}


private:
	Vect3f		m_Position;
	Vect3f		m_PointA, m_PointB, m_PointC, m_PointD;
	float			m_SizeX, m_SizeY;
	CTexture*  m_pTexParticle; 
  int        m_iTexNumFiles;// numero de files que te la texture
  int        m_iTexNumColumnes;// numero de columnes que te la textura
  float      m_fTimeAnimationActual;//temps k porta durant l'animacio
  float      m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
  int        m_iNumDiapo;//numero de diapositiva a la que ens trobem
  float      m_fIncrementV; //alçada de cada diapositiva 
  float      m_fIncrementU; //amplada de cada diapositiva
  int        m_iTotalDiapos;
  // cordenades de textura per a cada punt.
  float      m_fAU;
  float      m_fAV;
  float      m_fBU;
  float      m_fBV;
  float      m_fCU;
  float      m_fCV;
  float      m_fDU;
  float      m_fDV;
};

