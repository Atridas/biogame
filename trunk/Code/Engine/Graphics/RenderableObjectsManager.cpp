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
  //renderitzar nom�s els visibles
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
                                                      const string& _szInstanceName,
                                                      const string& _szDefaultAnimation)
{
  if(GetResource(_szInstanceName) != 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CRenderableObjectsManager:: CAnimatedModel amb nom repetit \"%s\"", _szCoreModelName.c_str());
    return 0;
  }

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
  int l_iMeshIndex = 0;
  for(int i = 0; i < l_iNumObjects; i++)
  {
    string l_szName, l_szClass, l_szResource,l_szDefaultAnimation;
    Vect3f l_vPos,l_vMin,l_vMax;
    float l_fYaw;
    float l_fPitch;
    float l_fRoll;
    float l_fAltura;

    CRenderableObject* l_pRenderableObject = 0;

    CXMLTreeNode l_XMLObject = l_XMLObjects(i);

    l_szName      = l_XMLObject.GetPszISOProperty("name" ,"");
    l_szClass     = l_XMLObject.GetPszISOProperty("class" ,"");
    l_szResource  = l_XMLObject.GetPszISOProperty("resource" ,"");
    l_szDefaultAnimation = l_XMLObject.GetPszISOProperty("cycle" ,"");
    //l_szDefaultAnimation = l_XMLObject.GetIntProperty("cycle");
    l_vMin        = l_XMLObject.GetVect3fProperty("Min",Vect3f(0.0f));
    l_vMax        = l_XMLObject.GetVect3fProperty("Max",Vect3f(0.0f));
    l_fAltura     = l_XMLObject.GetFloatProperty("Altura");

    l_vPos        = l_XMLObject.GetVect3fProperty("position",Vect3f(0.0f));
    l_fYaw        = l_XMLObject.GetFloatProperty("yaw") * FLOAT_PI_VALUE / 180.0f;
    l_fPitch      = l_XMLObject.GetFloatProperty("pitch") * FLOAT_PI_VALUE / 180.0f;
    l_fRoll       = l_XMLObject.GetFloatProperty("roll") * FLOAT_PI_VALUE / 180.0f;

    l_pRenderableObject = GetResource(l_szName);
    if(!l_pRenderableObject)
    {
      if(l_szClass == "StaticMesh") 
      {
        l_pRenderableObject = AddMeshInstance(l_szResource, l_szName);
        l_pRenderableObject->m_vMin = Vect3f(l_vMin.x,l_vMin.y,l_vMin.z);
          l_pRenderableObject->m_vMax = Vect3f(l_vMax.x,l_vMax.y,l_vMax.z);
          l_pRenderableObject->m_fAltura = 0;
          m_vIndexMeshes.push_back(l_iMeshIndex);

      } else if(l_szClass == "AnimatedModel") 
      {
        l_pRenderableObject = AddAnimatedModel(l_szResource, l_szName,l_szDefaultAnimation);
        m_vIndexAnimated.push_back(l_iMeshIndex);
          l_pRenderableObject->m_fAltura = l_fAltura;

      } else 
      {
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

      l_iMeshIndex = l_iMeshIndex + 1;
    }else{
      //i=i-1;
      if(!_bReload)
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
      }
      /*l_pRenderableObject->SetPosition(l_vPos);
      l_pRenderableObject->SetYaw     (l_fYaw);
      l_pRenderableObject->SetPitch   (l_fPitch);
      l_pRenderableObject->SetRoll    (l_fRoll);*/
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


void CRenderableObjectsManager::SetAllVisible(bool _bVisible, int _ID)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
  {
    if (i==_ID)
    {
      m_RenderableObjects[i]->SetVisible(true);
    }
    else
    {
      m_RenderableObjects[i]->SetVisible(_bVisible);
    }
  }
}