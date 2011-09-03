#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysXTriggerEntityController.h"
#include "PhysXCollisionEntityController.h"
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
#include <SoundManager.h>
#include <EntityManager.h>
#include <PortalManager.h>
#include <IAManager.h>
#include <EmiterCoreManager.h>
#include <EmiterManager.h>

#include "Utils\MemLeaks.h"
//#include <AnimatedModelManager.h>



//Macros d'inicialitzacio
#define INIT_NO_ARGUMENTS(object, name)                        \
  if(IsOk())                                                   \
  if(!object->Init())                                          \
  {                                                            \
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al %s.", name);  \
    SetOk(false);                                              \
  }

#define LOAD(object, name, argument)                          \
  if(IsOk())                                                  \
  if(!object->Load(argument))                                 \
  {                                                           \
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al %s.", name); \
    SetOk(false);                                             \
  }

#define INIT(object, name, argument)                          \
  if(IsOk())                                                  \
  if(!object->Init(argument))                                 \
  {                                                           \
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al %s.", name); \
    SetOk(false);                                             \
  }

#define INIT2(object, name, argument, argument2)              \
  if(IsOk())                                                  \
  if(!object->Init(argument, argument2))                      \
  {                                                           \
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al %s.", name); \
    SetOk(false);                                             \
  }

#define INIT3(object, name, argument, argument2, argument3)   \
  if(IsOk())                                                  \
  if(!object->Init(argument, argument2, argument3))           \
  {                                                           \
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al %s.", name); \
    SetOk(false);                                             \
  }

#define INIT4(object, name, argument, argument2, argument3, argument4)   \
  if(IsOk())                                                             \
  if(!object->Init(argument, argument2, argument3, argument4))           \
  {                                                                      \
    LOGGER->AddNewLog(ELL_ERROR,"Core:: Error al %s.", name);            \
    SetOk(false);                                                        \
  }


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
  m_pEmiterCoreManager        = new CEmiterCoreManager();
  m_pSoundManager             = new CSoundManager();
  m_pEntityManager            = new CEntityManager();
  m_pPhysicTriggerReport      = new CPhysXTriggerEntityController();
  m_pPhysicCollisionReport    = new CPhysXCollisionEntityController();
  m_pPortalManager            = new CPortalManager();
  m_pIAManager                = new CIAManager();
  m_pEmiterManager            = new CEmiterManager();

  m_pEngine                   = _pEngine;

  m_pRenderManager->Init(hWnd,_InitParams.RenderManagerParams);

  SetOk(true);

  INIT_NO_ARGUMENTS(m_pTextureManager, "manager de textures");

  LOAD(m_pEffectManager, "Manager d'Effects", _InitParams.EffectManagerParams);
  
  LOAD(m_pStaticMeshManager, "Manager de Static Meshes", _InitParams.StaticMeshManagerParams);
  
  LOAD(m_pAnimatedModelManager, "Manager d'Animated Models", _InitParams.AnimatedModelManagerParams);
  
  INIT(m_pLanguageManager, "Language Manager", _InitParams.LanguageManagerParams);
  INIT2(m_pFontManager, "Font Manager", m_pRenderManager,_InitParams.FontManagerParams.pcFontsXML);
  INIT4(m_pInputManager, "Input Manager", hWnd, Vect2i(_InitParams.RenderManagerParams.v2iResolution.x,_InitParams.RenderManagerParams.v2iResolution.y),_InitParams.InputManagerParams.bExclusiveMouse,_InitParams.InputManagerParams.fSensitivity);
  INIT(m_pActionManager, "Action Manager", _InitParams.ActionToInputParams.pcFile);
  
  LOAD(m_pRenderableObjectsManager, "Manager de Renderable Objects", _InitParams.RenderableObjectsManagerParams.vXMLFiles);
  INIT(m_pPortalManager, "Portal Manager", _InitParams.PortalManagerParams.szFiles);
  LOAD(m_pLightManager, "Manager de Llums", _InitParams.LightsManagerParams.szFile);
  LOAD(m_pSceneEffectManager, "Manager de Effectes d'Escena", _InitParams.SceneEffectParams.szFile);

  m_pScriptManager->Initialize();
  m_pScriptManager->Load(_InitParams.ScriptManagerParams.szFile);
  m_pLogRender->SetWindowsPos(_InitParams.LogRenderParams.vPosition);

  INIT(m_pConsole, "Consola", m_pScriptManager);
  
  INIT(m_pGUIManager, "Manager de GUI", _InitParams.GUIManagerParams.szXML);
  if(m_pGUIManager->IsOk())
  {
    m_pGUIManager->ActiveWindows(_InitParams.GUIManagerParams.szInitWindow);
    m_pGUIManager->SetVisiblePointerMouse(_InitParams.GUIManagerParams.bRenderMouse);
  }
  
  INIT(m_pPhysicsManager, "Manager de Physics", _InitParams.PhysXManagerParams.szFile);
  if(m_pPhysicsManager->IsOk())
  {
    m_pPhysicsManager->SetTriggerReport(m_pPhysicTriggerReport);
    m_pPhysicsManager->SetCollisionReport(m_pPhysicCollisionReport);
  }
  
  //TODO
  LOAD(m_pEmiterCoreManager, "Manager de Cores d'emisors de partícules", _InitParams.CoreEmiterManagerParams.sFiles);

  INIT(m_pSoundManager, "Manager de Sons", _InitParams.SoundManagerParams.szFile);
  
  INIT_NO_ARGUMENTS(m_pIAManager, "Manager d'IA");

  srand(_InitParams.RandomSeed);

  return IsOk();
}

void CCore::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Core::Release");
  

  //delete a l'inrevès de com s'ha fet l'init
  CHECKED_DELETE(m_pIAManager);
  CHECKED_DELETE(m_pPhysicCollisionReport);
  CHECKED_DELETE(m_pPhysicTriggerReport);
  CHECKED_DELETE(m_pEntityManager);
  CHECKED_DELETE(m_pSoundManager);
  CHECKED_DELETE(m_pEmiterManager);
  CHECKED_DELETE(m_pEmiterCoreManager);
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

  m_pPhysicsManager->WaitForSimulation();  

  m_pEntityManager->UpdatePostPhysX(l_fElapsedTime);

  m_pEntityManager->UpdatePostAnim(l_fElapsedTime);
  
  m_pEntityManager->PostUpdate(l_fElapsedTime);

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
  //m_pPhysicsManager->WaitForSimulation();

  m_pEmiterManager->Update(l_fElapsedTime);

  m_pPortalManager->Update(l_fElapsedTime);
}