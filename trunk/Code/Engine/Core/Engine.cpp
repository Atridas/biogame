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

  m_pTimer = new CTimer(30);
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
  CHECKED_DELETE(m_pTimer);
  LOGGER->SaveLogsInFile();
}

void CEngine::Update()
{
	

	m_pTimer->Update();

	float elapsedTime = m_pTimer->GetElapsedTime();

	m_pCore->Update(elapsedTime);

	//m_pCurrentProcess->Update(elapsedTime);
	//m_pProcess->Update(elapsedTime);
	if(m_pProcess != NULL)
	{
		m_pProcess->Update(elapsedTime);
	}


}

void CEngine::Render()
{
	CRenderManager * rm = m_pCore->GetRenderManager();
	rm->BeginRendering();
  rm->SetupMatrices(m_pProcess->GetCamera());

  m_pCore->Render();
	{
		RenderScene();
	}
	rm->EndRendering();


}

void CEngine::RenderScene()
{
	if(m_pProcess != NULL)
	{
    m_pProcess->PreRender(RENDER_MANAGER);
		m_pProcess->Render(RENDER_MANAGER);
		//Mostrem la informació de Debug (en aquest cas els FPS del timer)
    float l_fFrameRate = m_pTimer->GetFPS();
		m_pProcess->DebugInformation(l_fFrameRate);
	}

	
}
void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}