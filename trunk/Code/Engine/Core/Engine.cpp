#include "Engine.h"

#include <base.h>

#include <Utils/Exception.h>

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <LogRender.h>
#include <Utils/Timer.h>
#include <ActionManager.h>
#include <Console.h>
#include <GUIManager.h>
#include <EffectManager.h>
#include "HDRPipeline.h"

#include "params.h"

bool CEngine::Init(const SInitParams& _InitParams,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");

  m_pCore = new CCore();

  bool result = m_pCore->Init(hWnd, _InitParams, this);

  assert(result);
  
  if(m_pActiveProcess) //TODO: Comprovar excepcio m_pProcess == NULL i logejar
  {
    m_pActiveProcess->RegisterLuaFunctions();
    m_pActiveProcess->Init(); 
  }
  m_pHDR = new CHDRPipeline();
  if(!m_pHDR->Init(_InitParams.EngineParams.szHDRFile))
  {
    CHECKED_DELETE( m_pHDR );
  }

  m_pCore->GetActionManager()->SetProcess(m_pActiveProcess);

  SetOk(true);

  return IsOk();
}

void CEngine::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Release");

  CHECKED_DELETE(m_pHDR);
  
  for(VectorProcessPtr::iterator l_it = m_vProcesses.begin(); l_it != m_vProcesses.end(); ++l_it)
  {
    delete *l_it;
  }
  m_vProcesses.clear();
  m_pActiveProcess = 0;
  CHECKED_DELETE(m_pCore);
  LOGGER->SaveLogsInFile();
}

void CEngine::Update()
{
	m_pCore->Update();
	float l_fElapsedTime = m_pCore->GetTimer()->GetElapsedTime();

	if(m_pActiveProcess != NULL)
		m_pActiveProcess->Update(l_fElapsedTime);

  UpdateSystems(l_fElapsedTime);
}

void CEngine::Render()
{
	CRenderManager* l_pRM = m_pCore->GetRenderManager();

  if(m_pActiveProcess != NULL)
  {
    CEffectManager* l_pEM = m_pCore->GetEffectManager();
    l_pEM->Begin();

    CColor l_ClearColor = l_pRM->GetClearColor();
    l_pRM->SetClearColor(colBLACK);

    m_pActiveProcess->PreRender(l_pRM);

    l_pRM->SetClearColor(l_ClearColor);

    if(m_pHDR && m_pHDR->IsActive())
    {
      RenderHDR(l_pRM, m_pActiveProcess);
    } else {
      RenderNoHDR(l_pRM, m_pActiveProcess);
    }
  }
}


void CEngine::RenderHDR(CRenderManager* _pRM, CProcess* _pProcess)
{
  assert(m_pHDR->IsOk());

  m_pHDR->PrepareTextures(_pRM, _pProcess);

  _pRM->BeginRendering();
  
  m_pHDR->Render(_pRM);
  _pProcess->PostRender(_pRM);
	_pProcess->DebugInformation();
  
  CEngine::RenderSystems(_pRM);

	_pRM->EndRendering();
}

void CEngine::RenderNoHDR(CRenderManager* _pRM, CProcess* _pProcess)
{
  _pRM->BeginRendering();

  _pRM->SetupMatrices(m_pActiveProcess->GetCamera());
  _pProcess->Render(_pRM);
  _pProcess->PostRender(_pRM);
	_pProcess->DebugInformation();

  CEngine::RenderSystems(_pRM);

	_pRM->EndRendering();
  _pRM->Present();
}


void CEngine::RenderSystems(CRenderManager* _pRM)
{
  CFontManager* l_pFontManager = m_pCore->GetFontManager();
  CLogRender* l_pLR = m_pCore->GetLogRender();

  _pRM->EnableAlphaBlend();

  if(l_pLR)
    l_pLR->Render(_pRM,l_pFontManager);

  CConsole* l_pC = m_pCore->GetConsole();
  if(l_pC)
    l_pC->Render(_pRM,l_pFontManager);

  CGUIManager* l_pGUI = m_pCore->GetGUIManager();
  if(l_pGUI)
  {
    l_pGUI->Render(_pRM,l_pFontManager);
    l_pGUI->RenderPointerMouse(_pRM,l_pFontManager);
  }

  _pRM->DisableAlphaBlend();
}

void CEngine::UpdateSystems(float _fElapsedTime)
{
  CLogRender* l_pLR = m_pCore->GetLogRender();
  if(l_pLR)
    l_pLR->Update(_fElapsedTime);

  CConsole* l_pC = m_pCore->GetConsole();
  if(l_pC)
    l_pC->Update(_fElapsedTime);

  CGUIManager* l_pGUI = m_pCore->GetGUIManager();
  if(l_pGUI)
    l_pGUI->Update(_fElapsedTime);
}

void CEngine::AddProcess(CProcess* _pProcess)
{
  m_vProcesses.push_back(_pProcess);
  if(!m_pActiveProcess) 
  {
    m_pActiveProcess = _pProcess;
  }
}

void CEngine::ActivateProcess(int _i)
{
  m_pActiveProcess = m_vProcesses[_i];
}

void CEngine::ActivateProcess(CProcess* _pProcess)
{
  for(VectorProcessPtr::iterator l_it = m_vProcesses.begin(); l_it != m_vProcesses.end(); ++l_it)
  {
    if(*l_it == _pProcess)
    {
      m_pActiveProcess = *l_it;
      return;
    }
  }
  assert(false && "Assingant un process inexistent");

}