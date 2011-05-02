#include "BillBoard.h"
#include <d3dx9.h>
#include "Camera.h"
#include <math.h>
#define D3DFVF_VERTEX_TEXTURED (D3DFVF_XYZ|D3DFVF_TEX1)


struct VERTEX_TEXTURED
{
	float x, y, z;
    float u, v;         // COORDENADAS DE TEXTURAS
	static unsigned int getFlags()
	{
		return D3DFVF_VERTEX_TEXTURED;
	}
};


CBillBoard::~CBillBoard(void)
{
}

CBillBoard::CBillBoard()
	: m_Position(D3DXVECTOR3(0,0,0))
	, m_SizeX(2)
	, m_SizeY(2)
	{}

void CBillBoard::Update(CCamera *camera)
{
	
	/*
	A	/------------------\	B
		|              /   |
		|       POS /      |
		|      / *         |
		| /                |
	C	\------------------/	D
	*/


	Vect3f yaw_angle = camera->GetLookAt(); //GetYaw();
	D3DXVECTOR3 l_VRight;// = D3DXVECTOR3( cos(yaw_angle-D3DX_PI*0.5f), 0.0f, sin(yaw_angle-D3DX_PI*0.5f) );
	D3DXVECTOR3 l_VUp;//=camera->GetVecUp();
	float normalize =	l_VRight.x*l_VRight.x +
						l_VRight.y*l_VRight.y +
						l_VRight.z*l_VRight.z;
	normalize = sqrt( normalize );
	l_VRight = l_VRight/normalize;

	 normalize =	l_VUp.x*l_VUp.x +
					l_VUp.y*l_VUp.y +
					l_VUp.z*l_VUp.z;
	normalize = sqrt( normalize );
	l_VUp = l_VUp/normalize;

	m_PointA = m_Position - (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointB = m_Position + (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointC = m_Position - (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
	m_PointD = m_Position + (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
}

void CBillBoard::Render(LPDIRECT3DDEVICE9 device,const LPDIRECT3DTEXTURE9& texture )
{
		VERTEX_TEXTURED l_Points[4];
		unsigned short l_Indexes[6]={0,2,1,1,2,3};

		l_Points[0].x=m_PointA.x;
		l_Points[0].y=m_PointA.y;
		l_Points[0].z=m_PointA.z;
		l_Points[0].u=0.0f;
		l_Points[0].v=1.0f;
		
		l_Points[1].x=m_PointB.x;
		l_Points[1].y=m_PointB.y;
		l_Points[1].z=m_PointB.z;
		l_Points[1].u=1.0f;
		l_Points[1].v=1.0f;

		l_Points[2].x=m_PointC.x;
		l_Points[2].y=m_PointC.y;
		l_Points[2].z=m_PointC.z;
		l_Points[2].u=0.0f;
		l_Points[2].v=0.0f;

		l_Points[3].x=m_PointD.x;
		l_Points[3].y=m_PointD.y;
		l_Points[3].z=m_PointD.z;
		l_Points[3].u=1.0f;
		l_Points[3].v=0.0f;

		

		device->SetTexture(0,texture);
		//device->SetTexture(0, Texture);
		device->SetFVF(D3DFVF_VERTEX_TEXTURED);
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED));
		
		
}