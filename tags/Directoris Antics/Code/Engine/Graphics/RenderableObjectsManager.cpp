#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "InstanceMesh.h"
#include <XML/XMLTreeNode.h>

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
  CInstanceMesh* l_pInstanceMesh = new CInstanceMesh(_szInstanceName);
  if(!l_pInstanceMesh->Init(_szCoreMeshName))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CRenderableObjectsManager:: No s'ha pogut carregar el CInstanceMesh \"%s\" de la core \"%s\"", _szInstanceName, _szCoreMeshName);
    delete l_pInstanceMesh;
    return 0;
  }

  AddResource(_szInstanceName, l_pInstanceMesh);

  return l_pInstanceMesh;
}

void CRenderableObjectsManager::AddResource(const string& _szName, CRenderableObject* _pRenderableObject)
{
  CMapManager::AddResource(_szName,_pRenderableObject);
  m_RenderableObjects.push_back(_pRenderableObject);
}

bool CRenderableObjectsManager::Load(const string& _szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CRenderableObjectsManager::Load");

  m_szFileName = _szFileName;

  CXMLTreeNode l_XMLObjects;
  if(!l_XMLObjects.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    return false;
  }

  int l_iNumObjects = l_XMLObjects.GetNumChildren();
  for(int i = 0; i < l_iNumObjects; i++)
  {
    string l_szName, l_szClass, l_szResource;
    Vect3f l_vPos;
    float l_fYaw;
    float l_fPitch;
    float l_fRoll;
    CRenderableObject* l_pRenderableObject = 0;

    CXMLTreeNode l_XMLObject = l_XMLObjects(i);

    l_szName      = l_XMLObject.GetPszProperty("name" ,"");
    l_szClass     = l_XMLObject.GetPszProperty("class" ,"");
    l_szResource  = l_XMLObject.GetPszProperty("resource" ,"");

    l_vPos        = l_XMLObject.GetVect3fProperty("position",Vect3f(0.0f));
    l_fYaw        = l_XMLObject.GetFloatProperty("yaw");
    l_fPitch      = l_XMLObject.GetFloatProperty("pitch");
    l_fRoll       = l_XMLObject.GetFloatProperty("roll");

    if(!GetResource(l_szName))
    {
      CRenderableObject* l_pRenderableObject = 0;
      if(l_szClass == "StaticMesh") {
        l_pRenderableObject = AddMeshInstance(l_szResource, l_szName);
      } else if(l_szClass == "") {
        LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Object: \"%s\" has no class", l_szName.c_str());
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
      LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
    }
  }

  return true;
}