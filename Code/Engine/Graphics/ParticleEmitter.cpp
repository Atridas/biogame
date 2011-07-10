#include "ParticleEmitter.h"
#include <math.h>
#include "Utils\Timer.h"
#include "VertexsStructs.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "Effect.h"
#include "EffectManager.h"
#include "StaticMeshEmptyMaterial.h"
#include "DiffuseTextureDecorator.h"


CParticleEmitter::CParticleEmitter():
m_Particles(NUMPARTICLES),
m_fMinEmitRate(10.0f),
m_fMaxEmitRate(30.0f),
m_Color1(1.0f,1.0f,1.0f,1.0f),
m_Color2(1.0f,1.0f,1.0f,1.0f),
m_fMinSize(1.0f),
m_fMaxSize(3.0f),
m_vSpawnDir1(-5.0f, -5.0f, -5.0f),
m_vSpawnDir2(5.0f, 5.0f, 5.0f),
m_vVel(1.0f,1.0f,1.0f),
m_vGravity(0.0f,0.0f,0.0f),
m_pTexParticle(0),
m_vbParticles(0),
m_fNumNewPartsExcess  (0.0f ),
m_fLife1              (0.0f ),
m_fLife2              (0.0f ),
m_fAngle1             (0.0f ),
m_fAngle2             (0.0f ),
//m_fAngle            (0.0f ),
m_fSizeX              (1.0f ),
m_fSizeY              (1.0f ),
m_bBucleInfinit       (true ),
m_iNumBucle           (0    ),
m_iNumBucleAux        (0    ),
m_iCont               (5    ),
m_fContElapsedTime    (0.0f ),
m_fRandomRebootEmitter(0.0f ),
m_fRebootEmitter1     (0.0f ),
m_fRebootEmitter2     (0.0f ),
m_iNumDirections      (1    ),
m_bTotDeCop           (false),
m_fAgeParticle        (0.0f ),

 /* m_PointA=(D3DXVECTOR3(0.0f,0.0f,0.0f));
  m_PointB=(D3DXVECTOR3(0.0f,0.0f,0.0f));
  m_PointC=(D3DXVECTOR3(0.0f,0.0f,0.0f)); 
  m_PointD=(D3DXVECTOR3(0.0f,0.0f,0.0f));*/
m_bAnimated  ( false),
m_bActive    ( false),
m_bActiveAux ( true ),

m_InstancedData()
  
{
  m_pMaterial = new CStaticMeshEmptyMaterial();
  //TODO inicialitzar els vector de color i temps
}

