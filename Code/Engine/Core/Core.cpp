#include "Core.h"

#include <base.h>

#include "params.h"

#include <RenderManager.h>
#include <FontManager.h>
#include <Utils/LanguageManager.h>
#include <InputManager.h>
#include <ActionToInput.h>
#include <RenderableObjectsManager.h>
#include <LightManager.h>
//#include <AnimatedModelManager.h>

bool CCore::Init(HWND hWnd, const SInitParams& _InitParams)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Init");

  m_pRenderManager            = new CRenderManager();
  m_pLanguageManager          = new CLanguageManager();
  m_pFontManager              = new CFontManager();
  m_pInputManager             = new CInputManager();
  m_pActionToInput            = new CActionToInput();
  m_pRenderableObjectsManager = new CRenderableObjectsManager();
  m_pLightManager             = new CLightManager();

  m_pRenderManager->Init(hWnd,_InitParams.RenderManagerParams);
  m_pLanguageManager->Init(_InitParams.LanguageManagerParams);
  m_pFontManager->Init(m_pRenderManager,_InitParams.FontManagerParams.pcFontsXML);
  m_pInputManager->Init(hWnd,Vect2i(_InitParams.RenderManagerParams.v2iResolution.x,_InitParams.RenderManagerParams.v2iResolution.y),_InitParams.InputManagerParams.bExclusiveMouse);
  m_pActionToInput->Init(m_pInputManager,_InitParams.ActionToInputParams.pcFile);

  m_pRenderableObjectsManager->Load(_InitParams.RenderableObjectsManager.vXMLFiles);
  m_pLightManager->Load(_InitParams.LightsManager.szFile);

  SetOk(true);

  return IsOk();
}

void CCore::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Release");
  

  //delete a l'inrevès de com s'ha fet l'init
  CHECKED_DELETE(m_pLightManager);
  CHECKED_DELETE(m_pRenderableObjectsManager);

  CHECKED_DELETE(m_pActionToInput);
  CHECKED_DELETE(m_pInputManager);
  CHECKED_DELETE(m_pFontManager);
  CHECKED_DELETE(m_pLanguageManager);
  CHECKED_DELETE(m_pRenderManager);
}

void CCore::Update(float elapsedTime)
{
  m_pInputManager->Update();
  m_pActionToInput->Update(elapsedTime);
  m_pRenderableObjectsManager->Update(elapsedTime);
}