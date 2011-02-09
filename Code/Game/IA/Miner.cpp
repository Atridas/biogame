#include "Miner.h"


void CMiner::InitTemplate(CXMLTreeNode& _XMLParams)
{
  //CMiner::InitTemplate(_XMLParams);
  m_fRotateSpeed = (_XMLParams.GetFloatProperty("rotate_speed",false));
}

void CMiner::InitInstance(CXMLTreeNode& _XMLParams)
{
  //CMiner::InitInstance(_XMLParams);
}