void CParticleEmitter::SetAttributes(SParticleInfo* _info) 
{
  m_szId  = _info->m_szId;
  m_fMinEmitRate = _info->m_fMinEmitRate;
  m_fMaxEmitRate = _info->m_fMaxEmitRate;
  m_Color1 = _info->m_Color1; 
  m_Color2 = _info->m_Color2;
  m_fMinSize = _info->m_fMinSize;
  m_fMaxSize = _info->m_fMaxSize;
 // m_vSpawnDir1 = _info->m_vSpawnDir1;
 // m_vSpawnDir2 = _info->m_vSpawnDir2;
  SetTexParticle( _info->m_pTexParticle );
  m_fLife1 = _info->m_fLife1;
  m_fLife2 = _info->m_fLife2;
  m_vColor = _info->m_vColor;
  m_vTimeColor = _info->m_vTimeColor;
  m_vTimeColorInterpolation=_info->m_vTimeColorInterpolation;
  m_vSize = _info->m_vSize;
  m_vTimeSize = _info->m_vTimeSize;
  m_vTimeSizeInterpolation=_info->m_vTimeSizeInterpolation;

  


 // m_vDirection = _info->m_vDirection;
 // m_vTimeDirection = _info->m_vTimeDirection;
  //m_vTimeDirectionInterpolation= _info->m_vTimeDirectionInterpolation;
  m_vGravity = _info->m_vGravity;
  m_vVel = _info->m_vVel;
  m_fAngle1 = _info->m_fAngle1;
  m_fAngle2 = _info->m_fAngle2;
  m_bBucleInfinit  = _info->m_bBucleInfinit;
  m_iNumBucle = _info->m_iNumBucle;
  m_iNumBucleAux = _info->m_iNumBucle;
  m_fRebootEmitter1 = _info->m_fRebootEmitter1;
  m_fRebootEmitter2 = _info->m_fRebootEmitter2;
  m_bTotDeCop = _info->m_bTotDeCop;
  //*** per animació

  m_bAnimated = _info->m_bAnimated;
  m_vFilesColumnes = _info->m_vFilesColumnes;
  m_vTimeAnimated = _info->m_vTimeAnimated;
  m_vTimeAnimatedInterpolation = _info->m_vTimeAnimatedInterpolation;
  m_vTextureAnimation=_info->m_vTextureAnimation;
  m_iNumDirections=_info->m_iNumDirections;

  //crear un vector de direccion de tantas posicions com estat pot tenir (segons vector temps)
  //int j=m_vTimeDirection.size();
  for(int j=0;j<(int)m_vTimeDirection.size();j++)
  {
    m_vNewDirection.push_back(m_vSpawnDir1);
  }
  //crear un vector de color de tantas posicions com estat pot tenir (segons vector temps)
  for(int j=0;j<(int)m_vTimeColor.size();j++)
  {
    m_vNewColor.push_back(m_Color1);
  }
 
  //crear un vector de tamanys de tantas posicions com estats pot tenir (segons vector temps)
  for(int j=0;j<(int)m_vTimeSize.size();j++)
  {
    m_vNewSize.push_back(m_fMaxSize);
  }
}

