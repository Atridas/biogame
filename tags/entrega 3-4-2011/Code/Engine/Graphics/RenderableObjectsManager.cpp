#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "InstanceMesh.h"
#include "StaticMesh.h"
#include <XML/XMLTreeNode.h>
#include "RenderableAnimatedInstanceModel.h"
#include "InstancedData.h"
#include "Core.h"

struct SHWIntancedMeshes
{
  CInstancedData<D3DMATRIX> m_mWorldMats;
  vector<CInstanceMesh*> m_vInstances;
};

void CRenderableObjectsManager::Release()
{
  map<const CStaticMesh*,SHWIntancedMeshes*>::iterator l_it = m_mapHWStaticInstances.begin();
  while(l_it != m_mapHWStaticInstances.end())
  {
    CHECKED_DELETE(l_it->second);
    l_it++;
  }
  m_mapHWStaticInstances.clear();
  CMapManager<CRenderableObject>::Release();
}

void CRenderableObjectsManager::Update(float _fElapsedTime)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
    m_RenderableObjects[i]->Update(_fElapsedTime);
}

void CRenderableObjectsManager::Render(CRenderManager *_pRM)
{
  if(m_bRenderInstanced)
  {
    RenderHWInstanced(_pRM);
  } else {
    RenderOld(_pRM);
  }
}

void FillBuffer(SHWIntancedMeshes* _HWStaticInstances, CRenderManager* _pRM)
{
  // Agafem les matrius -------------------------------------------------------------------------------
  D3DMATRIX* l_mBuffer = _HWStaticInstances->m_mWorldMats.GetBuffer(_HWStaticInstances->m_vInstances.size(), _pRM);

  vector<CInstanceMesh*>::const_iterator l_itInst,
                                         l_endInst = _HWStaticInstances->m_vInstances.cend();
  int cont = 0;
  for(l_itInst = _HWStaticInstances->m_vInstances.begin(); l_itInst != l_endInst; ++l_itInst)
  {
    l_mBuffer[cont] = (*l_itInst)->GetMat44().GetD3DXMatrix();
    cont++;
  }
  // Omplim el buffer ------------------------------------------------------------------------------
  bool result = _HWStaticInstances->m_mWorldMats.SetData(l_mBuffer, cont, _pRM);
  assert(result);// ---
}

void CRenderableObjectsManager::FillBuffers(CRenderManager* _pRM)
{
  map<const CStaticMesh*,SHWIntancedMeshes*>::iterator l_it, l_end;
  l_end = m_mapHWStaticInstances.end();
  for(l_it = m_mapHWStaticInstances.begin(); l_it != l_end; ++l_it)
  {
    SHWIntancedMeshes* l_HWStaticInstances = l_it->second;
      
    FillBuffer(l_HWStaticInstances, _pRM);
  }
}

void CRenderableObjectsManager::RenderHWInstanced(CRenderManager* _pRM)
{
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  //objectes est�tics
  {
    map<const CStaticMesh*,SHWIntancedMeshes*>::iterator l_it, l_end;
    l_end = m_mapHWStaticInstances.end();
    for(l_it = m_mapHWStaticInstances.begin(); l_it != l_end; ++l_it)
    {
      SHWIntancedMeshes* l_HWStaticInstances = l_it->second;
      
      int cont = l_HWStaticInstances->m_vInstances.size();

      // Fem els set stream sources
      l_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA  | cont));

      l_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));
      bool result = l_HWStaticInstances->m_mWorldMats.SetStreamSource(_pRM, 1);
      assert(result);// ---

      l_it->first->Render(_pRM, true);
    }
  }
  //Deixem els streams com els teniem
  l_pDevice->SetStreamSourceFreq(0, 1);
  l_pDevice->SetStreamSourceFreq(1, 1);

  //objectes animats
  {
    vector<CRenderableObject*>::iterator l_it, l_end;
    l_end = m_vAnimatedModels.end();
    for(l_it = m_vAnimatedModels.begin(); l_it != l_end; ++l_it)
    {
      if((*l_it)->GetVisible())
      {
        (*l_it)->Render(_pRM);
      }
    }
  }
}

