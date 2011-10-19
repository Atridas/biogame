#include "AnimatedInstanceModel.h"

#include "Core.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "VertexsStructs.h"
#include <cal3d/cal3d.h>

#include "IndexedVertexs.h"
#include "RenderableVertexs.h"
#include "Material.h"
#include "EffectManager.h"

#include <boost/foreach.hpp>

#define foreach         BOOST_FOREACH

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *_pAnimatedCoreModel)
{
  if(_pAnimatedCoreModel != NULL)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedInstanceModel::Initialize Inicialitzant un AnimatedCoreModel.");

    m_pAnimatedCoreModel = _pAnimatedCoreModel;
    CalCoreModel* l_pCoreModel = m_pAnimatedCoreModel->GetCoreModel();
    m_pCalModel = new CalModel(l_pCoreModel);

    //m_iNumVtxs = 0;
    //m_iNumFaces = 0;

    int l_iMeshCount = l_pCoreModel->getCoreMeshCount();
    for(int l_iMeshId = 0; l_iMeshId < l_iMeshCount; l_iMeshId++)
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedInstanceModel::Initialize Afegint mesh %d.", l_iMeshId);
      m_pCalModel->attachMesh(l_iMeshId);
      //CalCoreMesh* l_pCoreMesh = l_pCoreModel->getCoreMesh(l_iMeshId);
      //int l_iSubmeshCount = l_pCoreMesh->getCoreSubmeshCount();
      //for(int l_iSubMeshId = 0; l_iSubMeshId < l_iSubmeshCount; l_iSubMeshId++)
      //{
      //  m_iNumVtxs += l_pCoreMesh->getCoreSubmesh(l_iSubMeshId)->getVertexCount();
      //  m_iNumFaces += l_pCoreMesh->getCoreSubmesh(l_iSubMeshId)->getFaceCount();
      //}
    }

    InitD3D(RENDER_MANAGER);

    //animated vertex technique
    //string l_szTechniqueName = RENDER_MANAGER->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(TCAL3D_HW_VERTEX::GetVertexType());
    //m_pEffectTechnique = RENDER_MANAGER->GetEffectManager()->GetEffectTechnique(l_szTechniqueName);

    SetOk(true);


    m_szAnimationState = _pAnimatedCoreModel->m_szDefaultAnimationState;
    if(m_szAnimationState != "")
    {
      map<string, SAnimationState>::iterator l_it = _pAnimatedCoreModel->m_AnimationStates.find(m_szAnimationState);
      if(l_it != _pAnimatedCoreModel->m_AnimationStates.end())
      {
        foreach(const SCycle& cycle, l_it->second.Cycles)
        {
          float l_fWeight = (cycle.bFromParameter)? m_fAnimationParameter : ((cycle.bFromComplementaryParameter)? 1 - m_fAnimationParameter : 1);
          l_fWeight *= cycle.fWeight;
          m_pCalModel->getMixer()->blendCycle(cycle.iId,l_fWeight,l_it->second.fDefaultFadeIn);
        }

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "CAnimatedInstanceModel::Initialize Invalid default animation state %s", m_szAnimationState.c_str());
        m_szAnimationState = "";
      }
    }

  }else
    LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel::Initialize L'AnimatedCoreModel proporcionat es NULL.");
}