void CParticleEmitter::Update(float fElapsedTime,CCamera *camera)
{
  string l_nom=m_szName;
  //if(!m_bActive)
  //  return;
  
  //1.] Updatejar les particules i en cas de que s'hagi acabat el seu temps de vida, posar 
  //    en el vector que la posicio esta lliure
  CParticle* particula = 0;
  for (int i =0; i<NUMPARTICLES;i++)
  {
    if(!m_Particles.IsFree(i))
    {
      particula = m_Particles.GetAt(i);
      if(!particula->Update(fElapsedTime, camera))
      {
        m_Particles.Free(i);
      }
    }else
	  {
		  m_iCont++;
	  }
  }
  
  if(!m_bActive)
    return;
  //2.] Si es temps de crear particules noves fer-ho:
  if(m_bBucleInfinit==false && m_fRandomRebootEmitter==0)
  {
    m_fRandomRebootEmitter = RandomNumber(m_fRebootEmitter1,m_fRebootEmitter2);
    if(m_fRandomRebootEmitter==0 && m_iCont==NUMPARTICLES)
	  {
		  m_bActive=false;
      m_iCont=5;
      m_iNumBucle=m_iNumBucleAux+1;
    }else
    {
      m_iCont=0;
    }
  }
  
	  
  if(m_bBucleInfinit==true || m_iNumBucle>0)
  {
	  int iNumNewParts = 0;
    float fEmitRateThisFrame = RandomNumber(m_fMinEmitRate, m_fMaxEmitRate);
	  m_fNumNewPartsExcess = (fEmitRateThisFrame * fElapsedTime)+ m_fNumNewPartsExcess;
  
	  if(m_fNumNewPartsExcess > 1.0f && m_bTotDeCop==false)
	  {
		  iNumNewParts = (int)m_fNumNewPartsExcess;
		  m_fNumNewPartsExcess = m_fNumNewPartsExcess - (int)m_fNumNewPartsExcess;
      m_iNumBucle--;
	  }
  
    if(m_bTotDeCop==true)
    {
      iNumNewParts = (int)RandomNumber(m_fMinEmitRate, m_fMaxEmitRate);   
      //iNumNewParts=5;
      m_iNumBucle--;
    }

    for (int q=0; q < iNumNewParts; q++)
	  {
		  // Si hay espacio para una nueva partícula:
		  if (m_Particles.GetNumFreeElements() > 0)
		  {
		    CParticle* part =m_Particles.New();
     
      
      

		    // determina el random de la vida de cada particula
		    float fRandLife = RandomNumber(m_fLife1,m_fLife2);
		    part->SetLifeTimer(fRandLife);

		    // determina el random del tamany de la particula
		    //float fRandSize = RandomNumber(m_fMinSize,m_fMaxSize);
		    //part->SetSize(fRandSize);

		    float fRandAngle = RandomNumber(m_fAngle1,m_fAngle2);
		    part->SetAngle(fRandAngle);
      
		    float l_iIncrementAngle= RandomNumber(0.0f,120.0f);
		    part->SetInitAngle(l_iIncrementAngle);
      

		    //*****************
		    //es guarda en un vector totes les direccions que tindra la particula durant el seu temps de vida
			    //ja amb el valor calculat	(fet el random) 
	 
     
		    int i=m_vTimeDirection.size()-1;
		    while(i>=0)
		    {
			    float fRandX = RandomNumber(m_vDirection[i*2].x, m_vDirection[(i*2)+1].x);
			    float fRandY = RandomNumber(m_vDirection[i*2].y, m_vDirection[(i*2)+1].y);
			    float fRandZ = RandomNumber(m_vDirection[i*2].z, m_vDirection[(i*2)+1].z);
        		    
        
				    m_vNewDirection[i] = Vect3f(fRandX, fRandY, fRandZ);
				    i--;
        
		    }	  
		    part->m_vTimeDirection = m_vTimeDirection;
		    part->m_vDirection = m_vNewDirection;
		    part->m_vTimeDirectionInterpolation = m_vTimeDirectionInterpolation;

		    //es guarda en un vector tots els colors que tindra la particula durant el seu temps de vida
			    //ja amb el valor calculat	  
		    /* j=m_vTimeColor.size();
		    while(j!=0)
		    {
			    m_vNewColor.push_back(m_Color1);
			    j--;
		    }*/
		    i=m_vTimeColor.size()-1;
		    while(i>=0)
		    {
       
        
			    float fRandR = RandomNumber(m_vColor[i*2].r, m_vColor[(i*2)+1].r);
			    float fRandG = RandomNumber(m_vColor[i*2].g, m_vColor[(i*2)+1].g);
			    float fRandB = RandomNumber(m_vColor[i*2].b, m_vColor[(i*2)+1].b);
			    float fRandA = RandomNumber(m_vColor[i*2].a, m_vColor[(i*2)+1].a);
		    
        
			    m_vNewColor[i]= D3DXCOLOR(fRandR, fRandG, fRandB, fRandA);
			    i--;
        
		    }	
      
        //*****************
        i=m_vTimeSize.size()-1;
		    while(i>=0)
		    {
       
          float fRandSize = RandomNumber(m_vSize[i*2], m_vSize[(i*2)+1]);
			    
          m_vNewSize[i]= fRandSize;
			    i--;
        
		    }	
//****************************************
        Vect3f l_vPos_aux;

		    if(m_szFormEmitter=="dummy")
		    {
		      /*l_vPos_aux.x = m_vPos.x+RandomNumber(m_vPosFormEmitter.x/-2,m_vPosFormEmitter.x/2);
          l_vPos_aux.y = m_vPos.y+RandomNumber(m_vPosFormEmitter.y/-2,m_vPosFormEmitter.y/2);
          l_vPos_aux.z = m_vPos.z+RandomNumber(m_vPosFormEmitter.z/-2,m_vPosFormEmitter.z/2);*/

          //l_vPos_aux.x = m_vPosition.x+RandomNumber(m_vPosFormEmitter.x/-2,m_vPosFormEmitter.x/2);
          //l_vPos_aux.y = m_vPosition.y+RandomNumber(m_vPosFormEmitter.y/-2,m_vPosFormEmitter.y/2);
          //l_vPos_aux.z = m_vPosition.z+RandomNumber(m_vPosFormEmitter.z/-2,m_vPosFormEmitter.z/2);

          l_vPos_aux.x = RandomNumber(m_vPosFormEmitter.x/-2,m_vPosFormEmitter.x/2);
          l_vPos_aux.y = RandomNumber(m_vPosFormEmitter.y/-2,m_vPosFormEmitter.y/2);
          l_vPos_aux.z = RandomNumber(m_vPosFormEmitter.z/-2,m_vPosFormEmitter.z/2);
		    }
		    part->SetGravity(m_vGravity);
		    part->SetVel(m_vVel);
		    part->m_vTimeColor = m_vTimeColor;
		    part->m_vTimeColorInterpolation = m_vTimeColorInterpolation;
		    part->m_vColor = m_vNewColor;

        part->m_vTimeSize = m_vTimeSize;
		    part->m_vTimeSizeInterpolation = m_vTimeSizeInterpolation;
		    part->m_vSize = m_vNewSize;

		    //part->SetAngle(m_fAngle);
		    part->SetPos(l_vPos_aux);

		    part->SetAnimated(m_bAnimated);
		    part->m_vFilesColumnes=m_vFilesColumnes;
		    part->m_vTimeAnimated = m_vTimeAnimated;
		    part->m_vTimeAnimatedInterpolation = m_vTimeAnimatedInterpolation;
		    part->SetTexParticle(m_pTexParticle);
        //retorna el valor de la textura. Aixo serveix per si vol canviar de textura una particula ja creada
        m_pTexParticle=part->GetTexParticle();
        

		    //**************************
		    /*if(m_bAnimated)
		    {
			    part->m_vFilesColumnes=m_vFilesColumnes;
			    part->m_vTimeAnimated = m_vTimeAnimated;
		    }*/
      }
	  }
  }else
  {
    
    /*if(m_fRandomRebootEmitter==0)
    {
      m_bActive=false;
      m_iNumBucle=m_iNumBucleAux;
    }*/
    if(m_fRandomRebootEmitter>0 && m_bBucleInfinit==false)
    {

      //tindria k anar amb el elapsed Time, pero fa coses rares, aixi k de moment es fa un simple comptador
      m_fContElapsedTime=m_fContElapsedTime+fElapsedTime;
      if(m_fRandomRebootEmitter<=m_fContElapsedTime)
      {
        m_iNumBucle=m_iNumBucleAux;
        m_fContElapsedTime=0;
        m_fRandomRebootEmitter=0;
      }
    }
  }
}
  
