#include "Core.h"

#include <base.h>

#include "params.h"

#include <RenderManager.h>
#include <FontManager.h>
#include <Utils/LanguageManager.h>
#include <InputManager.h>

bool CCore::Init(HWND hWnd, const SInitParams& _InitParams)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Init");

  m_pRenderManager    = new CRenderManager();
  m_pLanguageManager  = new CLanguageManager();
  m_pFontManager      = new CFontManager();
  m_pInputManager     = new CInputManager();

  m_pRenderManager->Init(hWnd,_InitParams.RenderManagerParams);
  m_pLanguageManager->Init(_InitParams.LanguageManagerParams);
  m_pFontManager->Init(m_pRenderManager,_InitParams.FontManagerParams.pcFontsXML);
  m_pInputManager->Init(hWnd,Vect2i(_InitParams.RenderManagerParams.uiWidth,_InitParams.RenderManagerParams.uiHeight),_InitParams.InputManagerParams.bExclusiveMouse);

  SetOk(true);

  return IsOk();
}

void CCore::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Release");
  

  //delete a l'inrev�s de com s'ha fet l'init
  CHECKED_DELETE(m_pInputManager);
  CHECKED_DELETE(m_pFontManager);
  CHECKED_DELETE(m_pLanguageManager);
  CHECKED_DELETE(m_pRenderManager);
}

void CCore::Update(float elapsedTime)
{

}

void CCore::Render()
{

}