bool CAnimatedInstanceModel::LoadVertexBuffer(CRenderManager *_pRM)
{
  bool isOk = true;

  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  if(FAILED(l_pDevice->CreateVertexBuffer(sizeof(SNORMALTEXTUREDVERTEX)*m_iNumVtxs,D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,SNORMALTEXTUREDVERTEX::GetVertexType(),D3DPOOL_DEFAULT,&m_pVB,NULL)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel::LoadVertexBuffer CreateVertexBuffer ha fallat");
    isOk = false;
  }

  _D3DFORMAT l_IndexFormat = (sizeof(CalIndex) == 2)? D3DFMT_INDEX16 : D3DFMT_INDEX32;

  if(FAILED(l_pDevice->CreateIndexBuffer(sizeof(CalIndex)*m_iNumFaces*3,0,l_IndexFormat,D3DPOOL_DEFAULT,&m_pIB,NULL)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel::LoadVertexBuffer CreateIndexBuffer ha fallat");
    isOk = false;
  }

  return isOk;
}

void CAnimatedInstanceModel::Render(CRenderManager *_pRM, const vector<CEffect*>& _vEffects)
{
  CalHardwareModel* l_pCalHardwareModel = m_pAnimatedCoreModel->GetCalHardwareModel();

  vector<CEffect*>::const_iterator l_ItEffect = _vEffects.begin();
  vector<CMaterial*>::const_iterator l_itMaterial = m_pAnimatedCoreModel->GetMaterials().begin();
   vector<CMaterial*>::const_iterator l_itMaterialEnd = m_pAnimatedCoreModel->GetMaterials().end();

  CMaterial* l_pMaterial = 0;
  CEffectManager* l_pEM = CORE->GetEffectManager();

  CalSkeleton* l_pSkeleton = m_pCalModel->getSkeleton();

  if(l_pCalHardwareModel)
  {
    for(int l_iHardwareMeshId=0; l_iHardwareMeshId < l_pCalHardwareModel->getHardwareMeshCount(); l_iHardwareMeshId++)
    {
      l_pCalHardwareModel->selectHardwareMesh(l_iHardwareMeshId);

      CEffect* l_pEffect = 0;

      if(l_ItEffect != _vEffects.end())
      {
        l_pEffect = *l_ItEffect;
        ++l_ItEffect;
      }

      if(l_itMaterial != l_itMaterialEnd)
      {
        l_pMaterial = *l_itMaterial;
        ++l_itMaterial;
      }

      if(l_pEffect && l_pMaterial)
      {
        l_pMaterial->Activate(l_pEffect->GetTextureMask());

        int l_iMaterialType = l_pMaterial->GetMaterialType();

        l_pEM->SetGlow((l_iMaterialType & GLOW_MATERIAL_MASK) > 0);
        if(l_iMaterialType & GLOW_MATERIAL_MASK)
        {
          l_pEM->SetGlowIntensity(l_pMaterial->GetGlowIntensity());
        }

        l_pEM->SetSpecular((l_iMaterialType & SPECULARMAP_MATERIAL_MASK) > 0);
        l_pEM->SetSpecularParams(l_pMaterial->GetGlossiness(), l_pMaterial->GetSpecularFactor());
        l_pEM->SetEnvironmentIntensity(l_pMaterial->GetEnvironmentIntensity());
        l_pEM->SetSkeleton(l_pSkeleton,l_pCalHardwareModel);

        l_pEM->LoadShaderData(l_pEffect);
      }
      
      ((CIndexedVertexs<TCAL3D_HW_VERTEX>*)m_pAnimatedCoreModel->GetRenderableVertexs())->
        Render( _pRM,
                l_pEffect,
                l_pCalHardwareModel->getBaseVertexIndex(),
			          0,
			          l_pCalHardwareModel->getVertexCount(),
			          l_pCalHardwareModel->getStartIndex(),
			          l_pCalHardwareModel->getFaceCount());
    }
  }
}


void CAnimatedInstanceModel::RenderModelBySoftware(CRenderManager *_pRM)
{
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

  // get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = m_pCalModel->getRenderer();

  // begin the rendering loop
  if(!pCalRenderer->beginRendering()) return;

  int l_iVBCursor=0;
  int l_iIBCursor=0;

  DWORD dwVBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags=D3DLOCK_NOOVERWRITE;

  l_pDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(SNORMALTEXTUREDVERTEX) );
  l_pDevice->SetFVF(SNORMALTEXTUREDVERTEX::GetFVF());
  l_pDevice->SetIndices(m_pIB);
  
  // get the number of meshes
  int l_iMeshCount;
  l_iMeshCount = pCalRenderer->getMeshCount();

  //Això pot petar!!!
  //vector<CTexture*>::iterator l_itTexture = m_vTextureList.begin();

  // render all meshes of the model
  int l_iMeshId;
  for(l_iMeshId = 0; l_iMeshId < l_iMeshCount; l_iMeshId++)
  {
    // get the number of submeshes
    int l_iSubmeshCount;
    l_iSubmeshCount = pCalRenderer->getSubmeshCount(l_iMeshId);

    // render all submeshes of the mesh
    int l_iSubmeshId;
    for(l_iSubmeshId = 0; l_iSubmeshId < l_iSubmeshCount; l_iSubmeshId++)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(l_iMeshId, l_iSubmeshId))
      {
        //m_pAnimatedCoreModel->GetMaterial(l_iSubmeshId)->ActivateTextures(_pRM);
		   
		    // Get vertexbuffer from the model		  		  
		    SNORMALTEXTUREDVERTEX* l_pVertices;

		    m_pVB->Lock(l_iVBCursor*sizeof(SNORMALTEXTUREDVERTEX), pCalRenderer->getVertexCount()*sizeof(SNORMALTEXTUREDVERTEX), (void**)&l_pVertices, dwVBLockFlags);

        int l_iVertexCount = pCalRenderer->getVerticesNormalsAndTexCoords((float*)l_pVertices);
		    m_pVB->Unlock();
		  
		    CalIndex *l_pMeshFaces;

        int l_iFaceCount;

		    m_pIB->Lock(l_iIBCursor* 3*sizeof(CalIndex), pCalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&l_pMeshFaces,dwIBLockFlags);

		    l_iFaceCount = pCalRenderer->getFaces(l_pMeshFaces);
		    m_pIB->Unlock();


        l_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
          
		    l_pDevice->DrawIndexedPrimitive(
			    D3DPT_TRIANGLELIST,
			    l_iVBCursor,
			    0,
			    l_iVertexCount,
			    l_iIBCursor*3,
			    l_iFaceCount
			  );

		    l_iVBCursor+=l_iVertexCount;
		    l_iIBCursor+=l_iFaceCount;

		    dwIBLockFlags=D3DLOCK_NOOVERWRITE;
		    dwVBLockFlags=D3DLOCK_NOOVERWRITE;

      }
    }
  }


  // end the rendering
  pCalRenderer->endRendering();
}