void CParticleEmitter::Init(CRenderManager* rm)
{
  bool bIsOk = rm != NULL;

  if (bIsOk)
  {
	  m_Particles.DeleteAllElements();
	  LPDIRECT3DDEVICE9 l_pd3dDevice = rm->GetDevice();

	  bIsOk = l_pd3dDevice != NULL;

	  if (bIsOk)
	  {
		   
// pointsize*****************************************
    /* if (FAILED(l_pd3dDevice->CreateVertexBuffer( NUMPARTICLES * sizeof(SPARTICLE_VERTEX), 
											D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,   
											SPARTICLE_VERTEX::GetFVF(),
											D3DPOOL_DEFAULT, 
											&m_vbParticles,NULL)))*/
//******************************************************

//BillBoard*****************************************************
      
      if (FAILED(l_pd3dDevice->CreateVertexBuffer(NUMPARTICLES * sizeof(VERTEX_TEXTURED), 
											D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,   
											VERTEX_TEXTURED::GetFVF(),
											D3DPOOL_DEFAULT, 
											&m_vbParticles,NULL)))
//*************************************************
		  {
			  bIsOk = false;
		  }
    }

    if(bIsOk)
    {
      bIsOk = m_InstancedData.Init(rm, NUMPARTICLES);
    }
		  
  }
  
  SetOk(bIsOk);

  if (!bIsOk)
  {
	  Release();
  }
}
  