void CRenderableObjectsManager::RenderOld(CRenderManager* _pRM)
{
  //renderitzar nom�s els visibles
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
  {
    if(m_RenderableObjects[i]->GetVisible())
    {
      m_RenderableObjects[i]->Render(_pRM);
    }
    //if(i > 25) return;
  }
}

//TODO
CRenderableObject* CRenderableObjectsManager::AddMeshInstance(
                                                      const string& _szCoreMeshName,
                                                      const string& _szInstanceName)
{
  if(GetResource(_szInstanceName) != 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CRenderableObjectsManager:: CInstanceMesh amb nom repetit \"%s\"", _szCoreMeshName.c_str());
    return 0;
  }

  CInstanceMesh* l_pInstanceMesh = new CInstanceMesh(_szInstanceName);
  if(!l_pInstanceMesh->Init(_szCoreMeshName))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CRenderableObjectsManager:: No s'ha pogut carregar el CInstanceMesh \"%s\" de la core \"%s\"", _szInstanceName.c_str(), _szCoreMeshName.c_str());
    delete l_pInstanceMesh;
    return 0;
  }

  AddResource(_szInstanceName, l_pInstanceMesh);

  AddHWStaticInstance(l_pInstanceMesh);

  return l_pInstanceMesh;
}


//Add AnimatedModel
CRenderableObject* CRenderableObjectsManager::AddAnimatedModel(
                                                      const string& _szCoreModelName,
                                                      const string& _szInstanceName)
{
  if(GetResource(_szInstanceName) != 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CRenderableObjectsManager:: CAnimatedModel amb nom repetit \"%s\"", _szCoreModelName.c_str());
    return 0;
  }

  CRenderableAnimatedInstanceModel* l_pAnimatedModel = new CRenderableAnimatedInstanceModel(_szInstanceName);
  if(!l_pAnimatedModel->Init(_szCoreModelName))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CRenderableObjectsManager:: No s'ha pogut carregar el CRenderableAnimatedInstanceModel \"%s\" de la core \"%s\"", _szInstanceName.c_str(), _szCoreModelName.c_str());
    delete l_pAnimatedModel;
    return 0;
  }

  AddResource(_szInstanceName, l_pAnimatedModel);

  return l_pAnimatedModel;
}

void CRenderableObjectsManager::AddResource(const string& _szName, CRenderableObject* _pRenderableObject)
{
  CMapManager::AddResource(_szName,_pRenderableObject);
  m_RenderableObjects.push_back(_pRenderableObject);
}

