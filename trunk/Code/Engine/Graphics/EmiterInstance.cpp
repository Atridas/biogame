#include "EmiterInstance.h"
#include "EmiterCoreManager.h"
#include "Effect.h"
#include "EffectManager.h"
#include "RenderManager.h"
#include "RenderableVertexs.h"
#include "SimpleEmiterCore.h"
#include "AggregateEmiterCore.h"
#include "PortalManager.h"
#include "Room.h"
#include "Material.h"

bool CEmiterInstance::Init(const string& _szCoreName, const CObject3D& _Position, const Vect3f& _vVolume, int _iMaxParticles, bool _bBillboardMode )
{
  assert(!IsOk());
  SetOk(true);
  SetMat44( _Position.GetMat44() );

  m_szCoreName     = _szCoreName;
  m_vVolume        = _vVolume;
  m_vMaxVolume     = m_vVolume * .5f;
  m_vMinVolume     = -m_vMaxVolume;
  m_fVolume        = _vVolume.x * _vVolume.y * _vVolume.z;
  m_pEmiterCore    = CORE->GetEmiterCoreManager()->GetEmiterCore(m_szCoreName);
  m_bBillboardMode = _bBillboardMode;
  m_iMaxParticles  = _iMaxParticles;

  m_RecyclingParticles.Reset(m_iMaxParticles);
  m_iaParticles    = new int[m_iMaxParticles];

  GetBoundingBox()->Init(_vVolume);

  if(m_bBillboardMode)
  {
    if(m_pEmiterCore->IsSimpleEmiter())
    {
      m_Billboard.Init(dynamic_cast<const CSimpleEmiterCore*>(m_pEmiterCore), Vect3f(0,0,0),true);
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING, "Trying to initialize billboard with aggregate emiter.");
      m_pEmiterCore = CORE->GetEmiterCoreManager()->GetNullEmiter();
      m_Billboard.Init(CORE->GetEmiterCoreManager()->GetNullEmiter(), Vect3f(0,0,0),true);
    }
  } else if(m_pEmiterCore->IsSimpleEmiter())
  {
    m_bIsSimple = true;
    const CSimpleEmiterCore *l_pEmiterCore = dynamic_cast<const CSimpleEmiterCore*>(m_pEmiterCore);

    m_fTimeToNextParticle = 1.f / (l_pEmiterCore->GetEmitRate() * m_fVolume);
    m_iActiveParticles = 0;
    memset(m_iaParticles, 0, sizeof(int) * m_iMaxParticles);

    m_bAwake = true;
    m_fTimeToAwakeOrSleep = l_pEmiterCore->GetAwakeTime();

    m_pObjectReference = 0;
    m_bActive = true;
  }
  else
  {
    m_bIsSimple = false;
    const CAggregateEmiterCore *l_pEmiterCore = dynamic_cast<const CAggregateEmiterCore*>(m_pEmiterCore);

    vector<CAggregateEmiterCore::SEmiters>::const_iterator l_it  = l_pEmiterCore->GetChilds().begin();
    vector<CAggregateEmiterCore::SEmiters>::const_iterator l_end = l_pEmiterCore->GetChilds().end();


    for(; l_it != l_end; ++l_it)
    {
      CEmiterInstance *l_pChild = new CEmiterInstance();
      Vect3f l_vChildBox = l_it->volume.GetScaled(_vVolume);
      //Mat44f l_mChildTransform = Mat44f(_vVolume.x, 0, 0, 0,
      //                                  0, _vVolume.y, 0, 0,
      //                                  0, 0, _vVolume.z, 0,
      //                                  0, 0, 0,          1)
      //                           * l_it->movement.GetMat44();
      //CObject3D l_O3D;
      //l_O3D.SetMat44(l_mChildTransform);
      Mat44f l_mChildTransform = l_it->movement.GetMat44();
      Vect3f l_vChildTranslation = l_mChildTransform.GetTranslationVector();
      l_vChildTranslation.Scale(_vVolume);
      l_mChildTransform.Translate(l_vChildTranslation);
      CObject3D l_O3D;
      l_O3D.SetMat44(l_mChildTransform);

      bool l_bIsOk = l_pChild->Init(l_it->emiter, l_O3D, l_vChildBox);
      if(l_bIsOk)
      {
        m_ChildEmiters.push_back(l_pChild);
        GetBoundingBox()->Adjust(*l_pChild->GetBoundingBox());
      }
      else
      {
        delete l_pChild;
        SetOk(false);
        break;
      }
    }
  }

  if(!IsOk())
  {
    Release();
  }
  else if(!m_InstancedData.IsOk())
  {
    bool l_bIsOk = m_InstancedData.Init(CORE->GetRenderManager(), m_iMaxParticles);
    SetOk(l_bIsOk);
  }

  GetBoundingSphere()->Init(*GetBoundingBox());
  if(IsOk())
    CORE->GetPortalManager()->InsertEmiter(this);
  return IsOk();
}

