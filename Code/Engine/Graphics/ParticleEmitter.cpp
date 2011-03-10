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
  m_pd3dDevice    = NULL;
  //SetVBSize(NUMPARTICLES / 10);
  m_fNumNewPartsExcess = 0.0f;
  
  
  SetOk(true);
}

bool CParticleEmitter::Init(SParticleInfo* _info) 
{
  return true;
}
/*
void CParticleEmitter::Init(CXMLTreeNode& l_XMLParticle)
{
  m_szId=l_XMLParticle.GetPszISOProperty("id" ,"");
  //m_vPos = l_XMLParticle.GetVect3fProperty("Position");
  m_fMinEmitRate = l_XMLParticle.GetFloatProperty("MinEmitRate");
  m_fMaxEmitRate = l_XMLParticle.GetFloatProperty("MaxEmitRate");
  //m_Color1 = l_XMLParticle.GetVect4fProperty("Color1",Vect4f(0.0f),true);
  //m_Color2 = l_XMLParticle.GetVect4fProperty("Color2",Vect4f(0.0f),true);
  m_fMinSize = l_XMLParticle.GetFloatProperty("MinSize");
  m_fMaxSize = l_XMLParticle.GetFloatProperty("MaxSize");

}
*/
/*
bool CParticleEmitter::Load(CXMLTreeNode& l_XMLParticle)
{
  /*LOGGER->AddNewLog(ELL_INFORMATION, "CParticleManager::Load \"%s\"", _szFileName.c_str());

  m_szFileName = _szFileName;
  m_szFileName = l_XMLParticle.LoadFile("Data\XML\Particules.xml");

 // CXMLTreeNode l_XMLParticles;
 /* if(!l_XMLParticles.LoadFile(_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CParticleManager:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
    //SetOk(false);
    return false;
  }*/
/*
  CParticleManager* l_pParticleManager = CORE->GetParticleManager();
  
  
  int l_iNumParticleEmitters = l_XMLParticles.GetNumChildren();
  for(int i = 0; i < l_iNumParticleEmitters; i++)
  {
    CXMLTreeNode l_XMLParticle = l_XMLParticles(i);
    if(l_XMLParticle.IsComment()) 
    {
      continue;
    }
    m_szId=l_pParticleManager->m_vEmitterParticle[l_iNumParticleEmitters].GetId();
    m_fMinEmitRate = l_pParticleManager->m_vEmitterParticle[l_iNumParticleEmitters].GetMinEmitRate();
    m_fMaxEmitRate = l_pParticleManager->m_vEmitterParticle[l_iNumParticleEmitters].GetMaxEmitRate();
    m_fMinSize = l_pParticleManager->m_vEmitterParticle[l_iNumParticleEmitters].GetMinSize();
    m_fMaxSize = l_pParticleManager->m_vEmitterParticle[l_iNumParticleEmitters].GetMaxSize();
    
    m_szId=l_XMLParticle.GetPszISOProperty("id" ,"");
    //m_vPos = l_XMLParticle.GetVect3fProperty("Position");
    m_fMinEmitRate = l_XMLParticle.GetFloatProperty("MinEmitRate");
    m_fMaxEmitRate = l_XMLParticle.GetFloatProperty("MaxEmitRate");
    //m_Color1 = l_XMLParticle.GetVect4fProperty("Color1",Vect4f(0.0f),true);
    //m_Color2 = l_XMLParticle.GetVect4fProperty("Color2",Vect4f(0.0f),true);
    m_fMinSize = l_XMLParticle.GetFloatProperty("MinSize");
    m_fMaxSize = l_XMLParticle.GetFloatProperty("MaxSize");
    // m_vSpawnDir1;
    //m_vSpawnDir2;
    //m_fNumNewPartsExcess;
    //m_pTexParticle;
    
    
  
  }
  SetOk(true);
  return IsOk();
  return true;

}*/
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
    // Si hay espacio para una nueva partícula:
    if (m_Particles.GetNumFreeElements() > 0)
    {
      CParticle* part = m_Particles.New();
      part->SetLifeTimer(5.0f);
  
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
    }
  }

}
  
void CParticleEmitter::Init(CRenderManager* rm, const string& _texureFileName)
{
  
  m_Particles.DeleteAllElements();
  m_pd3dDevice = rm->GetDevice();
  m_pd3dDevice->CreateVertexBuffer( NUMPARTICLES * sizeof(VERTEX_PARTICLE), 
                                    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,   
                                    VERTEX_PARTICLE::GetFVF(),
                                    D3DPOOL_DEFAULT, 
                                    &m_vbParticles,NULL);

  m_pTexParticle = CORE->GetTextureManager()->GetResource(_texureFileName)->GetD3DTexture();
}
  
void CParticleEmitter::Release()
{

/*  CHECKED_RELEASE(m_vbParticles);
  CHECKED_RELEASE(m_pd3dDevice);
  CHECKED_RELEASE(m_pTexParticle);
 */
  

  m_Particles.DeleteAllElements();
  if ( m_vbParticles!= NULL)
  {
//    m_vbParticles->Release();
  }
}
  
void CParticleEmitter::Render()
{
  m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

  m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
 
   m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

  float l_fPointSize    = 100.0f;
  float l_fPointSizeMin = 0.00f;
  float l_fPointSizeMax = 1000.00f;
  float l_fPointScaleA  = 0.00f;
  float l_fPointScaleB  = 0.00f;
  float l_fPointScaleC  = 1.00f;
  m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE,     *((DWORD*)&l_fPointSize));
  m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&l_fPointSizeMin));    
  m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&l_fPointSizeMax));    
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_A,  *((DWORD*)&l_fPointScaleA));    
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_B,  *((DWORD*)&l_fPointScaleB));    
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_C,  *((DWORD*)&l_fPointScaleC));
  //m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

   
  // Set up the vertex buffer to be rendered
  m_pd3dDevice->SetStreamSource( 0, m_vbParticles,0, sizeof(VERTEX_PARTICLE));
  m_pd3dDevice->SetFVF( VERTEX_PARTICLE::GetFVF() );

  m_pd3dDevice->SetTexture(0, m_pTexParticle);

  VERTEX_PARTICLE *pVertices;
 

  m_vbParticles->Lock(  0, NUMPARTICLES * sizeof(VERTEX_PARTICLE),
                        (void **) &pVertices, D3DLOCK_DISCARD);

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

  m_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, dwNumParticlesToRender);
  
  // Reset render states
  m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

  m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}




                      