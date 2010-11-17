#include "Engine.h"

#include <XML/XMLTreeNode.h>

#include <Utils/Exception.h>

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <base.h>
#include "Utils/Timer.h"

#include "params.h"

bool CEngine::Init(const string& _PathXML,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");
  string msg = "Engine:: load XML :";
  msg += _PathXML;
  LOGGER->AddNewLog(ELL_INFORMATION,msg.c_str());
  //m_pProcess->Init();
  m_pTimer = new CTimer(30);
  m_pCore = new CCore();

  CXMLTreeNode l_TreeConfig;

  SInitParams l_InitParams;

  if(!l_TreeConfig.LoadFile(_PathXML.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"Engine:: No s'ha trobat el XML");
  } else 
  {
    //llegir XML
    
    //Render Manager ------------------------------------------------
    CXMLTreeNode l_TreeRenderManager = l_TreeConfig["RenderManager"];
    
    CXMLTreeNode l_TreeScreenResolution = l_TreeRenderManager["ScreenResolution"];
    CXMLTreeNode l_TreeWindowsPosition = l_TreeRenderManager["WindowsPosition"];
    CXMLTreeNode l_TreeRendermode = l_TreeRenderManager["Rendermode"];
    
    l_InitParams.m_RenderManagerParams.m_uiWidth = l_TreeScreenResolution.GetIntProperty("width",l_InitParams.m_RenderManagerParams.m_uiWidth);
    l_InitParams.m_RenderManagerParams.m_uiHeight = l_TreeScreenResolution.GetIntProperty("height",l_InitParams.m_RenderManagerParams.m_uiHeight);

    l_InitParams.m_RenderManagerParams.m_uiPositionWidth = l_TreeWindowsPosition.GetIntProperty("positionWidth",l_InitParams.m_RenderManagerParams.m_uiPositionWidth);
    l_InitParams.m_RenderManagerParams.m_uiPositionHeight = l_TreeWindowsPosition.GetIntProperty("positionHeight",l_InitParams.m_RenderManagerParams.m_uiPositionHeight);
    
    l_InitParams.m_RenderManagerParams.m_bFullscreen = l_TreeRendermode.GetBoolProperty("fullscreenMode",l_InitParams.m_RenderManagerParams.m_bFullscreen);

   
  }

  m_pCore->Init(hWnd, l_InitParams); //TODO passar els paràmetres

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