void CEmiterInstance::Update(float _fDeltaTime)
{
  assert(IsOk());

  if(!m_bActive) return;

  if(!m_pInRoom->GetNeightbour()) return;
  //mirem si el delta time és massa gran, per no fer actualitzacions massa a saco, les capem a un min de framerate
  if(_fDeltaTime > MAX_PARTICLE_DELTA_TIME) _fDeltaTime = MAX_PARTICLE_DELTA_TIME;


  
  if(m_pObjectReference)
  {
    //l_mTransform = _mTransform * m_pObjectReference->GetMat44();
    Mat44f m = m_pObjectReference->GetMat44();
    m = m * m_ObjectOffset.GetMat44();
    SetMat44(m);
    //l_mTransform = l_mTransform * GetMat44();
  }



  bool l_bBBModified = false;

  if(m_bBillboardMode)
  {
    
    CRenderManager* l_pRM = CORE->GetRenderManager();
    SParticleRenderInfo* l_pInstanceBuffer = m_InstancedData.GetBuffer(1, l_pRM);
    assert(l_pInstanceBuffer);

    m_Billboard.Update(_fDeltaTime);
    m_Billboard.FillRenderInfo(l_pInstanceBuffer[0]);
    
    bool l_bResult = m_InstancedData.SetData(l_pInstanceBuffer, 1, l_pRM);
    assert(l_bResult);
  }else if(m_bIsSimple)
  {
    CRenderManager* l_pRM = CORE->GetRenderManager();
    SParticleRenderInfo* l_pInstanceBuffer = m_InstancedData.GetBuffer(m_iMaxParticles, l_pRM);
    assert(l_pInstanceBuffer);
    const CSimpleEmiterCore *l_pEmiterCore = dynamic_cast<const CSimpleEmiterCore*>(m_pEmiterCore);

    //actualitzem les partícules
    for(int i = 0; i < m_iActiveParticles; ++i)
    {
      CParticle* l_pParticle = m_RecyclingParticles.GetAt(m_iaParticles[i]);
      if(l_pParticle->Update(_fDeltaTime))
      {
        //la partícula encara està viva, omplim la informació al buffer de rendetizatge.
        l_pParticle->FillRenderInfo(l_pInstanceBuffer[i]);
        l_bBBModified = GetBoundingBox()->Adjust(l_pParticle->GetPosition()) | l_bBBModified;
      }
      else
      {
        m_RecyclingParticles.Free(m_iaParticles[i]);
        m_iActiveParticles--;
        m_iaParticles[i] = m_iaParticles[m_iActiveParticles];
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
        m_fTimeToAwakeOrSleep += l_pEmiterCore->GetAwakeTime();
      }
      else
      {
        m_fTimeToAwakeOrSleep += l_pEmiterCore->GetSleepTime();
      }
    }

    // si ha passat prou temps com per crear una partícula nova
    while(_fDeltaTime > m_fTimeToNextParticle)
    {

      _fDeltaTime -= m_fTimeToNextParticle;

      float l_fMultiplier = l_pEmiterCore->GetEmitAbsolute()? 1 : m_fVolume;

      m_fTimeToNextParticle = 1.f / (l_pEmiterCore->GetEmitRate() * l_fMultiplier); //carreguem el temps fins la següent partícula

      if(m_bAwake && m_iActiveParticles < m_iMaxParticles) //comprovem que el buffer no hagi quedat ple
      {
        int l_iParticle = m_RecyclingParticles.NewIndex(); //agafem una partícula del buffer
        CParticle* l_pParticle = m_RecyclingParticles.GetAt(l_iParticle);

        //creem la partícula dintre de la caixa inicial
        Vect3f l_vRnd(Random01(),Random01(),Random01());
        Vect3f l_v_1_Minus_Rnd(1.f - l_vRnd.x, 1.f - l_vRnd.y, 1.f - l_vRnd.z);
        Vect3f l_vInitialPosition = ( l_v_1_Minus_Rnd.Scale(m_vMinVolume) ) + ( l_vRnd.Scale(m_vMaxVolume) );

        //inicialitzem la partícula
        l_pParticle->Init(l_pEmiterCore, l_vInitialPosition, false);
        m_iaParticles[m_iActiveParticles] = l_iParticle;

        //actualitzem aquesta partícula fins al final d'aquest frame
        if(l_pParticle->Update(_fDeltaTime))
        {
          //la partícula encara està viva, omplim la informació al buffer de rendetizatge.
          l_pParticle->FillRenderInfo(l_pInstanceBuffer[m_iActiveParticles]);
          l_bBBModified = GetBoundingBox()->Adjust(l_pParticle->GetPosition()) | l_bBBModified;
        }
        else
        {
          m_RecyclingParticles.Free(m_iaParticles[m_iActiveParticles]);
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
  else
  {
    vector<CEmiterInstance*>::iterator l_it  = m_ChildEmiters.begin();
    vector<CEmiterInstance*>::iterator l_end = m_ChildEmiters.end();

    for(; l_it != l_end; ++l_it)
    {
      (*l_it)->Update(_fDeltaTime);
      l_bBBModified = GetBoundingBox()->Adjust(*(*l_it)->GetBoundingBox()) | l_bBBModified;
    }
  }
  if(l_bBBModified)
  {
    GetBoundingSphere()->Init(*GetBoundingBox());
  }
}



void CEmiterInstance::Render(CRenderManager* _pRM, CEffect* _pEffect, const Mat44f& _mTransform)
{
  assert(IsOk());

  if(!m_bActive)
    return;

  Mat44f l_mTransform = _mTransform * GetMat44();
  
  if(m_bBillboardMode)
  {
    _pRM->SetTransform(l_mTransform);

    LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
    l_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA  | 1 ));
    l_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1 ));

    bool l_bResult = m_InstancedData.SetStreamSource(_pRM, 1);
    assert(l_bResult);// ---

    const CSimpleEmiterCore *l_pEmiterCore = dynamic_cast<const CSimpleEmiterCore*>(m_pEmiterCore);
    
    CMaterial *l_pMaterial = l_pEmiterCore->GetMaterial();
    if(_pEffect && l_pMaterial)
    {
      l_pMaterial->Activate(_pEffect->GetTextureMask());

      int l_iMaterialType = l_pMaterial->GetMaterialType();
      
      CEffectManager* l_pEM = CORE->GetEffectManager();
      l_pEM->SetGlow((l_iMaterialType & GLOW_MATERIAL_MASK) > 0);
      if(l_iMaterialType & GLOW_MATERIAL_MASK)
      {
        l_pEM->SetGlowIntensity(l_pMaterial->GetGlowIntensity());
      }

      l_pEM->SetSpecular((l_iMaterialType & SPECULARMAP_MATERIAL_MASK) > 0);
      l_pEM->SetSpecularParams(l_pMaterial->GetGlossiness(), l_pMaterial->GetSpecularFactor());
      l_pEM->SetEnvironmentIntensity(l_pMaterial->GetEnvironmentIntensity());
      l_pEM->SetSpriteSize(l_pMaterial->GetSpriteSize());
        
      l_pEM->LoadShaderData(_pEffect);
    }
    _pRM->GetParticleVertexs()->Render(_pRM, _pEffect);

  } else if(m_bIsSimple)
  {
    if(m_iActiveParticles == 0)
      return;
    const CSimpleEmiterCore *l_pEmiterCore = dynamic_cast<const CSimpleEmiterCore*>(m_pEmiterCore);
    CEffectManager* l_pEM = CORE->GetEffectManager();
    assert(l_pEM && l_pEM->IsOk());

    _pRM->SetTransform(l_mTransform);
  
    // renderitzem -----------------------------------------------------------------------------------------------------------------


    LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

    // Fem els set stream sources
    l_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA  | m_iActiveParticles));

    l_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));

    bool l_bResult = m_InstancedData.SetStreamSource(_pRM, 1);
    assert(l_bResult);// ---


    CMaterial *l_pMaterial = l_pEmiterCore->GetMaterial();
    if(_pEffect && l_pMaterial)
    {
      l_pMaterial->Activate(_pEffect->GetTextureMask());

      int l_iMaterialType = l_pMaterial->GetMaterialType();

      l_pEM->SetGlow((l_iMaterialType & GLOW_MATERIAL_MASK) > 0);
      if(l_iMaterialType & GLOW_MATERIAL_MASK)
      {
        l_pEM->SetGlowIntensity(l_pMaterial->GetGlowIntensity());
      }

      l_pEM->SetSpecular((l_iMaterialType & SPECULARMAP_MATERIAL_MASK) > 0);
      l_pEM->SetSpecularParams(l_pMaterial->GetGlossiness(), l_pMaterial->GetSpecularFactor());
      l_pEM->SetEnvironmentIntensity(l_pMaterial->GetEnvironmentIntensity());
      l_pEM->SetSpriteSize(l_pMaterial->GetSpriteSize());
        
      l_pEM->LoadShaderData(_pEffect);
    }
    _pRM->GetParticleVertexs()->Render(_pRM, _pEffect);
  }
  else
  {
    vector<CEmiterInstance*>::iterator l_it  = m_ChildEmiters.begin();
    vector<CEmiterInstance*>::iterator l_end = m_ChildEmiters.end();

    for(; l_it != l_end; ++l_it)
    {
      (*l_it)->Render(_pRM, _pEffect, l_mTransform);
    }
  }
  
}


