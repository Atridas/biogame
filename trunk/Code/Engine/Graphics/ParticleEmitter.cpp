#include "ParticleEmitter.h"
#include <math.h>
#include "Utils\Timer.h"
#include "VertexsStructs.h"
#include "TextureManager.h"
#include "ParticleManager.h"

float RandomNumber(float _fMin, float _fMax)
{
  float l_fNumber;
  if(_fMin> _fMax)
  {
    float l_fAux = _fMax;
    _fMax = _fMin;
    _fMin=l_fAux;
  }
  l_fNumber=( (rand()/static_cast<float>(RAND_MAX)) * (_fMax - _fMin) + _fMin);

  return l_fNumber;
}

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
 
  // initialize misc. other things
  m_pTexParticle  = NULL;
  m_vbParticles   = NULL;
  //SetVBSize(NUMPARTICLES / 10);
  m_fNumNewPartsExcess = 0.0f;

  //TODO inicialitzar els vector de clolor i temps
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
  m_vTime = _info->m_vTime;
  //m_vNewColor = D3DXCOLOR(0.0, 0.0, 0.0, 0.0);
}

void CParticleEmitter::Update(float fElapsedTime)
{

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
      // determine a random vector between dir1 and dir2
      float fRandX = RandomNumber(m_vSpawnDir1.x, m_vSpawnDir2.x);
      float fRandY = RandomNumber(m_vSpawnDir1.y, m_vSpawnDir2.y);
      float fRandZ = RandomNumber(m_vSpawnDir1.z, m_vSpawnDir2.z);

    
      part->SetDir(D3DXVECTOR3(fRandX, fRandY, fRandZ));
      part->SetPos(m_vPos);


      int j=m_vTime.size();
      while(j!=0)
      {
        m_vNewColor.push_back(m_Color1);
        j--;
      }
	    int i=m_vTime.size()-1;
      while(i!=0)
      {
        
	      float fRandR = RandomNumber(m_vColor[i*2].r, m_vColor[(i*2)+1].r);
		    float fRandG = RandomNumber(m_vColor[i*2].g, m_vColor[(i*2)+1].g);
		    float fRandB = RandomNumber(m_vColor[i*2].b, m_vColor[(i*2)+1].b);
		    float fRandA = RandomNumber(m_vColor[i*2].a, m_vColor[(i*2)+1].a);
		    
        
		    m_vNewColor[i]= D3DXCOLOR(fRandR, fRandG, fRandB, fRandA);
		    i--;
        
       }	  
      part->m_vTime = m_vTime;
      part->m_vColor = m_vNewColor;
    //***************************************************
    // nou
  /*   
      int i=m_vTime.size()-1;
      float temps=0;
      float temps2=0;
      float temps3=0;
      while(i!=0)
      {
        temps3=part->GetAge();
        temps=m_vTime[i];
        temps2=part->GetLifeTimer();
        if(m_vTime[i]< part->GetAge())
        {
          m_Color1 = m_vColor[i*2];
          m_Color2 = m_vColor[(i*2)+1];
          i=1;
        }
        --i;
      }
      */
    //*************************************************

                      
      float fRandR = RandomNumber(m_Color1.r, m_Color2.r);
      float fRandG = RandomNumber(m_Color1.g, m_Color2.g);
      float fRandB = RandomNumber(m_Color1.b, m_Color2.b);
      float fRandA = RandomNumber(m_Color1.a, m_Color2.a);
                      
    
      part->SetColor(D3DXCOLOR(fRandR, fRandG, fRandB, fRandA));
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
}
  
void CParticleEmitter::Render(CRenderManager* _pRM)
{
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
      pVertices->pointsize = 1.0f;
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