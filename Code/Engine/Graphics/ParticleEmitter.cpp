#include "include\ParticleEmitter.h"
#include <math.h>
#include "Utils\Timer.h"
#include "VertexsStructs.h"

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
  SetMinEmitRate(30.0f);
  SetMaxEmitRate(30.0f);
  SetColor1(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetColor2(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetMinSize(1.0f);
  SetMaxSize(1.0f);
  SetSpawnDir1(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
  SetSpawnDir2(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
 
  // initialize misc. other things
  m_pd3dDevice = NULL;
  //SetVBSize(NUMPARTICLES / 10);
  m_fNumNewPartsExcess = 0.0f;
}


CParticleEmitter::~CParticleEmitter(void)
{
  Release();
}

void CParticleEmitter::Update(float fElapsedTime)
{
  //1.] Updatejar les particules i en cas de que s'hagi acabat el seu temps de vida, posar 
  //    en el vector que la posicio esta lliure
  CParticle* particula = 0;
  for (int i =0; i<NUMPARTICLES;i++)
  {
    if(m_Particles.IsAlive(i))
    {
      particula = m_Particles.GetAt(i);
      if(!particula->Update(fElapsedTime))
      {
        m_Particles.Free(i);
      }
    }
  }

  //2.] Si es temps de crear particules noves fer-ho:
 /* float fEmitRateThisFrame = RandomNumber(m_fMinEmitRate, m_fMaxEmitRate);
  int iNumNewParts = fEmitRateThisFrame * fElapsedTime;
  m_fNumNewPartsExcess = iNumNewParts + m_fNumNewPartsExcess;
  if(m_fNumNewPartsExcess > 1.0f)
  {
    iNumNewParts = (int)m_fNumNewPartsExcess;
    m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
  }

  
  for (int q=0; q < iNumNewParts; q++)
  {
    // Si hay espacio para una nueva partícula:
    CParticle *part = m_Particles.New();
  
    // determine a random vector between dir1 and dir2
    float fRandX = RandomNumber(m_vSpawnDir1.x, m_vSpawnDir2.x);
    float fRandY = RandomNumber(m_vSpawnDir1.y, m_vSpawnDir2.y);
    float fRandZ = RandomNumber(m_vSpawnDir1.z, m_vSpawnDir2.z);

    
    part->SetDir(D3DXVECTOR3(fRandX, fRandY, fRandZ));
    part->SetPos(m_vPos);
    
                      
    float fRandR = RandomNumber(m_Color1.r, m_Color2.r);
    float fRandG = RandomNumber(m_Color1.g, m_Color2.g);
    float fRandB = RandomNumber(m_Color1.b, m_Color2.b);
    float fRandA = RandomNumber(m_Color1.a, m_Color2.a);
                      
    
    part->SetColor(D3DXCOLOR(fRandR, fRandG, fRandB, fRandA));
  }*/

}
  
void CParticleEmitter::Init(CRenderManager* rm, const string& _texureFileName)
{
/*
  if(FAILED(hr = m_pd3dDevice->CreateVertexBuffer(m_iVBSize * sizeof(VERTEX_PARTICLE),
     D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,   D3DFVF_PARTICLE, D3DPOOL_DEFAULT, &m_vbParticles))) 
  {
    return(hr);
  }*/
}
  
void CParticleEmitter::Release()
{
  m_Particles.DeleteAllElements();
}
  
void CParticleEmitter::Render()
{/*
  m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(100.0f) );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.00f) );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MAX, FtoDW(1000.00f) );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.00f) );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.00f) );

  // Set up the vertex buffer to be rendered
  m_pd3dDevice->SetStreamSource( 0, m_vbParticles, sizeof(VERTEX_PARTICLE));
  m_pd3dDevice->SetVertexShader( D3DFVF_PARTICLE );

  m_pd3dDevice->SetTexture(0, m_texParticle);

  VERTEX_PARTICLE *pVertices;
  DWORD dwNumParticlesToRender = 0;

  if(FAILED(hr = m_vbParticles->Lock(0, m_iVBSize * sizeof(VERTEX_PARTICLE),
  (BYTE **) &pVertices, D3DLOCK_DISCARD)))
  {
    return hr;
  }
  for (int q=0; q < NUMPARTICLES; q++)
  {
    // Render each particle a bunch of times to get a blurring effect
    if (m_Particles.IsAlive(q)) {
      CParticle &part = m_Particles.GetAt(q)
      dwNumParticlesToRender++;
 
      //pVertices->position = part.m_vPos;
      pVertices->x = part.m_vPos.x;
      pVertices->y = part.m_vPos.y;
      pVertices->z = part.m_vPos.z;
      pVertices->pointsize = 1.0f;
      pVertices->color = (DWORD)part.m_Color;
      pVertices++;
  }

  m_vbParticles->Unlock();

  if(FAILED(hr = m_pd3dDevice->DrawPrimitive(
  D3DPT_POINTLIST, 0, dwNumParticlesToRender)))
  return hr;


  // Reset render states
  m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );*/
}




                      