void CParticleEmitter::Release()
{

  CHECKED_RELEASE(m_vbParticles);
  CHECKED_DELETE(m_pMaterial);
  
  m_Particles.DeleteAllElements();
  if ( m_vbParticles!= NULL)
  {
    m_vbParticles->Release();
  }

   

  m_vColor.clear();
  m_vDirection.clear();
  m_vNewColor.clear();
  m_vNewDirection.clear();
  m_vTimeColor.clear();
  m_vTimeDirection.clear();
  m_vFilesColumnes.clear();
  m_vTimeAnimated.clear();
  m_vTimeAnimatedInterpolation.clear();
  m_vTimeDirectionInterpolation.clear();
  m_vTimeColorInterpolation.clear();
  m_vTextureAnimation.clear();
  
}

void CParticleEmitter::SetTexParticle(CTexture* _pTexParticle)
{
  m_pTexParticle = _pTexParticle;

  
  CHECKED_DELETE(m_pMaterial);

  m_pMaterial = new CStaticMeshEmptyMaterial();
  m_pMaterial = new CDiffuseTextureDecorator(m_pMaterial,m_pTexParticle);
};
  
void CParticleEmitter::RenderHW(CRenderManager* _pRM)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();
  assert(l_pEM && l_pEM->IsOk());

  //omplim el buffer----------------------------------------------------------------------------------------------------------

  SParticleRenderInfo* l_mBuffer = m_InstancedData.GetBuffer(NUMPARTICLES, _pRM);

  CParticle* l_pParticula = 0;
  int cont = 0;
  for (int i =0; i<NUMPARTICLES;i++)
  {
    if(!m_Particles.IsFree(i))
    {
      l_pParticula = m_Particles.GetAt(i);
      if(l_pParticula->GetAge() > 0)
      {
        l_pParticula->FillInstanceData(&(l_mBuffer[cont]));
        cont++;
      }

      //l_mBuffer[cont] = (*l_itInst)->GetMat44().GetD3DXMatrix();
    }
  }
  // Omplim el buffer -----------------------------------------------------------------------------------------
  bool result = m_InstancedData.SetData(l_mBuffer, cont, _pRM);
  assert(result);// ---


  // renderitzem -----------------------------------------------------------------------------------------------------------------


  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

  // Fem els set stream sources
  l_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA  | cont));

  l_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));

  result = m_InstancedData.SetStreamSource(_pRM, 1);
  assert(result);// ---

  //l_it->first->Render(_pRM, true);
  CEffect* l_pEffect = l_pEM->ActivateMaterial(m_pMaterial);
  CORE->GetParticleManager()->GetRenderableVertexs()->Render(_pRM, l_pEffect);

}

