#include "EnemyResource.h"


CEnemyResource::CEnemyResource(void)
{
  m_bIsOk = false;
}


void CEnemyResource::Release(void)
{
  // ----
}


bool CEnemyResource::Init(const char* _pcId, const char* _pcPathMalla)
{
  m_szPathMalla = _pcPathMalla;
  m_szId = _pcId;

  return m_bIsOk = true;
}
