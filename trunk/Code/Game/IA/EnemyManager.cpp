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

  CXMLTreeNode l_XMLIAs;
  if(!l_XMLIAs.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CEnemyManager::Load() No s'ha trobat el XML.");
    SetOk(false);
    return false;
  }
	
  int l_iNumEnemys = l_XMLIAs.GetNumChildren();
  for(int i = 0; i < l_iNumEnemys; i++)
  {
    CXMLTreeNode l_XMLIA = l_XMLIAs(i);
	if(l_XMLIA.IsComment()) 
	{
      continue;
    }
	//------------ENEMY TEMPLATE------------------------------
	if(strcmp(l_XMLIA.GetName(),"enemy_template") == 0) 
    {
      string l_szType = l_treeIA.GetPszISOProperty("id", "");
      
      
      if(l_szType == "MILITAR")
      {
		CMilitar* l_pMilitar = new CMilitar(l_szType);
		l_pMilitar->Init(l_XMLIA);
		AddResource(l_pMilitar->GetName(),l_pMilitar);
        
      } else if(l_szType == "MINER")
      {
        CMiner* l_pMiner = new CMiner(l_szType);
		l_pMiner->Init(l_XMLIA);
		AddResource(l_pMiner->GetName(),l_pMiner);
      } else if(l_szType == "VIGIA")
      {
        CVigia* l_pVigia = new CVigia(l_szType);
		l_pVigia->Init(l_XMLIA);
		AddResource(l_pVigia->GetName(),l_pVigia);
      } else
      {
        LOGGER->AddNewLog(ELL_WARNING,"CEnemyManager::Load  type de pre-enemy_instance incorrecte o no trobat \"%s\".", l_szType.c_str());
        continue;
      }
    }
	//------------ENEMY INSTANCE------------------------------
    if(strcmp(l_XMLIA.GetName(),"enemy_instance") == 0) 
    {
      
      string l_szType = l_treeIA.GetPszISOProperty("type", "");
      
      
      if(l_szType == "MILITAR")
      {
		CMilitar* l_pMilitar = new CMilitar(l_szType);
		l_pMilitar->Init(l_XMLIA);
		AddResource(l_pMilitar->GetName(),l_pMilitar);
        
      } else if(l_szType == "MINER")
      {
        CMiner* l_pMiner = new CMiner(l_szType);
		l_pMiner->Init(l_XMLIA);
		AddResource(l_pMiner->GetName(),l_pMiner);
      } else if(l_szType == "VIGIA")
      {
        CVigia* l_pVigia = new CVigia(l_szType);
		l_pVigia->Init(l_XMLIA);
		AddResource(l_pVigia->GetName(),l_pVigia);
      } else
      {
        LOGGER->AddNewLog(ELL_WARNING,"CEnemyManager::Load  type de pre-enemy_instance incorrecte o no trobat \"%s\".", l_szType.c_str());
        continue;
      }
    }
  }
  SetOk(true);
  return IsOk();
}

     
