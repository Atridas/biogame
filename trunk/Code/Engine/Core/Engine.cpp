#include "Engine.h"

#include <Core.h>
#include <Process.h>

CEngine::CEngine(void)
{
	m_pCore = NULL;
	m_pProcess = NULL;
}


CEngine::~CEngine(void)
{
	if(m_pProcess != NULL)
	{
		delete m_pProcess;
		m_pProcess = NULL;
	}
}

void CEngine::Update()
{
	m_pProcess->Update();
}

void CEngine::Render()
{
	m_pProcess->Render();
}

void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}