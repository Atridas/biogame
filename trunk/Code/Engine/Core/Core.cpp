#include "Core.h"

#include <base.h>

#include "RenderManager.h"

bool CCore::Init()
{
  m_pRenderManager = new CRenderManager();


  m_pRenderManager->Init();

  return m_bIsOk = true;
}

void CCore::Relase()
{
  CHECKED_DELETE(m_pRenderManager);
}

void CCore::Update()
{

}

void CCore::Render()
{

}