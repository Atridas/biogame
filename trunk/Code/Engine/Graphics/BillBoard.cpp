#include "BillBoard.h"
#include <d3dx9.h>
#include "Camera.h"
#include <math.h>
//#include <Math\MathUtils.h>

#define D3DFVF_VERTEX_TEXTURED (D3DFVF_XYZ|D3DFVF_TEX1)


struct VERTEX_TEXTURED_OLD
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
	: m_Position(Vect3f(0,0,0))
	, m_SizeX(2)
	, m_SizeY(2)
	,angle(0)
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
  Vect3f l_VDirection = camera->GetDirection();
  Vect3f l_VUp = camera->GetVecUp();

  	// A ->centre de la particula     m_Position
	// n-> direction                  l_VDirection
	// P-> up                         l_VUp

	//P' = A + [n·(ESCALAR)(P - A)]*(VECTORIAL)n + 
  //     cos(theta)*[(P - A) - [n·(P - A)]*n] +
  //     - sin(theta)*[n cross (P - A)]
  
  Mat33f mat;
  l_VUp = mat.FromAxisAngle(l_VDirection.Normalize(), angle)* l_VUp;
   
  angle+=0.01f;
  //l_VUp.RotateZ(angle);

  /*//P-A
  Vect3f P_A;
  P_A= l_VUp - m_Position;

  // n·(P-A) -> nP_A
  Vect3f nP_A;
  nP_A = l_VDirection * P_A;

  // n·(P-A)*n -> nP_An
  Vect3f nP_An;
  nP_An= nP_A^l_VDirection;

  //[(P - A) - [n·(P - A)]*n]  -> PA_nP_An
  Vect3f PA_nP_An;
  PA_nP_An = P_A - nP_An;
  // n cross (P - A)
  Vect3f nCrossP_A;
  nCrossP_A= l_VDirection^P_A;

  //float angle;

  //l_VUp = m_Position + PA_nP_An + cos(angle)^PA_nP_An - sin(angle)^nCrossP_A;
  */
 
 
	//Vect3f l_VRight = l_VDirection^l_VUp; // producte vectorial
  Vect3f l_VRight = l_VUp^l_VDirection; // producte vectorial
  l_VRight.Normalize();
  m_PointA = m_Position - (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointB = m_Position + (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointC = m_Position - (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
	m_PointD = m_Position + (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);






 
}

void CBillBoard::Render(LPDIRECT3DDEVICE9 device,const LPDIRECT3DTEXTURE9& texture )
{
		VERTEX_TEXTURED_OLD l_Points[4];
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
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED_OLD));
		
		
}