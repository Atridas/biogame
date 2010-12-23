#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"

#include "Core.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "VertexsStructs.h"
#include <cal3d/cal3d.h>


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

void CAnimatedInstanceModel::Render(CRenderManager *_pRM)
{
  //TODO
  RenderModelBySoftware(_pRM);
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
  vector<CTexture *>::iterator l_itTexture = m_vTextureList.begin();

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
        (*l_itTexture)->Activate(0);
        l_itTexture++;
		   
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

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *_pAnimatedCoreModel)
{
  m_pAnimatedCoreModel = _pAnimatedCoreModel;
  CalCoreModel* l_pCoreModel = m_pAnimatedCoreModel->GetCoreModel();
  m_pCalModel = new CalModel(l_pCoreModel);

  m_iNumVtxs = 0;
  m_iNumFaces = 0;

  int l_iMeshId;
  for(l_iMeshId = 0; l_iMeshId < l_pCoreModel->getCoreMeshCount(); l_iMeshId++)
  {
    m_pCalModel->attachMesh(l_iMeshId);
    CalCoreMesh* l_pCoreMesh = l_pCoreModel->getCoreMesh(l_iMeshId);

    for(int l_iSubMeshId = 0; l_iSubMeshId < l_pCoreMesh->getCoreSubmeshCount(); l_iSubMeshId++)
    {
      m_iNumVtxs += l_pCoreMesh->getCoreSubmesh(l_iSubMeshId)->getVertexCount();
      m_iNumFaces += l_pCoreMesh->getCoreSubmesh(l_iSubMeshId)->getFaceCount();
    }
  }

  InitD3D(RENDER_MANAGER);

  SetOk(true);
}

void CAnimatedInstanceModel::InitD3D(CRenderManager *_pRM)
{
  if(!LoadVertexBuffer(_pRM))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel:: LoadVertexBuffer retorna false.");
  }
  LoadTextures(_pRM);
}

void CAnimatedInstanceModel::Release()
{
  m_vTextureList.clear();
  CHECKED_DELETE(m_pCalModel);
}

void CAnimatedInstanceModel::ExecuteAction(int _iId, float _fTime)
{
  m_pCalModel->getMixer()->executeAction(_iId,0,_fTime);
  m_iCurrentAnimationId = _iId;
  m_fCurrentAnimationTime = 0;
}

void CAnimatedInstanceModel::BlendCycle(int _iId, float _fTime)
{
  m_pCalModel->getMixer()->blendCycle(_iId,1,_fTime);
  m_iCurrentCycle = _iId;
}

void CAnimatedInstanceModel::ClearCycle(float _fTime)
{
  m_pCalModel->getMixer()->clearCycle(m_iCurrentCycle,_fTime);
}

