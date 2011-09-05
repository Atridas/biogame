#include "EmiterManager.h"
#include "Core.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Effect.h"

CEmiterInstance* CEmiterManager::GetEmiter(const string &_szName) const
{
  return GetResource(_szName);
}

CEmiterInstance* CEmiterManager::CreateEmiter(const string &_szName, const string &_szCore, const CObject3D& _Position, const Vect3f& _vDimensions)
{
  CEmiterInstance* l_pEI = GetResource(_szName);

  if(l_pEI)
  {
    l_pEI->Reset(_szCore,_Position,_vDimensions);
  }
  else
  {
    l_pEI = new CEmiterInstance();
    if(l_pEI->Init(_szCore,_Position,_vDimensions))
    {
      AddResource(_szName, l_pEI);
    }
    else
    {
      LOGGER->AddNewLog(ELL_ERROR, "CEmiterManager::CreateEmiter Error a l'inicialitzar un emiter");
      LOGGER->AddNewLog(ELL_ERROR, "\tName: %s", _szName.c_str());
      LOGGER->AddNewLog(ELL_ERROR, "\tCore: %s", _szCore.c_str());
      CHECKED_DELETE( l_pEI );
    }
  }

  return l_pEI;
}

void CEmiterManager::DeleteEmiter(const string &_szName)
{
  CEmiterInstance* l_pEI = GetResource(_szName);
  if(l_pEI)
  {
    delete l_pEI;
    m_Resources.erase(_szName);
  }
}

void CEmiterManager::Update(float _fDeltaTime)
{
  TMapResource::iterator l_it  = m_Resources.begin();
  TMapResource::iterator l_end = m_Resources.end  ();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->Update(_fDeltaTime);
  }
}
  
void CEmiterManager::Render(CRenderManager* _pRM)
{
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();

  CEffectManager* l_pEM = CORE->GetEffectManager();

  assert(l_pEM && l_pEM->IsOk());
  CEffect* l_pPrevEffect = l_pEM->GetForcedStaticMeshEffect();
  if(!l_pPrevEffect)
  {
    CEffect* l_pEffect = l_pEM->GetEffect("Particle");
    l_pEM->SetForcedStaticMeshEffect(l_pEffect);


    TMapResource::iterator l_it  = m_Resources.begin();
    TMapResource::iterator l_end = m_Resources.end  ();

    for(; l_it != l_end; ++l_it)
    {
      l_it->second->Render(_pRM);
    }
  
    l_pd3dDevice->SetStreamSourceFreq(0, 1);
    l_pd3dDevice->SetStreamSourceFreq(1, 1);
    l_pEM->SetForcedStaticMeshEffect(0);
  }
}

void CEmiterManager::DebugRender(CRenderManager* _pRM, bool _bDebugRenderBoundings)
{
  TMapResource::iterator l_it  = m_Resources.begin();
  TMapResource::iterator l_end = m_Resources.end  ();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->DebugRender(_pRM,_bDebugRenderBoundings);
  }
}

void CEmiterManager::ResetEmiters()
{
  TMapResource::iterator l_it  = m_Resources.begin();
  TMapResource::iterator l_end = m_Resources.end  ();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->Reset();
  }
}