void CEmiterInstance::DebugRender(CRenderManager* _pRM, const Mat44f& _mTransform, bool _bDebugRenderBoundings)
{
  assert(IsOk());
  
  Mat44f l_mTransform = _mTransform * GetMat44();

  _pRM->SetTransform(l_mTransform);
  if(m_bIsSimple)
  {
    if(m_bActive)
      _pRM->DrawCube(m_vMaxVolume - m_vMinVolume, colGREEN);
    else
      _pRM->DrawCube(m_vMaxVolume - m_vMinVolume, colRED);
  }
  else
  {
    vector<CEmiterInstance*>::iterator l_it  = m_ChildEmiters.begin();
    vector<CEmiterInstance*>::iterator l_end = m_ChildEmiters.end();

    for(; l_it != l_end; ++l_it)
    {
      (*l_it)->DebugRender(_pRM, l_mTransform,_bDebugRenderBoundings);
    }
  }

  //_pRM->DrawCube  (GetBoundingBox   ()->GetMiddlePoint(), GetBoundingBox   ()->GetDimension(), colMAGENTA);
  if(_bDebugRenderBoundings)
    _pRM->DrawSphere(GetBoundingSphere()->GetMiddlePoint(), GetBoundingSphere()->GetRadius   (), colMAGENTA, 10);
}

void CEmiterInstance::Release()
{
  m_pObjectReference = 0;
  vector<CEmiterInstance*>::iterator l_it  = m_ChildEmiters.begin();
  vector<CEmiterInstance*>::iterator l_end = m_ChildEmiters.end();

  for(; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  m_ChildEmiters.clear();

  m_RecyclingParticles.DeleteAllElements();
  CORE->GetPortalManager()->RemoveEmiter(this);

  CHECKED_DELETE_ARRAY(m_iaParticles);
}


const vector<CMaterial*>& CEmiterInstance::GetMaterials() const
{
  static vector<CMaterial*> a;
  return a;
}
