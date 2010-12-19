#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>

void CRenderableObjectsManager::Update(float _fElapsedTime)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
    m_RenderableObjects[i]->Update(_fElapsedTime);
}

void CRenderableObjectsManager::Render(CRenderManager *RM)
{
  for(size_t i=0; i < m_RenderableObjects.size() ; i++)
    m_RenderableObjects[i]->Render(RM);
}

//TODO
CRenderableObject* CRenderableObjectsManager::AddMeshInstance(
                                                      const string& _szCoreMeshName,
                                                      const string& _szInstanceName,
                                                      const Vect3f& _vPosition)
{
  return NULL;
}

void CRenderableObjectsManager::AddResource(const string& _szName, CRenderableObject* _pRenderableObject)
{
  m_Resources[_szName] = &_pRenderableObject;
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
    string l_szName;
    Vect3f l_vPos;
    float l_fYaw;
    float l_fPitch;
    float l_fRoll;
    CRenderableObject* l_pRenderableObject = 0;

    CXMLTreeNode l_XMLObject = l_XMLObjects(i);
    l_szName = l_XMLObject.GetPszProperty("name" ,"");
    l_vPos = l_XMLObject.GetVect3fProperty("position",Vect3f(0.0f));
    l_fYaw = l_XMLObject.GetFloatProperty("yaw");
    l_fPitch = l_XMLObject.GetFloatProperty("pitch");
    l_fRoll = l_XMLObject.GetFloatProperty("roll");

    if(!GetResource(l_szName))
    {
      //TODO: uncomment
      //l_pRenderableObject = new CRenderableObject(l_vPos, l_fYaw, l_fPitch, l_fRoll);

      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderableObjectsManager:: Adding object: \"%s\"", l_szName.c_str());
      //AddResource(l_szName,l_pRenderableObject);

    }else{
      LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
    }
  }

  return true;
}

//TODO
CRenderableObject* CRenderableObjectsManager::GetInstance(const string& _szName) const
{
  return NULL;
}