#include "Engine.h"

#include <Core.h>
#include <Process.h>

#include <base.h>

bool CEngine::Init(const string& _PathXML)
{
  //m_pProcess->Init();
  m_pCore = CCore::GetInstance();
  m_pCore->Init(); //TODO passar els paràmetres

  return m_bIsOk = true;
}

void CEngine::Relase()
{
  CHECKED_DELETE(m_pProcess);
  //CCore::GetInstance()->Done();
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
	if(m_pProcess != NULL)
	{
		m_pProcess->Render();
	}
}

void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}