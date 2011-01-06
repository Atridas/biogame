#include "AnimatedModelManager.h"

#include "AnimatedInstanceModel.h"
#include <XML/XMLTreeNode.h>

bool CAnimatedModelManager::Load(const string &_szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedModelManager::Load Carregant el fitxer \"%s\"", _szFileName.c_str());
  m_vXMLFiles.insert(_szFileName);

  CXMLTreeNode l_treeAnimatedModels;
  if(l_treeAnimatedModels.LoadFile(_szFileName.c_str()))
  {
    int l_iNumChildren = l_treeAnimatedModels.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeAnimatedModel = l_treeAnimatedModels(i);
      if(strcmp(l_treeAnimatedModel.GetName(),"animatedModel") == 0)
      {
        const char* l_pcPath = l_treeAnimatedModel.GetPszProperty("path",0);
        const char* l_pcName = l_treeAnimatedModel.GetPszProperty("name",0);

        if(l_pcPath == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load No s'ha trobat la propietat \"path\" a una animatedModel.");
        } else if(l_pcName == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load No s'ha trobat la propietat \"name\" a una animatedModel.");
        } else {
          GetCore(l_pcName, l_pcPath);
        }
      } else if(!l_treeAnimatedModel.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load S'ha trobat un element desconegut \"%s\"", l_treeAnimatedModel.GetName());
      }
    }
  } else {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load No s'ha trobat el fitxer");
  }
  return IsOk();
}


bool CAnimatedModelManager::Load(const vector<string>& _XMLs)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedModelManager::Load (Multi)");

  bool l_res = true;
  
  vector<string>::const_iterator l_end = _XMLs.cend();
  for(vector<string>::const_iterator l_it = _XMLs.cbegin(); l_it != l_end; ++l_it)
  {
    if(!Load(*l_it))
    {
      l_res = false;
    }
  }
  return l_res;
}

bool CAnimatedModelManager::Reload()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedModelManager::Reload");

  bool l_res = true;
  
  set<string>::iterator l_end = m_vXMLFiles.end();
  for(set<string>::iterator l_it = m_vXMLFiles.begin(); l_it != l_end; ++l_it)
  {
    if(!Load(*l_it))
    {
      l_res = false;
    }
  }
  return l_res;
}

CAnimatedCoreModel* CAnimatedModelManager::GetCore(const std::string &_szName)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedModelManager::GetCore Cercant la core \"%s\"",_szName.c_str());
  return GetResource(_szName);
}

CAnimatedCoreModel* CAnimatedModelManager::GetCore(const std::string &_szName, const std::string &_szPath)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedModelManager::GetCore Carregant la core \"%s\" a \"%s\"",_szName.c_str(),_szPath.c_str());
  CAnimatedCoreModel* l_pAnimatedCoreModel = GetResource(_szName);
  if(l_pAnimatedCoreModel)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedModelManager::GetCore Fent Reload");
    l_pAnimatedCoreModel->Reload(_szPath);
  } else {
    l_pAnimatedCoreModel = new CAnimatedCoreModel(_szName);
    if(l_pAnimatedCoreModel->Load(_szPath))
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedModelManager::GetCore Carregat correctament");
      AddResource(_szName,l_pAnimatedCoreModel);
    } else {
      LOGGER->AddNewLog(ELL_WARNING,"CAnimatedModelManager::GetCore No s'ha pogut carregar, borrant la core");
      CHECKED_DELETE(l_pAnimatedCoreModel)
    }
  }
  return l_pAnimatedCoreModel;
}

CAnimatedInstanceModel* CAnimatedModelManager::GetInstance(const std::string &_szName)
{
  CAnimatedCoreModel* l_pCore = GetResource(_szName);
  if(!l_pCore)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::GetInstance  No s'ha trobat la core \"%s\"", _szName.c_str());
    return 0;
  }
  CAnimatedInstanceModel* l_pInstance = new CAnimatedInstanceModel();
  l_pInstance->Initialize(l_pCore);

  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedModelManager::GetInstance  S'ha creat una instància de la core \"%s\"", _szName.c_str());

  return l_pInstance;
}
