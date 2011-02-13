#include "Engine.h"

#include <base.h>

#include <Utils/Exception.h>

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <Utils/Timer.h>
#include <ActionToInput.h>

#include "params.h"

bool CEngine::Init(const SInitParams& _InitParams,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");

  m_pCore = new CCore();

  m_pCore->Init(hWnd, _InitParams);

  if(m_pProcess) //TODO: Comprovar excepcio m_pProcess == NULL i logejar
    m_pProcess->Init(); 

  m_pCore->GetActionToInput()->SetProcess(m_pProcess);

  SetOk(true);

  return IsOk();
}

void CEngine::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Release");

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

}

void CEngine::Render()
{
	CRenderManager* l_pRM = m_pCore->GetRenderManager();

  if(m_pProcess != NULL)
    m_pProcess->PreRender(l_pRM);

	l_pRM->BeginRendering();

  if(m_pProcess != NULL)
    l_pRM->SetupMatrices(m_pProcess->GetCamera());
	
	RenderScene();

	l_pRM->EndRendering();

  //if(m_pProcess != NULL)
  //{
		//
  //  //m_pProcess->RenderINFO();
  //}

}

void CEngine::RenderScene()
{
	if(m_pProcess != NULL)
	{
    m_pProcess->Render(RENDER_MANAGER);
		m_pProcess->DebugInformation();
	}

	
}
void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}