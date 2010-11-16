#include "Engine.h"

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <base.h>

bool CEngine::Init(const string& _PathXML,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");
  //m_pProcess->Init();
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
  LOGGER->SaveLogsInFile();
}

void CEngine::Update()
{
	if(m_pProcess != NULL)
	{
		m_pProcess->Update();
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
	}
}
void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}