void CAnimatedInstanceModel::Update(float _fElapsedTime)
{
  m_pCalModel->update(_fElapsedTime);
  m_fCurrentAnimationTime += _fElapsedTime;
}

void CAnimatedInstanceModel::InitD3D(CRenderManager *_pRM)
{
  //if(!LoadVertexBuffer(_pRM))
  //{
  //  LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel:: LoadVertexBuffer retorna false.");
  //}
  //LoadTextures(_pRM);
}

void CAnimatedInstanceModel::Release()
{
  CHECKED_DELETE(m_pCalModel);
}

void CAnimatedInstanceModel::ExecuteAction(int _iId, float _fTime, float _fWeight, bool _bAutoLock)
{
  m_pCalModel->getMixer()->removeAction(_iId);
  m_pCalModel->getMixer()->executeAction(_iId,0,_fTime,_fWeight,_bAutoLock);
  m_iCurrentAnimationId = _iId;
  m_fCurrentAnimationTime = 0;
}

void CAnimatedInstanceModel::RemoveAction(int _iId)
{
  m_pCalModel->getMixer()->removeAction(_iId);
}

void CAnimatedInstanceModel::BlendCycle(int _iId, float _fTime, float _fWeight)
{
  m_pCalModel->getMixer()->blendCycle(_iId,_fWeight,_fTime);
  m_iCurrentCycle = _iId;
}

void CAnimatedInstanceModel::BlendCycle(const std::string &AnimationName, float _fTime, float _fWeight)
{
  int l_AnimationId=m_pAnimatedCoreModel->GetCoreModel()->getCoreAnimationId(AnimationName);
  BlendCycle(l_AnimationId, _fTime, _fWeight);
}

int CAnimatedInstanceModel::GetAnimationId(const std::string &AnimationName) const
{
  return m_pAnimatedCoreModel->GetCoreModel()->getCoreAnimationId(AnimationName);
}

void CAnimatedInstanceModel::ClearCycle(float _fTime)
{
  m_pCalModel->getMixer()->clearCycle(m_iCurrentCycle,_fTime);
}


void CAnimatedInstanceModel::ClearCycle(int _iId, float _fTime)
{
  m_pCalModel->getMixer()->clearCycle(_iId,_fTime);
}

void CAnimatedInstanceModel::ClearCycle(const std::string &AnimationName, float _fTime)
{
  int l_iId = m_pAnimatedCoreModel->GetCoreModel()->getCoreAnimationId(AnimationName);
  m_pCalModel->getMixer()->clearCycle(l_iId,_fTime);
}

int CAnimatedInstanceModel::GetAnimationCount()
{
  return m_pAnimatedCoreModel->GetAnimationCount();
}



