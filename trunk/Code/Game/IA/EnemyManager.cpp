#include "EnemyManager.h"
#include "XML/XMLTreeNode.h"
#include "Militar.h"
#include "Miner.h"
#include "Vigia.h"
#include "Enemy.h"

CEnemyManager::CEnemyManager()
{
  SetOk(true);
}


bool CEnemyManager::Load(const string &_szFileName)
{
  m_szFileName = _szFileName;
  return Load();
}

bool CEnemyManager::Load()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CEnemyManager::Load() Carregant el fitxer \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_treeIA;
  if(!l_treeIA.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CEnemyManager::Load() No s'ha trobat el XML.");
    SetOk(false);
    Release();
    return IsOk;
  }

  int l_iNumChildren = l_treeIA.GetNumChildren();
  for(int i = 0; i < l_iNumChildren; i++)
  {
    CXMLTreeNode l_treeSceneEffect = l_treeIA(i);
    if(strcmp(l_treeSceneEffect.GetName(),"enemy_instance") == 0)
    {
      //enemy_instance
      string l_szType = l_treeIA.GetPszISOProperty("type", "");
      
      CEnemy* l_pCEnemy = 0;
      if(l_szType == "MILITAR")
      {
        //l_pCEnemy = new CMilitar();
      } else if(l_szType == "MINER")
      {
        //l_pCEnemy = new CMiner();
      } else if(l_szType == "VIGIA")
      {
        //l_pCEnemy = new CVigia();
      } else
      {
        LOGGER->AddNewLog(ELL_WARNING,"CEnemyManager::Load  type de pre-enemy_instance incorrecte o no trobat \"%s\".", l_szType.c_str());
        continue;
      }
    }
  }
  return true;
}

     
