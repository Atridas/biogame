#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"

#include "Core.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "VertexsStructs.h"
#include <cal3d/cal3d.h>

#include "IndexedVertexs.h"
#include "RenderableVertexs.h"
#include "Material.h"
#include "EffectManager.h"

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

/*
void CAnimatedInstanceModel::LoadTextures(CRenderManager *_pRM)
{
  m_vTextureList.clear();
  CTextureManager* l_pTM = _pRM->GetTextureManager();

  int l_iNumTextures = m_pAnimatedCoreModel->GetNumTextures();
  for(int l_iTexId = 0; l_iTexId < l_iNumTextures; l_iTexId++)
  {
    const string& l_szTexName = m_pAnimatedCoreModel->GetTextureName(l_iTexId);
    CTexture* tex = l_pTM->GetResource(l_szTexName);
    m_vTextureList.push_back(tex);
  }

}
*/

/*void CAnimatedInstanceModel::Render(CRenderManager *_pRM)
{
  //TODO
  //RenderModelBySoftware(_pRM);

  //return;

  CEffectManager* l_pEffectManager = _pRM->GetEffectManager();
  CEffectTechnique* l_pEffectTechnique = l_pEffectManager->GetAnimatedModelTechnique();

  if(l_pEffectTechnique==NULL)
    l_pEffectTechnique = m_pEffectTechnique;

  if(l_pEffectTechnique == NULL)
    return;

  //l_pEffectManager->SetWorldMatrix(GetTransform());
  CEffect* l_pEffect = l_pEffectTechnique->GetEffect();
  
  if(l_pEffect==NULL)
    return;

  LPD3DXEFFECT l_pd3dEffect = l_pEffect->GetD3DEffect();
  if(l_pd3dEffect)
  {
    l_pEffectTechnique->BeginRender();
    CalHardwareModel* l_pCalHardwareModel = m_pAnimatedCoreModel->GetCalHardwareModel();

    if(l_pCalHardwareModel)
    {
      D3DXMATRIX l_mTransformation[MAXBONES];

      for(int l_iHardwareMeshId=0; l_iHardwareMeshId < l_pCalHardwareModel->getHardwareMeshCount(); l_iHardwareMeshId++)
      {
        l_pCalHardwareModel->selectHardwareMesh(l_iHardwareMeshId);

        for(int l_iBoneId = 0; l_iBoneId < l_pCalHardwareModel->getBoneCount(); l_iBoneId++)
        {
          D3DXMatrixRotationQuaternion(&l_mTransformation[l_iBoneId],(CONST D3DXQUATERNION*)& l_pCalHardwareModel->getRotationBoneSpace(l_iBoneId, m_pCalModel->getSkeleton()));
          CalVector l_vTranslationBoneSpace = l_pCalHardwareModel->getTranslationBoneSpace(l_iBoneId, m_pCalModel->getSkeleton());
          l_mTransformation[l_iBoneId]._14 = l_vTranslationBoneSpace.x;
          l_mTransformation[l_iBoneId]._24 = l_vTranslationBoneSpace.y;
          l_mTransformation[l_iBoneId]._34 = l_vTranslationBoneSpace.z;
        }

        float l_mMatrix[MAXBONES*3*4];

        for(int l_iB = 0; l_iB < l_pCalHardwareModel->getBoneCount(); ++l_iB)
        {
          memcpy(&l_mMatrix[l_iB*3*4], &l_mTransformation[l_iB],sizeof(float)*3*4);
        }

        l_pd3dEffect->SetFloatArray(l_pEffect->m_pBonesParameter, (float *)l_mMatrix,(l_pCalHardwareModel->getBoneCount())*3*4);

        vector<CTexture*>::iterator l_itTexture = m_vTextureList.begin();

        (*l_itTexture)->Activate(0);

        ((CIndexedVertexs<TCAL3D_HW_VERTEX>*)m_pAnimatedCoreModel->GetRenderableVertexs())->Render( _pRM, l_pEffectTechnique);

      }
    }
  }
}*/

void CAnimatedInstanceModel::Render(CRenderManager *_pRM)
{
  CalHardwareModel* l_pCalHardwareModel = m_pAnimatedCoreModel->GetCalHardwareModel();
  CEffectManager* m_pEM = CORE->GetEffectManager();
  CMaterial* l_pMaterial = 0;

  const vector<CMaterial*>& l_vMaterials = m_pAnimatedCoreModel->GetMaterials();

  int l_iMaterialCount = l_vMaterials.size();

  if(l_pCalHardwareModel)
  {
    for(int l_iHardwareMeshId=0; l_iHardwareMeshId < l_pCalHardwareModel->getHardwareMeshCount(); l_iHardwareMeshId++)
    {
      if(l_iHardwareMeshId < l_iMaterialCount)
      {
        l_pMaterial = l_vMaterials[l_iHardwareMeshId];
      }else{
        l_pMaterial = l_vMaterials[l_iMaterialCount-1];
      }

      l_pCalHardwareModel->selectHardwareMesh(l_iHardwareMeshId);

      m_pEM->SetSkeleton(m_pCalModel->getSkeleton(),l_pCalHardwareModel);

      CEffect* l_pEffect = m_pEM->ActivateMaterial(l_pMaterial);
      
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

