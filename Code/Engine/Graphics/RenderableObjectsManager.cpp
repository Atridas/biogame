#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "InstanceMesh.h"
#include <XML/XMLTreeNode.h>
#include "RenderableAnimatedInstanceModel.h"

void CRenderableObjectsManager::Update(float _fElapsedTime)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
    m_RenderableObjects[i]->Update(_fElapsedTime);
}

void CRenderableObjectsManager::Render(CRenderManager *RM)
{
  //renderitzar només els visibles
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
    if(m_RenderableObjects[i]->GetVisible())
      m_RenderableObjects[i]->Render(RM);
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
      
      //Log d'inserció
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


void CRenderableObjectsManager::SetAllVisible(bool _bVisible, CRenderableObject* _pRO)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
  {
      m_RenderableObjects[i]->SetVisible(_bVisible);
  }

  _pRO->SetVisible(true);
}