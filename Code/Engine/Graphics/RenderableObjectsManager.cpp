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
    return 0;

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
                                                      const string& _szInstanceName,
                                                      const string& _szDefaultAnimation)
{
  if(GetResource(_szInstanceName) != 0)
    return 0;

  CRenderableAnimatedInstanceModel* l_pAnimatedModel = new CRenderableAnimatedInstanceModel(_szInstanceName);
  if(!l_pAnimatedModel->Init(_szCoreModelName,_szDefaultAnimation))
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
    string l_szName, l_szClass, l_szResource,l_szDefaultAnimation;
    Vect3f l_vPos;
    float l_fYaw;
    float l_fPitch;
    float l_fRoll;

    CRenderableObject* l_pRenderableObject = 0;

    CXMLTreeNode l_XMLObject = l_XMLObjects(i);

    l_szName      = l_XMLObject.GetPszISOProperty("name" ,"");
    l_szClass     = l_XMLObject.GetPszISOProperty("class" ,"");
    l_szResource  = l_XMLObject.GetPszISOProperty("resource" ,"");
    l_szDefaultAnimation = l_XMLObject.GetPszISOProperty("cycle" ,"");
    //l_szDefaultAnimation = l_XMLObject.GetIntProperty("cycle");

    l_vPos        = l_XMLObject.GetVect3fProperty("position",Vect3f(0.0f));
    l_fYaw        = l_XMLObject.GetFloatProperty("yaw") * FLOAT_PI_VALUE / 180.0f;
    l_fPitch      = l_XMLObject.GetFloatProperty("pitch") * FLOAT_PI_VALUE / 180.0f;
    l_fRoll       = l_XMLObject.GetFloatProperty("roll") * FLOAT_PI_VALUE / 180.0f;

    if(!GetResource(l_szName))
    {
      CRenderableObject* l_pRenderableObject = 0;
      if(l_szClass == "StaticMesh") {
        l_pRenderableObject = AddMeshInstance(l_szResource, l_szName);
      } else if(l_szClass == "AnimatedModel") {
        l_pRenderableObject = AddAnimatedModel(l_szResource, l_szName,l_szDefaultAnimation);
        //LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Object: \"%s\" has no class", l_szName.c_str());
      } else {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Object: \"%s\" has unknown \"%s\" class", l_szName.c_str(), l_szClass.c_str());
      }
      
      if(l_pRenderableObject != 0)
      {
        l_pRenderableObject->SetPosition(l_vPos);
        l_pRenderableObject->SetYaw     (l_fYaw);
        l_pRenderableObject->SetPitch   (l_fPitch);
        l_pRenderableObject->SetRoll    (l_fRoll);

        LOGGER->AddNewLog(ELL_INFORMATION,"CRenderableObjectsManager:: Added object: \"%s\"", l_szName.c_str());
      } else {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Object: \"%s\" not added", l_szName.c_str());
      }


    }else{
      if(!_bReload)
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
      }
      l_pRenderableObject->SetPosition(l_vPos);
      l_pRenderableObject->SetYaw     (l_fYaw);
      l_pRenderableObject->SetPitch   (l_fPitch);
      l_pRenderableObject->SetRoll    (l_fRoll);
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