#include "StaticMeshManager.h"
#include <XML/XMLTreeNode.h>

bool CStaticMeshManager::Load(const string &_szFileName)
{
  return Load(_szFileName, false);
}

bool CStaticMeshManager::Load(const string &_szFileName, bool _bReload)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMeshManager::Load");

  m_szFileName = _szFileName;

  CXMLTreeNode l_XMLMeshes;
  if(!l_XMLMeshes.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CStaticMeshManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    return false;
  }

  int l_iNumMeshes = l_XMLMeshes.GetNumChildren();
  for(int i = 0; i < l_iNumMeshes; i++)
  {
    string l_szName;
    string l_szPath;
    CStaticMesh* l_pStaticMesh = 0;

    CXMLTreeNode l_XMLMesh = l_XMLMeshes(i);
    l_szName = l_XMLMesh.GetPszProperty("name" ,"");
    l_szPath = l_XMLMesh.GetPszProperty("path" ,"");

    if(!GetResource(l_szName))
    {
      l_pStaticMesh = new CStaticMesh();

      if(l_pStaticMesh->Load(l_szPath))
      {
        LOGGER->AddNewLog(ELL_INFORMATION,"CStaticMeshManager:: Adding mesh: \"%s\"", l_szName.c_str());
        AddResource(l_szName,l_pStaticMesh);
      }else{
        CHECKED_DELETE(l_pStaticMesh);
      }
    }else{
      if(!_bReload)
        LOGGER->AddNewLog(ELL_WARNING,"CStaticMeshManager:: Mesh \"%s\" repetit", l_szName.c_str());
    }
  }

  return true;
}

bool CStaticMeshManager::Reload()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMeshManager::Reload");

  return Load(m_szFileName, true);
}