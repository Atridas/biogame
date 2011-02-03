#include "EnemyManager.h"


CEnemyManager::CEnemyManager(void)
{

}


CEnemyManager::~CEnemyManager(void)
{

}

void CEnemyManager::Init(const string& _szPath)
{
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(_szPath.c_str()))
  {
    return;
  }

	CXMLTreeNode templates = newFile["templates"];

  string szId;
  string szMesh;

  if (templates.Exists())
  {
      int count = templates.GetNumChildren();

      for (int i = 0; i < count; ++i)
      {
        szId = string(templates(i).GetPszProperty("id"));
        szMesh = string(templates(i).GetPszProperty("mesh"));

        ///// omplir vectors de resources
      }
  }

}

void CEnemyManager::Release()
{

}