#include "Engine.h"

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <base.h>
#include "Utils/Timer.h"

bool CEngine::Init(const string& _PathXML,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");
  //m_pProcess->Init();
  m_pTimer = new CTimer(30);
  m_pCore = new CCore();
  m_pCore->Init(hWnd); //TODO passar els paràmetres

 

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
	rm->SetupMatrices();
	{
		RenderScene();
	}
	rm->EndRendering();


}

void CEngine::RenderScene()
{
	if(m_pProcess != NULL)
	{
		m_pProcess->Render();
		//Mostrem la informació de Debug (en aquest cas els FPS del timer)
		m_pProcess->DebugInformation(m_pTimer->GetFPS());
	}

	
}
void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}