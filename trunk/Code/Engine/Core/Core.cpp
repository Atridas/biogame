#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysXTriggerEntityController.h"
#include "Core.h"

#include <base.h>
#include "Utils/Timer.h"
#include "params.h"

#include <RenderManager.h>
#include "TextureManager.h"
#include "StaticMeshManager.h"
#include "AnimatedModelManager.h"
#include "EffectManager.h"
#include <FontManager.h>
#include <Utils/LanguageManager.h>
#include <InputManager.h>
#include <ActionManager.h>
#include <RenderableObjectsManager.h>
#include <LightManager.h>
#include <SceneEffectManager.h>
#include <ScriptManager.h>
#include <LogRender.h>
#include <Console.h>
#include <GUIManager.h>
#include <PhysicsManager.h>
#include <ParticleManager.h>
#include <BillBoardManager.h>
#include <SoundManager.h>
#include <EntityManager.h>
#include <PortalManager.h>

#include "Utils\MemLeaks.h"
//#include <AnimatedModelManager.h>

bool CCore::Init(HWND hWnd, const SInitParams& _InitParams, CEngine* _pEngine)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Init");
  
  m_pTimer                    = new CTimer(30);
  m_pRenderManager            = new CRenderManager();
  m_pTextureManager           = new CTextureManager();
  m_pStaticMeshManager        = new CStaticMeshManager();
  m_pAnimatedModelManager     = new CAnimatedModelManager();
  m_pEffectManager            = new CEffectManager();
  m_pLanguageManager          = new CLanguageManager();
  m_pFontManager              = new CFontManager();
  m_pInputManager             = new CInputManager();
  m_pActionManager            = new CActionManager();
  m_pRenderableObjectsManager = new CRenderableObjectsManager();
  m_pLightManager             = new CLightManager();
  m_pSceneEffectManager       = new CSceneEffectManager();
  m_pScriptManager            = new CScriptManager();
  m_pLogRender                = new CLogRender();
  m_pConsole                  = new CConsole();
  m_pGUIManager               = new CGUIManager(_InitParams.RenderManagerParams.v2iResolution);
  m_pPhysicsManager           = new CPhysicsManager();
  m_pParticleManager          = new CParticleManager();
  m_pBillBoardManager         = new CBillBoardManager();
  m_pSoundManager             = new CSoundManager();
  m_pEntityManager            = new CEntityManager();
  m_pPhysicTriggerReport      = new CPhysXTriggerEntityController();
  m_pPortalManager            = new CPortalManager();

  m_pEngine                   = _pEngine;

  m_pRenderManager->Init(hWnd,_InitParams.RenderManagerParams);

  SetOk(true);

  if(!m_pTextureManager->Init()) 
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al manager de textures.");
    SetOk(false);
  }

  if(!m_pEffectManager->Load(_InitParams.EffectManagerParams))
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al manager d'Effects.");
    SetOk(false);
  }

  if(!m_pStaticMeshManager->Load(_InitParams.StaticMeshManagerParams))
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al manager de Static Meshes.");
    SetOk(false);
  }

  if(!m_pParticleManager->Load(_InitParams.PaticleManagerParams.szFile))
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al manager de particulas");
    SetOk(false);
  }
  m_pParticleManager->Init(CORE->GetRenderManager());

  if(!m_pBillBoardManager->Load(_InitParams.BillBoardManagerParams.szFile))
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al manager de particulas");
    SetOk(false);
  }
  m_pBillBoardManager->Init(CORE->GetRenderManager());

  if(!m_pAnimatedModelManager->Load(_InitParams.AnimatedModelManagerParams))
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al manager d'Animated Models.");
    SetOk(false);
  }

  m_pLanguageManager->Init(_InitParams.LanguageManagerParams);
  m_pFontManager->Init(m_pRenderManager,_InitParams.FontManagerParams.pcFontsXML);
  m_pInputManager->Init(hWnd,Vect2i(_InitParams.RenderManagerParams.v2iResolution.x,_InitParams.RenderManagerParams.v2iResolution.y),_InitParams.InputManagerParams.bExclusiveMouse);
  m_pActionManager->Init(_InitParams.ActionToInputParams.pcFile);
  
  m_pRenderableObjectsManager->Load(_InitParams.RenderableObjectsManagerParams.vXMLFiles);
  m_pPortalManager->Init(_InitParams.PortalManagerParams.szFiles);
  m_pLightManager->Load(_InitParams.LightsManagerParams.szFile);
  m_pSceneEffectManager->Load(_InitParams.SceneEffectParams.szFile);
  m_pScriptManager->Initialize();
  m_pScriptManager->Load(_InitParams.ScriptManagerParams.szFile);
  m_pLogRender->SetWindowsPos(_InitParams.LogRenderParams.vPosition);
  m_pConsole->Init(m_pScriptManager);
  m_pGUIManager->Init(_InitParams.GUIManagerParams.szXML);
  m_pGUIManager->ActiveWindows(_InitParams.GUIManagerParams.szInitWindow);
  m_pGUIManager->SetVisiblePointerMouse(_InitParams.GUIManagerParams.bRenderMouse);
  m_pPhysicsManager->Init(_InitParams.PhysXManagerParams.szFile);
  m_pPhysicsManager->SetTriggerReport(m_pPhysicTriggerReport);
  
  if(!m_pSoundManager->Init(_InitParams.SoundManagerParams.szFile))
  {
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error inicialitzant SoundManager.");
    SetOk(false);  
  }
  
  return IsOk();
}

