#pragma once

#include <d3dx9.h>
#include "vector"
#include "Camera.h"

class CBillBoard
{
public:

	CBillBoard();
	~CBillBoard(void);

	void Update	( CCamera *camera );
	void Render	( LPDIRECT3DDEVICE9 device, const LPDIRECT3DTEXTURE9 & texture );
	
	void SetPos		( const Vect3f & position ) {m_Position = position;}
	void SetSize	( const float &sizeX, const float &sizeY ) {m_SizeX = sizeX; m_SizeY = sizeY;}


private:
	Vect3f		m_Position;
	Vect3f		m_PointA, m_PointB, m_PointC, m_PointD;
	float			m_SizeX, m_SizeY;
	float		angle;
	
	
};

