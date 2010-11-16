#include "Core.h"

#include <base.h>

#include "RenderManager.h"
#include "FontManager.h"
#include "Utils/LanguageManager.h"

bool CCore::Init(HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Init");

  m_pRenderManager    = new CRenderManager();
  m_pLanguageManager  = new CLanguageManager();
  m_pFontManager      = new CFontManager();

  m_pRenderManager->Init(hWnd);
  m_pLanguageManager->Init();
  m_pFontManager->Init(m_pRenderManager);

  SetOk(true);

  return IsOk();
}

void CCore::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Release");
  

  //delete a l'inrevès de com s'ha fet l'init
  CHECKED_DELETE(m_pFontManager);
  CHECKED_DELETE(m_pLanguageManager);
  CHECKED_DELETE(m_pRenderManager);
}

void CCore::Update()
{

}

void CCore::Render()
{

}