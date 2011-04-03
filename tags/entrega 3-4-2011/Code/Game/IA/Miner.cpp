#include "Miner.h"


void CMiner::InitTemplate(CXMLTreeNode& _XMLParams)
{
  CEnemy::InitTemplate(_XMLParams);
  m_fRotateSpeed = (_XMLParams.GetFloatProperty("rotate_speed",false));
}

void CMiner::InitInstance(CXMLTreeNode& _XMLParams)
{
  CEnemyInstance::InitInstance(_XMLParams);
}