void CParticleEmitter::Render(CRenderManager* _pRM)
{
  if(!m_bActive)
    return;

  _pRM->SetTransform(GetMat44());

#ifdef __PARTICLE_VIA_SHADER__
  RenderHW(_pRM);
#else
//POINTSIZE ***************************************************
 /*
  float l_fPointSize    = 100.0f;
  float l_fPointSizeMin = 0.00f;
  float l_fPointSizeMax = 1000.00f;
  float l_fPointScaleA  = 0.00f;
  float l_fPointScaleB  = 0.00f;
  float l_fPointScaleC  = 1.00f;
  

  l_pd3dDevice->SetRenderState(D3DRS_POINTSIZE,     *((DWORD*)&l_fPointSize));
  l_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&l_fPointSizeMin));    
  l_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&l_fPointSizeMax));    
  l_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_A,  *((DWORD*)&l_fPointScaleA));    
  l_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_B,  *((DWORD*)&l_fPointScaleB));    
  l_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_C,  *((DWORD*)&l_fPointScaleC));*/
  
 /*  
  // Set up the vertex buffer to be rendered
  l_pd3dDevice->SetStreamSource( 0, m_vbParticles,0, sizeof(SPARTICLE_VERTEX));
  l_pd3dDevice->SetFVF( SPARTICLE_VERTEX::GetFVF() );*/

  //l_pd3dDevice->SetTexture(0, m_pTexParticle);
  //m_pTexParticle->Activate(0);

  //SPARTICLE_VERTEX* pVertices;
 
  // akesta es la bona
  
  //m_vbParticles->Lock(  0, NUMPARTICLES * sizeof(SPARTICLE_VERTEX), (void **) &pVertices, D3DLOCK_DISCARD);*/

//*****************************************************************
	  
//BILLBOARD***********************
  VERTEX_TEXTURED l_Points[4*NUMPARTICLES];
		//unsigned short l_Indexes[6]={0,2,1,1,2,3}; 
    unsigned short l_Indexes[6*NUMPARTICLES]; 
    int l_cont=0;
//***********************************
 
  DWORD dwNumParticlesToRender = 0;
  for (int q=0; q < NUMPARTICLES; q++)
  {
    // Render each particle a bunch of times to get a blurring effect
    if (!m_Particles.IsFree(q)) 
	  {
      CParticle* part = m_Particles.GetAt(q);
      dwNumParticlesToRender++;

      l_Indexes[0+(l_cont*6)]=0+(l_cont*4);
      l_Indexes[1+(l_cont*6)]=2+(l_cont*4);
      l_Indexes[2+(l_cont*6)]=1+(l_cont*4);
      l_Indexes[3+(l_cont*6)]=1+(l_cont*4);
      l_Indexes[4+(l_cont*6)]=2+(l_cont*4);
      l_Indexes[5+(l_cont*6)]=3+(l_cont*4);
//POINTSIZE***************************************
  /*    pVertices->x = part->GetPos().x;
	    pVertices->y = part->GetPos().y;
	    pVertices->z = part->GetPos().z;
      pVertices->pointsize = part->GetSize();
      pVertices->color = (DWORD)part->GetColor();
      pVertices++;*/
//***********************************************

//BILLBOLARD*****************************

      l_Points[0+(l_cont*4)].x=part->GetPointA().x;
		  l_Points[0+(l_cont*4)].y=part->GetPointA().y;
		  l_Points[0+(l_cont*4)].z=part->GetPointA().z;
      l_Points[0+(l_cont*4)].u=part->GetAU();
      l_Points[0+(l_cont*4)].v=part->GetAV();
      l_Points[0+(l_cont*4)].color=(DWORD)part->GetColor();
     
		  
		  l_Points[1+(l_cont*4)].x=part->GetPointB().x;
		  l_Points[1+(l_cont*4)].y=part->GetPointB().y;
		  l_Points[1+(l_cont*4)].z=part->GetPointB().z;
		  l_Points[1+(l_cont*4)].u=part->GetBU();
		  l_Points[1+(l_cont*4)].v=part->GetBV();
      l_Points[1+(l_cont*4)].color=(DWORD)part->GetColor();

		  l_Points[2+(l_cont*4)].x=part->GetPointC().x;
		  l_Points[2+(l_cont*4)].y=part->GetPointC().y;
		  l_Points[2+(l_cont*4)].z=part->GetPointC().z;
		  l_Points[2+(l_cont*4)].u=part->GetCU();
		  l_Points[2+(l_cont*4)].v=part->GetCV();
      l_Points[2+(l_cont*4)].color=(DWORD)part->GetColor();

		  l_Points[3+(l_cont*4)].x=part->GetPointD().x;
		  l_Points[3+(l_cont*4)].y=part->GetPointD().y;
		  l_Points[3+(l_cont*4)].z=part->GetPointD().z;
		  l_Points[3+(l_cont*4)].u=part->GetDU();
		  l_Points[3+(l_cont*4)].v=part->GetDV();
      l_Points[3+(l_cont*4)].color=(DWORD)part->GetColor();

      
      l_cont++;
      
    }
  }

//POINTSIZE******************************************
 /* m_vbParticles->Unlock();
  l_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, dwNumParticlesToRender);
  // Reset render states
   l_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
  l_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

  _pRM->DisableAlphaBlend();
  //l_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );*/
//*******************************************************************
 
//BILLBOARD**********************************************************
  m_pTexParticle->Activate(0);
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();
  l_pd3dDevice->SetStreamSource( 0, m_vbParticles,0, sizeof(VERTEX_TEXTURED));// no se si serveix aki
  l_pd3dDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
  l_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,4*l_cont,2*l_cont,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED));
#endif
}
