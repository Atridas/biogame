#include "Engine.h"

#include <base.h>

#include <Utils/Exception.h>

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <LogRender.h>
#include <Utils/Timer.h>
#include <ActionToInput.h>
#include <Console.h>
#include <GUIManager.h>

#include "HDRPipeline.h"

#include "params.h"

bool CEngine::Init(const SInitParams& _InitParams,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");

  m_pCore = new CCore();

  m_pCore->Init(hWnd, _InitParams);

  if(m_pProcess) //TODO: Comprovar excepcio m_pProcess == NULL i logejar
    m_pProcess->Init(); 

  m_pHDR = new CHDRPipeline();
  if(!m_pHDR->Init(_InitParams.EngineParams.szHDRFile))
  {
    CHECKED_DELETE( m_pHDR );
  }

  m_pCore->GetActionToInput()->SetProcess(m_pProcess);

  SetOk(true);

  return IsOk();
}

void CEngine::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Release");

  CHECKED_DELETE(m_pHDR);
  CHECKED_DELETE(m_pProcess);
  CHECKED_DELETE(m_pCore);
  LOGGER->SaveLogsInFile();
}

void CEngine::Update()
{
	m_pCore->Update();
	float l_fElapsedTime = m_pCore->GetTimer()->GetElapsedTime();

	if(m_pProcess != NULL)
		m_pProcess->Update(l_fElapsedTime);

  UpdateSystems(l_fElapsedTime);
}

void CEngine::Render()
{
	CRenderManager* l_pRM = m_pCore->GetRenderManager();

  if(m_pProcess != NULL)
  {
    m_pProcess->PreRender(l_pRM);

    if(m_pHDR && m_pHDR->IsActive())
    {
      RenderHDR(l_pRM, m_pProcess);
    } else {
      RenderNoHDR(l_pRM, m_pProcess);
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

  _pRM->SetupMatrices(m_pProcess->GetCamera());
  _pProcess->Render(_pRM);
  _pProcess->PostRender(_pRM);
	_pProcess->DebugInformation();

  CEngine::RenderSystems(_pRM);

	_pRM->EndRendering();
}


void CEngine::RenderSystems(CRenderManager* _pRM)
{
  CLogRender* l_pLR = m_pCore->GetLogRender();
  if(l_pLR)
    l_pLR->Render(_pRM,m_pCore->GetFontManager());

  CConsole* l_pC = m_pCore->GetConsole();
  if(l_pC)
    l_pC->Render(_pRM,m_pCore->GetFontManager());

  CGUIManager* l_pGUI = m_pCore->GetGUIManager();
  if(l_pGUI)
    l_pGUI->Render(_pRM,m_pCore->GetFontManager());
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

void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}