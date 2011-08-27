#include "BillBoard.h"
#include <d3dx9.h>
#include "Camera.h"
#include <math.h>
#include "VertexsStructs.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Core.h"
#include "ParticleManager.h"
#include "StaticMeshEmptyMaterial.h"
#include "DiffuseTextureDecorator.h"
#include "SpritePropertyDecorator.h"


CBillBoard::CBillBoard()
	: m_szId("")
  ,m_szType("")
  ,m_vPosition(Vect3f(0,0,0))
  ,m_PointA(Vect3f(0,0,0))
  ,m_PointB(Vect3f(0,0,0))
  ,m_PointC(Vect3f(0,0,0))
  ,m_PointD(Vect3f(0,0,0))
  ,m_bAnimated(false)
  ,m_bActive(false)
	,m_fSizeX(2)
	,m_fSizeY(2)
  ,m_vBillboards(0)
	,m_fTimeAnimationDiapo(0.05f)
  ,m_iTexNumFiles(4)
  ,m_iTexNumColumnes(4)
  ,m_fTimeAnimationActual(0)
  ,m_iNumDiapo(1)
  ,m_pTexParticle(0)
  ,m_bBucleInfinit(true)
  //,m_fTimeLife(0.0f)
  ,m_Color(1,1,1,1)
  ,m_fAngle(0)
  ,m_pMaterial(0)
{
  m_pMaterial = new CStaticMeshEmptyMaterial();
}


void CBillBoard::Init(CRenderManager* rm)
{
  bool bIsOk = rm != NULL;

  if (bIsOk)
  {
	  
	  LPDIRECT3DDEVICE9 l_pd3dDevice = rm->GetDevice();

	  bIsOk = l_pd3dDevice != NULL;

	  if (bIsOk)
	  {

      if (FAILED(l_pd3dDevice->CreateVertexBuffer(sizeof(VERTEX_TEXTURED), 
											D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,   
											VERTEX_TEXTURED::GetFVF(),
											D3DPOOL_DEFAULT, 
											&m_vBillboards,NULL)))

		  {
			  bIsOk = false;
		  }
    }
  }

  if(bIsOk)
  { 
    bIsOk = m_InstancedData.Init(rm, 1);
  }

  SetOk(bIsOk);
}

void CBillBoard::Release()
{
  CHECKED_RELEASE(m_vBillboards);
  CHECKED_DELETE(m_pMaterial);

  if ( m_vBillboards!= NULL)
  {
    m_vBillboards->Release();
  }


}


void CBillBoard::SetTexture(CTexture* _pTexParticle)              
{
  m_pTexParticle = _pTexParticle;

  
  CHECKED_DELETE(m_pMaterial);
  
  m_pMaterial = new CStaticMeshEmptyMaterial();
  m_pMaterial = new CSpritePropertyDecorator(m_pMaterial, Vect2f(1.f, 1.f));
  m_pMaterial = new CDiffuseTextureDecorator(m_pMaterial,m_pTexParticle);
};

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

  if (!m_bActive)
    return;
  if(!m_bBucleInfinit)
  {
    if(m_iTotalDiapos==m_iNumDiapo)
    {
      m_bActive=false;
    }
  }


  if(camera)
  {
    Vect3f l_VDirection = camera->GetDirection();
    Vect3f l_VUp = camera->GetVecUp();
    Mat33f mat;
    //per si es vol rotar amb un cert angle
    //l_VUp = mat.FromAxisAngle(l_VDirection.Normalize(), angle)* l_VUp;
   
   // angle+=0.01f;
  
    Vect3f l_VRight = l_VUp^l_VDirection; // producte vectorial
    l_VRight.Normalize();
    l_VUp.Normalize();
    m_PointA = m_vPosition - (l_VRight*m_fSizeX*0.5f) - (l_VUp*m_fSizeY*0.5f);
	  m_PointB = m_vPosition + (l_VRight*m_fSizeX*0.5f) - (l_VUp*m_fSizeY*0.5f);
	  m_PointC = m_vPosition - (l_VRight*m_fSizeX*0.5f) + (l_VUp*m_fSizeY*0.5f);
	  m_PointD = m_vPosition + (l_VRight*m_fSizeX*0.5f) + (l_VUp*m_fSizeY*0.5f);
  }
 
    //***** NOMES SI ES ANIMADA
  if(m_bAnimated)
  {
   
    m_fIncrementV = (float)m_pTexParticle->GetHeight()/m_iTexNumFiles;
    m_fIncrementV= m_fIncrementV/m_pTexParticle->GetHeight();
    
    m_fIncrementU = (float)m_pTexParticle->GetWidth()/m_iTexNumColumnes;
    m_fIncrementU = m_fIncrementU/m_pTexParticle->GetWidth();
    
	  m_iTotalDiapos=m_iTexNumFiles*m_iTexNumColumnes;
	  int l_canviDiapo=1;
		
	  m_fTimeAnimationActual += fTimeDelta;


    if(m_fTimeAnimationActual>m_fTimeAnimationDiapo)
    {
      m_iNumDiapo++;
      m_fTimeAnimationActual=0;
    }


    if(m_iNumDiapo>m_iTotalDiapos)
    {
      m_iNumDiapo=1;
      m_fTimeAnimationActual=0;
    }

    if(camera)
    {
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
      m_fDV = m_fCV;
    }
  }else if(camera)
  {
    m_fAU = 0.0f;
    m_fAV = 1.0f;
    m_fBU = 1.0f;
    m_fBV = 1.0f;
    m_fCU = 0.0f;
    m_fCV = 0.0f;
    m_fDU = 1.0f;
    m_fDV = 0.0f;
  }
}

