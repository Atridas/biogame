#include "AnimatedCoreModel.h"

#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>

void CAnimatedCoreModel::Release()
{
  CHECKED_DELETE(m_pCalCoreModel)
  m_szMeshFilename      = "";
  m_szSkeletonFilename  = "";
  m_vTextureFilenameList.clear();
}

bool CAnimatedCoreModel::Load(const std::string &_szPath)
{
  m_szPath = _szPath;
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::Load Carregant el Animated Model \"%s\"", _szPath.c_str());

  SetOk(false);

  CXMLTreeNode l_treeActor;
  if(l_treeActor.LoadFile(_szPath.c_str()))
  {
    const char* l_pcAux = l_treeActor.GetPszProperty("name",0);

    if(l_pcAux != 0)
    {
      SetName(string(l_pcAux));
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"name\"");
      return IsOk();
    }

    m_pCalCoreModel = new CalCoreModel(GetName().c_str());

    int l_iNumChildren = l_treeActor.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; i++)
    {

      CXMLTreeNode l_treeChild = l_treeActor(i);
      if(strcmp(l_treeChild.GetName(),"skeleton") == 0)
      {
        if(m_szSkeletonFilename != "")
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load Esquelet repetit!");
          Release();
          return IsOk();
        }

        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);

        if(l_pcFileName != 0)
        {
          m_szSkeletonFilename = l_pcFileName;
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" a l'esquelet.");
          Release();
          return IsOk();
        }

        if(!LoadSkeleton())
        {
          Release();
          return IsOk();
        }

      } else if(strcmp(l_treeChild.GetName(),"mesh") == 0)
      {
        if(m_szMeshFilename != "")
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load Mesh repetit!");
          Release();
          return IsOk();
        }

        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);

        if(l_pcFileName != 0)
        {
          m_szMeshFilename = l_pcFileName;
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" al mesh.");
          Release();
          return IsOk();
        }

        if(!LoadMesh())
        {
          Release();
          return IsOk();
        }

      } else if(strcmp(l_treeChild.GetName(),"texture") == 0)
      {
        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);

        if(l_pcFileName != 0)
        {
          m_vTextureFilenameList.push_back(string(l_pcFileName));
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" a una textura.");
        }
      } else if(strcmp(l_treeChild.GetName(),"animation") == 0)
      {
        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);
        const char* l_pcName = l_treeChild.GetPszProperty("name",0);

        if(l_pcFileName == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" a una animaci�.");
        } else if(l_pcName == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"name\" a una animaci�.");
        } else {
          LoadAnimation(l_pcName, l_pcFileName);
        }

      } else if (!l_treeChild.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load S'ha trobat un element desconegut \"%s\"", l_treeChild.GetName());
      }
    }

    SetOk(true);
  } else {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat l'arxiu");
  }


  return IsOk();
}


bool CAnimatedCoreModel::LoadMesh()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadMesh Carregant el Mesh \"%s\"", m_szMeshFilename.c_str());

  if(m_pCalCoreModel->loadCoreMesh(m_szMeshFilename.c_str()) < 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh Error al Cal3D \"%s\"", CalError::getLastErrorText().c_str());
    return false;
  } else {
    return true;
  }
}

bool CAnimatedCoreModel::LoadSkeleton()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadSkeleton Carregant l'Esquelet \"%s\"", m_szSkeletonFilename.c_str());

  if(!m_pCalCoreModel->loadCoreSkeleton(m_szSkeletonFilename.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadSkeleton Error al Cal3D \"%s\"", CalError::getLastErrorText().c_str());
    return false;
  } else {
    return true;
  }
}

bool CAnimatedCoreModel::LoadAnimation(const string& _szName, const std::string& _szFilename)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadAnimation Carregant l'animaci� \"%s\"", _szFilename.c_str());

  if(m_pCalCoreModel->loadCoreAnimation(_szFilename,_szName) < 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadAnimation Error al Cal3D \"%s\"", CalError::getLastErrorText().c_str());
    return false;
  } else {
    return true;
  }
}
