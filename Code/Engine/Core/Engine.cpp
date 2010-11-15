#include "Engine.h"

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <base.h>

bool CEngine::Init(const string& _PathXML,  HWND hWnd)
{
  //m_pProcess->Init();
  new CLogger();
  m_pCore = new CCore();
  m_pCore->Init(hWnd); //TODO passar els paràmetres

  return m_bIsOk = true;
}

void CEngine::Relase()
{
  CHECKED_DELETE(m_pProcess);
  CHECKED_DELETE(m_pCore);

  CLogger *pLogger = LOGGER;
  pLogger->SaveLogsInFile();
  CHECKED_DELETE(pLogger);
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