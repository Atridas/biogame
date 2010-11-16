#include "Core.h"

#include <base.h>

#include "RenderManager.h"

bool CCore::Init(HWND hWnd)
{
  m_pRenderManager = new CRenderManager();

  m_pRenderManager->Init(hWnd);

  SetOk(true);

  return IsOk();
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