bool CRenderableObjectsManager::Load(const string& _szFileName, bool _bReload)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderableObjectsManager::Load");

  if(m_vXMLFiles.find(_szFileName) != m_vXMLFiles.end())
  {
    if(!_bReload)
    {
      LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Carregant \"%s\" 2 vegades", _szFileName.c_str());
    }
  } else {
    m_vXMLFiles.insert( _szFileName );
  }

  CXMLTreeNode l_XMLObjects;
  if(!l_XMLObjects.LoadFile(_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
    return false;
  }

  int l_iNumObjects = l_XMLObjects.GetNumChildren();

  for(int i = 0; i < l_iNumObjects; i++)
  {
    string l_szName, l_szClass, l_szResource;

    CRenderableObject* l_pRenderableObject = 0;

    CXMLTreeNode l_XMLObject = l_XMLObjects(i);

    if(l_XMLObject.IsComment())
    {
      continue;
    }

    l_szName      = l_XMLObject.GetPszISOProperty("name" ,"");
    l_szClass     = l_XMLObject.GetPszISOProperty("class" ,"");
    l_szResource  = l_XMLObject.GetPszISOProperty("resource" ,"");

    l_pRenderableObject = GetResource(l_szName);
    if(!l_pRenderableObject)
    {
      if(l_szClass == "StaticMesh") 
      {
        l_pRenderableObject = AddMeshInstance(l_szResource, l_szName);

        if(l_pRenderableObject != 0  && l_pRenderableObject->IsOk())
        {
          l_pRenderableObject->InitFromXML(l_XMLObject);
          m_vMeshes.push_back(l_pRenderableObject);
        }

      } else if(l_szClass == "AnimatedModel") 
      {
        l_pRenderableObject = AddAnimatedModel(l_szResource, l_szName);

        if(l_pRenderableObject != 0 && l_pRenderableObject->IsOk())
        {
          l_pRenderableObject->InitFromXML(l_XMLObject);
          m_vAnimatedModels.push_back(l_pRenderableObject);
        }

      } else 
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Object: \"%s\" has unknown \"%s\" class", l_szName.c_str(), l_szClass.c_str());
      }
      
      //Log d'inserci�
      if(l_pRenderableObject != 0)
        LOGGER->AddNewLog(ELL_INFORMATION,"CRenderableObjectsManager:: Added object: \"%s\"", l_szName.c_str());
      else
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Object: \"%s\" not added", l_szName.c_str());

      
    }else{
      if(!_bReload)
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
      }
      l_pRenderableObject->InitFromXML(l_XMLObject);
    }
  }

  FillBuffers(CORE->GetRenderManager());
  return true;
}



bool CRenderableObjectsManager::Load(const vector<string>& _vXMLFiles)
{
  bool l_bResult = true;
  vector<string>::const_iterator l_end = _vXMLFiles.cend();
  for(vector<string>::const_iterator l_it = _vXMLFiles.cbegin(); l_it != l_end; ++l_it)
  {
    if(!Load(*l_it, false))
    {
      l_bResult = false;
    }
  }
  return l_bResult;
}

bool CRenderableObjectsManager::Load(const set<string>& _vXMLFiles)
{
  bool l_bResult = true;
  set<string>::const_iterator l_end = _vXMLFiles.cend();
  for(set<string>::const_iterator l_it = _vXMLFiles.cbegin(); l_it != l_end; ++l_it)
  {
    if(!Load(*l_it, true))
    {
      l_bResult = false;
    }
  }
  return l_bResult;
}


void CRenderableObjectsManager::SetAllVisibility(bool _bVisible)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
  {
      m_RenderableObjects[i]->SetVisible(_bVisible);
  }
}

void CRenderableObjectsManager::SetAllRenderBoundingBox(bool _bVisible)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
  {
      m_RenderableObjects[i]->SetRenderBoundingBox(_bVisible);
  }
}

void CRenderableObjectsManager::SetAllRenderBoundingSphere(bool _bVisible)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
  {
      m_RenderableObjects[i]->SetRenderBoundingSphere(_bVisible);
  }
}
/*
struct SHWIntancedMeshes
{
  CInstancedData<Mat44f> m_mWorldMats;
  vector<CInstanceMesh*> m_vInstances;
};
*/
void CRenderableObjectsManager::AddHWStaticInstance(CInstanceMesh* _pInstanceMesh)
{
  const CStaticMesh* l_pStaticMesh = _pInstanceMesh->GetStaticMesh();
  SHWIntancedMeshes* l_pHWIntancedMeshes;
  map<const CStaticMesh*,SHWIntancedMeshes*>::iterator find = m_mapHWStaticInstances.find(l_pStaticMesh);
  if(find == m_mapHWStaticInstances.end())
  {
    l_pHWIntancedMeshes = new SHWIntancedMeshes;
    m_mapHWStaticInstances[l_pStaticMesh] = l_pHWIntancedMeshes;
    l_pHWIntancedMeshes->m_mWorldMats.Init(CORE->GetRenderManager(), 1);
  } else {
    l_pHWIntancedMeshes = find->second;
  }

  l_pHWIntancedMeshes->m_vInstances.push_back(_pInstanceMesh);
}