void CBillBoard::Render(CRenderManager* _pRM)

{
  if (!m_bActive)
    return;
  
#ifdef __PARTICLE_VIA_SHADER__
  RenderHW(_pRM);
#else
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();

	VERTEX_TEXTURED l_Points[4];
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

		
  m_pTexParticle->Activate(0);
  //l_pd3dDevice->SetStreamSource( 0, m_vBillboards,0, sizeof(VERTEX_TEXTURED));// no se si serveix aki
 	l_pd3dDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	l_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED));
#endif
		
}


void CBillBoard::RenderHW(CRenderManager* _pRM)
{
  
  CEffectManager* l_pEM = CORE->GetEffectManager();
  assert(l_pEM && l_pEM->IsOk());

  //omplim el buffer----------------------------------------------------------------------------------------------------------

  SParticleRenderInfo* l_mBuffer = m_InstancedData.GetBuffer(1, _pRM);
  
  l_mBuffer->x = m_vPosition.x;
  l_mBuffer->y = m_vPosition.y;
  l_mBuffer->z = m_vPosition.z;

  //TODO refer size
  l_mBuffer->sizeX = m_fSizeX;
  l_mBuffer->sizeY = m_fSizeY;

  l_mBuffer->angleSin = sin(m_fAngle + FLOAT_PI_VALUE * .25f);
  l_mBuffer->angleCos = cos(m_fAngle + FLOAT_PI_VALUE * .25f);
  
  //l_mBuffer->minU = m_fCU;
  //l_mBuffer->minV = m_fCV;
  //l_mBuffer->maxU = m_fBU;
  //l_mBuffer->maxV = m_fBV;
  if(m_bAnimated)
    l_mBuffer->diapo = (float)m_iNumDiapo - 1.f;
  else
    l_mBuffer->diapo = 0.f;

  l_mBuffer->color=(DWORD)m_Color;


  // Omplim el buffer -----------------------------------------------------------------------------------------
  bool result = m_InstancedData.SetData(l_mBuffer, 1, _pRM);
  assert(result);// ---


  // renderitzem -----------------------------------------------------------------------------------------------------------------


  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

  // Fem els set stream sources
  l_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA  | 1   ));
  l_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));


  result = m_InstancedData.SetStreamSource(_pRM, 1);
  assert(result);// ---

  //l_it->first->Render(_pRM, true);
  m_pMaterial->SetSpriteSize(Vect2f(1.f / m_iTexNumColumnes, 1.f / m_iTexNumFiles));
  CEffect* l_pEffect = l_pEM->ActivateMaterial(m_pMaterial);
  CORE->GetParticleManager()->GetRenderableVertexs()->Render(_pRM, l_pEffect);
}
