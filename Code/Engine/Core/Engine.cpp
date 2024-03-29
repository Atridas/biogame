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
#include "Renderer.h"
#include "LevelChanger.h"

#include "params.h"

bool CEngine::Init(const SInitParams& _InitParams,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");

  m_pCore = new CCore();

  bool result = m_pCore->Init(hWnd, _InitParams);

  if(!result)
  {
    LOGGER->SaveLogsInFile();
  }

  assert(result);
  
  if(m_pActiveProcess) //TODO: Comprovar excepcio m_pProcess == NULL i logejar
  {
    m_pActiveProcess->Init(); 
    m_pActiveProcess->RegisterLuaFunctions();
  }

  m_pCore->GetActionManager()->SetProcess(m_pActiveProcess);

  SetOk(true);

  return IsOk();
}

void CEngine::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Release");
  
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
  if(m_pCore->GetExit())
  {
    SetExit(true);
    return;
  }
	m_pCore->Update();
  if(!m_pCore->GetLevelChanger()->ChangingLevel())
  {
	  float l_fElapsedTime = m_pCore->GetTimer()->GetElapsedTime();

	  if(m_pActiveProcess != NULL)
		  m_pActiveProcess->Update(l_fElapsedTime);

    UpdateSystems(l_fElapsedTime);
  }
}

void CEngine::Render()
{
  if(m_pActiveProcess != NULL)
  {
    CORE->GetRenderer()->Render(m_pActiveProcess);
  }
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