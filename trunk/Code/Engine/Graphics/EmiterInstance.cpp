#include "EmiterInstance.h"
#include "CoreEmiterManager.h"
#include "Effect.h"
#include "EffectManager.h"
#include "RenderManager.h"
#include "RenderableVertexs.h"

bool CEmiterInstance::Init(const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume )
{
  assert(!IsOk());
  SetMat44( _Position.GetMat44() );

  m_szCoreName  = _szCoreName;
  m_vMaxVolume  = _vVolume * .5f;
  m_vMinVolume  = -m_vMaxVolume;
  m_fVolume     = _vVolume.x * _vVolume.y * _vVolume.z;
  m_pCoreEmiter = CORE->GetCoreEmiterManager()->GetCoreEmiter(m_szCoreName);

  m_fTimeToNextParticle = 1.f / (m_pCoreEmiter->GetEmitRate() * m_fVolume);
  m_iActiveParticles = 0;
  memset(m_Particles, 0, sizeof(m_Particles));

  m_bAwake = true;
  m_fTimeToAwakeOrSleep = m_pCoreEmiter->GetAwakeTime();

  m_pObjectReference = 0;
  bool l_bIsOk = m_InstancedData.Init(CORE->GetRenderManager(), MAX_PARTICLES_PER_EMITER);
  SetOk(l_bIsOk);
  return l_bIsOk;
}

void CEmiterInstance::Reset( const CObject3D& _Position, const Vect3f& _vVolume )
{
  assert(IsOk());

  SetMat44( _Position.GetMat44() );
  m_vMaxVolume  = _vVolume * .5f;
  m_vMinVolume  = -m_vMaxVolume;
  m_fVolume     = _vVolume.x * _vVolume.y * _vVolume.z;
  m_pCoreEmiter = CORE->GetCoreEmiterManager()->GetCoreEmiter(m_szCoreName);

  m_RecyclingParticles.DeleteAllElements();
  m_fTimeToNextParticle = 1.f / (m_pCoreEmiter->GetEmitRate() * m_fVolume);
  m_iActiveParticles = 0;
  memset(m_Particles, 0, sizeof(m_Particles));

  m_bAwake = true;
  m_fTimeToAwakeOrSleep = m_pCoreEmiter->GetAwakeTime();
}

void CEmiterInstance::Update(float _fDeltaTime)
{
  assert(IsOk());
  //mirem si el delta time és massa gran, per no fer actualitzacions massa a saco, les capem a un min de framerate
  if(_fDeltaTime > MAX_PARTICLE_DELTA_TIME) _fDeltaTime = MAX_PARTICLE_DELTA_TIME;

  CRenderManager* l_pRM = CORE->GetRenderManager();
  SParticleRenderInfo* l_pInstanceBuffer = m_InstancedData.GetBuffer(MAX_PARTICLES_PER_EMITER, l_pRM);
  assert(l_pInstanceBuffer);

  //actualitzem les partícules
  for(int i = 0; i < m_iActiveParticles; ++i)
  {
    CParticleInstance* l_pParticle = m_RecyclingParticles.GetAt(m_Particles[i]);
    if(l_pParticle->Update(_fDeltaTime))
    {
      //la partícula encara està viva, omplim la informació al buffer de rendetizatge.
      l_pParticle->FillRenderInfo(l_pInstanceBuffer[i]);
    }
    else
    {
      m_RecyclingParticles.Free(m_Particles[i]);
      m_iActiveParticles--;
      m_Particles[i] = m_Particles[m_iActiveParticles];
      if(i != m_iActiveParticles)
        --i;
    }
  }
  
  m_fTimeToAwakeOrSleep -= _fDeltaTime;
  while(m_fTimeToAwakeOrSleep <= 0)
  {
    m_bAwake = !m_bAwake;
    if(m_bAwake)
    {
      m_fTimeToAwakeOrSleep += m_pCoreEmiter->GetAwakeTime();
    }
    else
    {
      m_fTimeToAwakeOrSleep += m_pCoreEmiter->GetSleepTime();
    }
  }

  // si ha passat prou temps com per crear una partícula nova
  while(_fDeltaTime > m_fTimeToNextParticle)
  {

    _fDeltaTime -= m_fTimeToNextParticle;

    m_fTimeToNextParticle = 1.f / (m_pCoreEmiter->GetEmitRate() * m_fVolume); //carreguem el temps fins la següent partícula

    if(m_bAwake && m_iActiveParticles < MAX_PARTICLES_PER_EMITER) //comprovem que el buffer no hagi quedat ple
    {
      int l_iParticle = m_RecyclingParticles.NewIndex(); //agafem una partícula del buffer
      CParticleInstance* l_pParticle = m_RecyclingParticles.GetAt(l_iParticle);

      //creem la partícula dintre de la caixa inicial
      Vect3f l_vRnd(Random01(),Random01(),Random01());
      Vect3f l_v_1_Minus_Rnd(1.f - l_vRnd.x, 1.f - l_vRnd.y, 1.f - l_vRnd.z);
      Vect3f l_vInitialPosition = ( l_v_1_Minus_Rnd.Scale(m_vMinVolume) ) + ( l_vRnd.Scale(m_vMaxVolume) );

      //inicialitzem la partícula
      l_pParticle->Init(m_pCoreEmiter, l_vInitialPosition);
      m_Particles[m_iActiveParticles] = l_iParticle;

      //actualitzem aquesta partícula fins al final d'aquest frame
      if(l_pParticle->Update(_fDeltaTime))
      {
        //la partícula encara està viva, omplim la informació al buffer de rendetizatge.
        l_pParticle->FillRenderInfo(l_pInstanceBuffer[m_iActiveParticles]);
      }
      else
      {
        m_RecyclingParticles.Free(m_Particles[m_iActiveParticles]);
        m_iActiveParticles--;
      }


      m_iActiveParticles++;
    }
    else
    {
      m_fTimeToNextParticle = _fDeltaTime;
      break;
    }
  }

  m_fTimeToNextParticle -= _fDeltaTime;

  bool l_bResult = m_InstancedData.SetData(l_pInstanceBuffer, m_iActiveParticles, l_pRM);
  assert(l_bResult);
}



void CEmiterInstance::Render(CRenderManager* _pRM)
{
  assert(IsOk());

  if(m_iActiveParticles == 0)
    return;

  CEffectManager* l_pEM = CORE->GetEffectManager();
  assert(l_pEM && l_pEM->IsOk());

  if(m_pObjectReference)
  {
    _pRM->SetTransform(m_pObjectReference->GetMat44());
  }
  else
  {
    _pRM->SetTransform(GetMat44());
  }
  
  // renderitzem -----------------------------------------------------------------------------------------------------------------


  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

  // Fem els set stream sources
  l_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA  | m_iActiveParticles));

  l_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));

  bool l_bResult = m_InstancedData.SetStreamSource(_pRM, 1);
  assert(l_bResult);// ---

  CEffect* l_pEffect = l_pEM->ActivateMaterial(m_pCoreEmiter->GetMaterial());
  _pRM->GetParticleVertexs()->Render(_pRM, l_pEffect);

  
}


void CEmiterInstance::DebugRender(CRenderManager* _pRM)
{
  assert(IsOk());
  if(m_pObjectReference)
  {
    _pRM->SetTransform(m_pObjectReference->GetMat44());
  }
  else
  {
    _pRM->SetTransform(GetMat44());
  }
  _pRM->DrawCube(m_vMaxVolume - m_vMinVolume, colRED);
  
}