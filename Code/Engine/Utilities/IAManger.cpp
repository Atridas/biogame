#include "IAManager.h"
#include "GraphDefines.h"


bool CIAManager::Init()
{
  m_pGraph = new CSparseGraph(true);

  SetOk(true);
  return IsOk();
}

void CIAManager::Release()
{
  CHECKED_DELETE(m_pGraph);
}