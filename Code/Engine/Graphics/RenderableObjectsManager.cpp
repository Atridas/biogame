#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>

void CRenderableObjectsManager::Update(float ElapsedTime)
{
  
}

void CRenderableObjectsManager::Render(CRenderManager *RM)
{
  
}

CRenderableObject* CRenderableObjectsManager::AddMeshInstance(
                                                      const string& _szCoreMeshName,
                                                      const string& _szInstanceName,
                                                      const Vect3f& _vPosition)
{
  return NULL;
}

void CRenderableObjectsManager::AddResource(const string& _szName, CRenderableObject* _pRenderableObject)
{

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
    string l_szName;
    string l_szPath;
    CRenderableObject* l_pRenderableObject = 0;

    CXMLTreeNode l_XMLObject = l_XMLObjects(i);
    l_szName = l_XMLObject.GetPszProperty("name" ,"");
    //l_szPath = l_XMLObject.GetPszProperty("path" ,"");

    if(!GetResource(l_szName))
    {
      //l_pRenderableObject = new CRenderableObject();

      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderableObjectsManager:: Adding object: \"%s\"", l_szName.c_str());
      //AddResource(l_szName,l_pRenderableObject);

    }else{
      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
    }
  }

  return true;
}

CRenderableObject* CRenderableObjectsManager::GetInstance(const string& _szName) const
{
  return NULL;
}