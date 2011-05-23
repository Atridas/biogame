#include "BillBoard.h"
#include <d3dx9.h>
#include "Camera.h"
#include <math.h>
//#include <Math\MathUtils.h>

#define D3DFVF_VERTEX_TEXTURED (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)


struct VERTEX_TEXTURED_OLD
{
	float x, y, z;
  uint32  color;
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
	,m_fTimeAnimationDiapo(0.05f)
  ,m_iTexNumFiles(4)
  ,m_iTexNumColumnes(4)
  ,m_fTimeAnimationActual(0)
  ,m_iNumDiapo(1)
  ,m_Color1(0,1,0,1)
  ,m_Color2(1,0,0,1)
  ,m_Color3(0,0,1,1)
  ,m_Color4(1,1,0,1)
	{}

void CBillBoard::Update(float fTimeDelta,CCamera *camera)
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
  Mat33f mat;
  //per si es vol rotar amb un cert angle
  //l_VUp = mat.FromAxisAngle(l_VDirection.Normalize(), angle)* l_VUp;
   
 // angle+=0.01f;
  
  Vect3f l_VRight = l_VUp^l_VDirection; // producte vectorial
  l_VRight.Normalize();
  m_PointA = m_Position - (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointB = m_Position + (l_VRight*m_SizeX*0.5f) - (l_VUp*m_SizeY*0.5f);
	m_PointC = m_Position - (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
	m_PointD = m_Position + (l_VRight*m_SizeX*0.5f) + (l_VUp*m_SizeY*0.5f);
  
  //*******************************************
  
     // m_fIncrementV = /*(float)m_pTexParticle->GetHeight()*/ 256.f/m_iTexNumFiles;
     // m_fIncrementV= m_fIncrementV/256.f;
     // m_fIncrementU = /*(float)m_pTexParticle->GetWidth()*/256.f/m_iTexNumColumnes;
     // m_fIncrementU = m_fIncrementU/256.f;
		//m_iTotalDiapos=m_iTexNumFiles*m_iTexNumColumnes;
		//int l_canviDiapo=1;
		
		//m_fTimeAnimationActual += fTimeDelta;
		
    // Per a quin numero de dispositiva tenim que ensenyar
		/*float l_fTempsEnters = m_fTimeAnimationActual/m_fTimeAnimationDiapo;
		m_iNumDiapo= (int)l_fTempsEnters;
    if(m_iNumDiapo!=l_canviDiapo)
    {
      int llubarru=1;
    }
    if(m_iNumDiapo>m_iTotalDiapos)
    {
      m_iNumDiapo=0;
      m_fTimeAnimationActual=0;
    }
		if(m_iNumDiapo>l_fTempsEnters)
		{
		  m_iNumDiapo++;
		}*/
  /*  if(m_fTimeAnimationActual>m_fTimeAnimationDiapo)
    {
      m_iNumDiapo++;
      m_fTimeAnimationActual=0;
    }


    if(m_iNumDiapo>m_iTotalDiapos)
    {
      m_iNumDiapo=1;
      m_fTimeAnimationActual=0;
    }
    //Per saber en quina posició esta la diapositiva que volem ensenyar
    bool l_bOk=false;
    int l_Columna=0, fila=2, AuxNumDiapo;
    AuxNumDiapo=m_iNumDiapo;
    while (l_bOk==false)
    {
      if(m_iTexNumColumnes<AuxNumDiapo)
      {
        AuxNumDiapo -= m_iTexNumColumnes;
        fila++;
      }else
      {
        l_Columna = AuxNumDiapo;
        l_bOk=true;
      }
	  }

    //Extreure les cordenades de textura concretas
    m_fAU = m_fIncrementU*(l_Columna-1);
    m_fAV = m_fIncrementV*(fila-1);//1- (m_fIncrementV*(fila-1));

    m_fBU = m_fAU+m_fIncrementU;
    m_fBV = m_fAV;

    m_fCU = m_fAU;
    m_fCV = m_fAV-m_fIncrementV;
   
    m_fDU = m_fBU;
    m_fDV = m_fCV;*/

    m_fAU = 0;
    m_fAV = 1;

    m_fBU = 1;
    m_fBV = 1;

    m_fCU = 0;
    m_fCV = 0;
   
    m_fDU = 1;
    m_fDV = 0;

 
}

void CBillBoard::Render(LPDIRECT3DDEVICE9 device,const LPDIRECT3DTEXTURE9& texture )

{

		VERTEX_TEXTURED_OLD l_Points[4];
		unsigned short l_Indexes[6]={0,2,1,1,2,3};
    
		l_Points[0].x=m_PointA.x;
		l_Points[0].y=m_PointA.y;
		l_Points[0].z=m_PointA.z;
		l_Points[0].u=m_fAU;
		l_Points[0].v=m_fAV;
    l_Points[0].color= (DWORD) m_Color1;
		
		l_Points[1].x=m_PointB.x;
		l_Points[1].y=m_PointB.y;
		l_Points[1].z=m_PointB.z;
		l_Points[1].u=m_fBU;
		l_Points[1].v=m_fBV;
    l_Points[1].color= (DWORD) m_Color2;

		l_Points[2].x=m_PointC.x;
		l_Points[2].y=m_PointC.y;
		l_Points[2].z=m_PointC.z;
		l_Points[2].u=m_fCU;
		l_Points[2].v=m_fCV;
    l_Points[2].color= (DWORD) m_Color3;

		l_Points[3].x=m_PointD.x;
		l_Points[3].y=m_PointD.y;
		l_Points[3].z=m_PointD.z;
		l_Points[3].u=m_fDU;
		l_Points[3].v=m_fDV;
    l_Points[3].color= (DWORD) m_Color4;

		

		device->SetTexture(0,texture);
		//device->SetTexture(0, Texture);
		device->SetFVF(D3DFVF_VERTEX_TEXTURED);
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED_OLD));
		
		
}