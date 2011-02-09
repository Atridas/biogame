#include "Militar.h"



void CMilitar::InitTemplate(CXMLTreeNode& _XMLParams)
{
  m_fRotateSpeed = (_XMLParams.GetFloatProperty("rotate_speed",false));
}

void CMilitar::InitInstance(CXMLTreeNode& _XMLParams)
{
}