void CAnimatedInstanceModel::SetAnimationState(const string& _szAnimationState)
{
  if(_szAnimationState == m_szAnimationState)
  {
    return;
  }

  map<string,SAnimationState>::const_iterator l_itNextAnimationState = m_pAnimatedCoreModel->m_AnimationStates.find(_szAnimationState);

  if(l_itNextAnimationState == m_pAnimatedCoreModel->m_AnimationStates.end())
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedInstanceModel::SetAnimationState intentant canviar a un estat inexistent \"%s\"", _szAnimationState.c_str());
    return;
  }

  const SAnimationState& l_NextAnimationState = l_itNextAnimationState->second;

  float l_fFade = l_NextAnimationState.fDefaultFadeIn;

  const set<SCycle>& l_CyclesToActivate = l_NextAnimationState.Cycles;

  if(m_szAnimationState != "")
  {
    SAnimationState &l_PrevAnimationState = m_pAnimatedCoreModel->m_AnimationStates.find(m_szAnimationState)->second;

    foreach(const SAction& l_Action, l_PrevAnimationState.OnExit)
    {
      ExecuteAction(l_Action);
    }

    const map<string, SAnimationChange>& l_ChangesFromPrev = m_pAnimatedCoreModel->m_AnimationChanges[m_szAnimationState];
    map<string, SAnimationChange>::const_iterator l_it = l_ChangesFromPrev.find(_szAnimationState);

    if(l_it != l_ChangesFromPrev.end())
    {
      l_fFade = ExecuteChange(l_it->second);
    }
    else
    {
      l_fFade += l_PrevAnimationState.fDefaultFadeOut;
      l_fFade *= .5f;
    }

    const set<SCycle>& l_PrevCycles = l_PrevAnimationState.Cycles;

    foreach(const SCycle& l_Cycle, l_PrevAnimationState.Cycles)
    {
      if(l_CyclesToActivate.find(l_Cycle) == l_CyclesToActivate.end())
      {
        ClearCycle(l_Cycle, l_fFade);
      }
    }
  }


  foreach(const SAction& l_Action, l_NextAnimationState.OnEnter)
  {
    ExecuteAction(l_Action);
  }

  foreach(const SCycle& l_Cycle, l_CyclesToActivate)
  {
    BlendCycle(l_Cycle, l_fFade);
  }

  m_szAnimationState = _szAnimationState;
}

void CAnimatedInstanceModel::SetAnimationParameter(float _fAnimationParameter) 
{
  m_fAnimationParameter = _fAnimationParameter;
  
  const SAnimationState& l_AnimationState = m_pAnimatedCoreModel->m_AnimationStates.find(m_szAnimationState)->second;

  
  foreach(const SCycle& l_Cycle, l_AnimationState.Cycles)
  {
    float l_fFactor;
    if(l_Cycle.bFromParameter)
    {
      l_fFactor = m_fAnimationParameter * l_Cycle.fWeight;
    } else if(l_Cycle.bFromComplementaryParameter)
    {
      l_fFactor = (1.f - m_fAnimationParameter) * l_Cycle.fWeight;
    } else
    {
      continue;
    }
    
    float l_fBlendTime = (l_Cycle.fFadeOnChange < 0)? 0 : l_Cycle.fFadeOnChange;

    m_pCalModel->getMixer()->blendCycle(l_Cycle.iId,
                                        l_fFactor,
                                        l_fBlendTime);
  }
};

//retorna el fade
float CAnimatedInstanceModel::ExecuteChange(const SAnimationChange& _AnimationChange)
{
  foreach(const SAction& l_Action, _AnimationChange.Actions)
  {
    ExecuteAction(l_Action);
  }

  return _AnimationChange.fFade;
}

void  CAnimatedInstanceModel::ExecuteAction(const SAction& _Action)
{
  CalMixer *l_pMixer = m_pCalModel->getMixer();
  if(_Action.bStop)
  {
    l_pMixer->removeAction(_Action.iId);
  }
  else
  {
    float l_fWeight;
    if(_Action.bFromParameter)
    {
      l_fWeight = m_fAnimationParameter * _Action.fWeight;
    } else if(_Action.bFromComplementaryParameter)
    {
      l_fWeight = (1.f - m_fAnimationParameter) * _Action.fWeight;
    } else
    {
      l_fWeight = _Action.fWeight;
    }
    
    float l_fBlendIn  = (_Action.fFadeIn  < 0)? 0 : _Action.fFadeIn;
    float l_fBlendOut = (_Action.fFadeOut < 0)? 0 : _Action.fFadeOut;

    l_pMixer->executeAction(_Action.iId, l_fBlendIn, l_fBlendOut, l_fWeight, _Action.bBlock);
  }
}

void  CAnimatedInstanceModel::BlendCycle(const SCycle& _Cycle, float _fBlendTime)
{
  float l_fWeight;
  if(_Cycle.bFromParameter)
  {
    l_fWeight = m_fAnimationParameter * _Cycle.fWeight;
  } else if(_Cycle.bFromComplementaryParameter)
  {
    l_fWeight = (1.f - m_fAnimationParameter) * _Cycle.fWeight;
  } else
  {
    l_fWeight = _Cycle.fWeight;
  }

  _fBlendTime = (_fBlendTime < 0)? 0 : _fBlendTime;

  m_pCalModel->getMixer()->blendCycle(_Cycle.iId, l_fWeight, _fBlendTime);
}

void  CAnimatedInstanceModel::ClearCycle(const SCycle& _Cycle, float _fBlendTime)
{
  m_pCalModel->getMixer()->clearCycle(_Cycle.iId, _fBlendTime);
}
