#include "ParticleEmitter.h"
#include <math.h>
#include "Utils\Timer.h"
#include "VertexsStructs.h"
#include "TextureManager.h"
#include "ParticleManager.h"

CParticleEmitter::CParticleEmitter():
m_Particles(NUMPARTICLES)
{
 
  //SetGravity(D3DXVECTOR3(0.0f,0.0f,0.0f));
  SetPosition(D3DXVECTOR3(0.0f,0.0f,0.0f));
  SetMinEmitRate(10.0f);
  SetMaxEmitRate(30.0f);
  SetColor1(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetColor2(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetMinSize(1.0f);
  SetMaxSize(3.0f);
  SetSpawnDir1(D3DXVECTOR3(-5.0f, -5.0f, -5.0f));
  SetSpawnDir2(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
  SetVel(D3DXVECTOR3(1.0f,1.0f,1.0f));
  SetGravity(D3DXVECTOR3(0.0f,0.0f,0.0f));
 
  // initialize misc. other things
  m_szName        = "";
  m_szId          = "";
  m_pTexParticle  = NULL;
  m_vbParticles   = NULL;
  //SetVBSize(NUMPARTICLES / 10);
  m_fNumNewPartsExcess = 0.0f;
  m_fLife1 = 0.0f;
  m_fLife2 = 0.0f;
  //m_fAngle = 0.0f;

  m_bActive = false;

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
  m_vSpawnDir1 = _info->m_vSpawnDir1;
  m_vSpawnDir2 = _info->m_vSpawnDir2;
  m_pTexParticle = _info->m_pTexParticle;
  m_fLife1 = _info->m_fLife1;
  m_fLife2 = _info->m_fLife2;
  m_vColor = _info->m_vColor;
  m_vTimeColor = _info->m_vTimeColor;
  m_vDirection = _info->m_vDirection;
  m_vTimeDirection = _info->m_vTimeDirection;
  m_vGravity = _info->m_vGravity;
  m_vVel = _info->m_vVel;

  //crear un vector de direccion de tantas posicions com estat pot tenir (segons vector temps)
  int j=m_vTimeDirection.size();
  while(j!=0)
  {
    m_vNewDirection.push_back(m_vSpawnDir1);
    j--;
  }
  //crear un vector de color de tantas posicions com estat pot tenir (segons vector temps)
  j=m_vTimeColor.size();
  while(j!=0)
  {
    m_vNewColor.push_back(m_Color1);
    j--;
  }
 
}

void CParticleEmitter::Update(float fElapsedTime)
{

  if(!m_bActive)
    return;
  
  //1.] Updatejar les particules i en cas de que s'hagi acabat el seu temps de vida, posar 
  //    en el vector que la posicio esta lliure
  CParticle* particula = 0;
  for (int i =0; i<NUMPARTICLES;i++)
  {
    if(!m_Particles.IsFree(i))
    {
      particula = m_Particles.GetAt(i);
      if(!particula->Update(fElapsedTime))
      {
        m_Particles.Free(i);
      }
    }
  }
  

  //2.] Si es temps de crear particules noves fer-ho:
  int iNumNewParts = 0;
  float fEmitRateThisFrame = RandomNumber(m_fMinEmitRate, m_fMaxEmitRate);
  m_fNumNewPartsExcess = (fEmitRateThisFrame * fElapsedTime)+ m_fNumNewPartsExcess;
  
  if(m_fNumNewPartsExcess > 1.0f)
  {
    iNumNewParts = (int)m_fNumNewPartsExcess;
    m_fNumNewPartsExcess = m_fNumNewPartsExcess - (int)m_fNumNewPartsExcess;
  }

  
  for (int q=0; q < iNumNewParts; q++)
  {
    // Si hay espacio para una nueva part�cula:
    if (m_Particles.GetNumFreeElements() > 0)
    {
      CParticle* part = m_Particles.New();
      

      // determina el random de la vida de cada particula
      float fRandLife = RandomNumber(m_fLife1,m_fLife2);
      part->SetLifeTimer(fRandLife);

      // determina el random del tamany de la particula
      float fRandSize = RandomNumber(m_fMinSize,m_fMaxSize);
      part->SetSize(fRandSize);

      
      

      //*****************
      //es guarda en un vector totes les direccions que tindra la particula durant el seu temps de vida
	    //ja amb el valor calculat	(fet el random) 
	 
     
	    int i=m_vTimeDirection.size()-1;
      while(i>=0)
      {
        float fRandX = RandomNumber(m_vDirection[i*2].x, m_vDirection[(i*2)+1].x);
        float fRandY = RandomNumber(m_vDirection[i*2].y, m_vDirection[(i*2)+1].y);
        float fRandZ = RandomNumber(m_vDirection[i*2].z, m_vDirection[(i*2)+1].z);
        		    
        
		    m_vNewDirection[i] = D3DXVECTOR3(fRandX, fRandY, fRandZ);
		    i--;
        
       }	  
      part->m_vTimeDirection = m_vTimeDirection;
      part->m_vDirection = m_vNewDirection;

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
      part->SetGravity(m_vGravity);
      part->SetVel(m_vVel);
      part->m_vTimeColor = m_vTimeColor;
      part->m_vColor = m_vNewColor;
      //part->SetAngle(m_fAngle);
      part->SetPos(m_vPos);

      
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
		  if (FAILED(l_pd3dDevice->CreateVertexBuffer( NUMPARTICLES * sizeof(SPARTICLE_VERTEX), 
											D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,   
											SPARTICLE_VERTEX::GetFVF(),
											D3DPOOL_DEFAULT, 
											&m_vbParticles,NULL)))
		  {
			  bIsOk = false;
		  }
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
  
}
  
void CParticleEmitter::Render(CRenderManager* _pRM)
{
  if(!m_bActive)
    return;

  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();

  _pRM->EnableAlphaBlend();

  l_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
  l_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
  
  /*l_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
 
  l_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  l_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );*/

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
  l_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_C,  *((DWORD*)&l_fPointScaleC));
  
  l_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
   
  // Set up the vertex buffer to be rendered
  l_pd3dDevice->SetStreamSource( 0, m_vbParticles,0, sizeof(SPARTICLE_VERTEX));
  l_pd3dDevice->SetFVF( SPARTICLE_VERTEX::GetFVF() );

  //l_pd3dDevice->SetTexture(0, m_pTexParticle);
  m_pTexParticle->Activate(0);

  SPARTICLE_VERTEX* pVertices;
 
  // akesta es la bona
  
  m_vbParticles->Lock(  0, NUMPARTICLES * sizeof(SPARTICLE_VERTEX), (void **) &pVertices, D3DLOCK_DISCARD);
	  
    

 
  DWORD dwNumParticlesToRender = 0;
  for (int q=0; q < NUMPARTICLES; q++)
  {
    // Render each particle a bunch of times to get a blurring effect
    if (!m_Particles.IsFree(q)) 
	  {
      CParticle* part = m_Particles.GetAt(q);
      dwNumParticlesToRender++;
 
      pVertices->x = part->GetPos().x;
	    pVertices->y = part->GetPos().y;
	    pVertices->z = part->GetPos().z;
      pVertices->pointsize = part->GetSize();
      pVertices->color = (DWORD)part->GetColor();
      pVertices++;
    }
  }

  m_vbParticles->Unlock();

  l_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, dwNumParticlesToRender);
  
  // Reset render states
  l_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
  l_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

  _pRM->DisableAlphaBlend();
  //l_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}