void CCore::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Release");
  

  //delete a l'inrevès de com s'ha fet l'init
  CHECKED_DELETE(m_pPhysicTriggerReport);
  CHECKED_DELETE(m_pEntityManager);
  CHECKED_DELETE(m_pSoundManager);
  CHECKED_DELETE(m_pBillBoardManager);
  CHECKED_DELETE(m_pParticleManager);
  CHECKED_DELETE(m_pPhysicsManager);
  CHECKED_DELETE(m_pGUIManager);
  CHECKED_DELETE(m_pConsole);
  CHECKED_DELETE(m_pLogRender)
  CHECKED_DELETE(m_pScriptManager)
  CHECKED_DELETE(m_pSceneEffectManager)
  CHECKED_DELETE(m_pLightManager)
  CHECKED_DELETE(m_pPortalManager);
  CHECKED_DELETE(m_pRenderableObjectsManager)
  CHECKED_DELETE(m_pActionManager)
  CHECKED_DELETE(m_pInputManager)
  CHECKED_DELETE(m_pFontManager)
  CHECKED_DELETE(m_pLanguageManager)
  CHECKED_DELETE(m_pEffectManager)
  CHECKED_DELETE(m_pAnimatedModelManager)
  CHECKED_DELETE(m_pStaticMeshManager)
  CHECKED_DELETE(m_pTextureManager)
  CHECKED_DELETE(m_pRenderManager)
  CHECKED_DELETE(m_pTimer)
}

void CCore::Update()
{
  //Time update
  m_pTimer->Update();
  float l_fElapsedTime = m_pTimer->GetElapsedTime();

  //Manager Updates
  m_pInputManager->Update();
  if(!m_pConsole->IsActive())
  {
    m_pActionManager->Update(l_fElapsedTime);
  }
  
  m_pEntityManager->PreUpdate(l_fElapsedTime);
  m_pEntityManager->Update(l_fElapsedTime);
  m_pEntityManager->UpdatePrePhysX(l_fElapsedTime);

  m_pPhysicsManager->Update(l_fElapsedTime);// -------------
  
  m_pEntityManager->UpdatePostPhysX(l_fElapsedTime);

  //m_pParticleManager->Update(l_fElapsedTime);// -------------
  m_pRenderableObjectsManager->Update(l_fElapsedTime);// ----
  
  m_pEntityManager->UpdatePostAnim(l_fElapsedTime);
  
  m_pEntityManager->PostUpdate(l_fElapsedTime);

  m_pPortalManager->Update(l_fElapsedTime);
}