#include "Engine.h"

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <base.h>

bool CEngine::Init(const string& _PathXML,  HWND hWnd)
{
  //m_pProcess->Init();
  m_pCore = CCore::GetInstance();
  m_pCore->Init(hWnd); //TODO passar els paràmetres

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
	CRenderManager * rm = m_pCore->GetRenderManager();
	rm->BeginRendering();
	rm->SetupMatrices();
	{
		RenderScene(rm);
	}
	rm->EndRendering();
}
void CEngine::RenderScene (CRenderManager* rm)
{
	if(m_pProcess != NULL)
	{
		m_pProcess->Render();
		CColor color(1.f,1.f,1.f);
		rm->DrawLine(Vect3f(0.f,0.f,0.f), Vect3f(0.f,100.f,0.f), color );
	}
}
void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}