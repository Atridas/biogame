#include "Engine.h"

#include <XML/XMLTreeNode.h>

#include <Utils/Exception.h>

#include <Core.h>
#include <Process.h>
#include <RenderManager.h>
#include <base.h>
#include "Utils/Timer.h"

#include "params.h"

void CEngine::ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML)
{
  CXMLTreeNode l_TreeConfig;
  if(!l_TreeConfig.LoadFile(_pcPathXML))
  {
    LOGGER->AddNewLog(ELL_WARNING,"Engine:: No s'ha trobat el XML \"%s\"", _pcPathXML);
  } else 
  {
    //llegir XML
    LOGGER->AddNewLog(ELL_INFORMATION,"Engine:: Llegint XML \"%s\"", _pcPathXML);

    //---------------------------------------------------------------
    //Render Manager ------------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeRenderManager = l_TreeConfig["RenderManager"];

    if(l_TreeRenderManager.Exists())
    {
      CXMLTreeNode l_TreeScreenResolution = l_TreeRenderManager["ScreenResolution"];
      CXMLTreeNode l_TreeWindowsPosition = l_TreeRenderManager["WindowsPosition"];
      CXMLTreeNode l_TreeRendermode = l_TreeRenderManager["Rendermode"];
    
      if(l_TreeScreenResolution.Exists())
      {
        InitParams_.RenderManagerParams.uiWidth = l_TreeScreenResolution.GetIntProperty("width",InitParams_.RenderManagerParams.uiWidth);
        InitParams_.RenderManagerParams.uiHeight = l_TreeScreenResolution.GetIntProperty("height",InitParams_.RenderManagerParams.uiHeight);

        LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Screen resolution: %dx%d",InitParams_.RenderManagerParams.uiWidth,InitParams_.RenderManagerParams.uiHeight);
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"RenderManager::ScreenResolution\". Usant valors per defecte.");
      }

      if(l_TreeWindowsPosition.Exists())
      {
        InitParams_.RenderManagerParams.uiPosX = l_TreeWindowsPosition.GetIntProperty("x",InitParams_.RenderManagerParams.uiPosX);
        InitParams_.RenderManagerParams.uiPosY = l_TreeWindowsPosition.GetIntProperty("y",InitParams_.RenderManagerParams.uiPosY);
        
        LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Screen position: %dx%d",InitParams_.RenderManagerParams.uiPosX,InitParams_.RenderManagerParams.uiPosY);
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"RenderManager::WindowsPosition\". Usant valors per defecte.");
      }

      if(l_TreeRendermode.Exists())
      {
        InitParams_.RenderManagerParams.bFullscreen = l_TreeRendermode.GetBoolProperty("fullscreenMode",InitParams_.RenderManagerParams.bFullscreen);
    
        LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Fullscreen: %s",InitParams_.RenderManagerParams.bFullscreen? "true":"false");
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"RenderManager::Rendermode\". Usant valors per defecte.");
      }
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"RenderManager\". Usant valors per defecte.");
    }
    //---------------------------------------------------------------
    //Font Manager --------------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeFontManager = l_TreeConfig["FontManager"];
    if(l_TreeFontManager.Exists())
    {
      InitParams_.FontManagerParams.pcFontsXML = l_TreeFontManager.GetPszProperty("fontsXML",InitParams_.FontManagerParams.pcFontsXML);

      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Fonts: \"%s\"", InitParams_.FontManagerParams.pcFontsXML);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"FontManager\". Usant valors per defecte.");
    }
  
    //---------------------------------------------------------------
    //Language Manager ----------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeLanguages = l_TreeConfig["Languages"];
    if(l_TreeLanguages.Exists())
    {
      InitParams_.LanguageManagerParams.pcDefault = l_TreeLanguages.GetPszProperty("default", InitParams_.LanguageManagerParams.pcDefault);

      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Default Language: \"%s\"", InitParams_.LanguageManagerParams.pcDefault);

      int l_iNumLanguages = l_TreeLanguages.GetNumChildren();
      for(int i = 0; i < l_iNumLanguages; i++)
      {
        string l_szXMLFile = l_TreeLanguages(i).GetPszProperty("languageXML", "");
        if(l_szXMLFile != "")
        {
          InitParams_.LanguageManagerParams.vXMLFiles.push_back(l_szXMLFile);
          LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: XML Language file: \"%s\"", l_szXMLFile.c_str());
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "Engine:: Error reading %d xml language entry. No file is added to the list.", i);
        }
      }
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"Languages\". Usant valors per defecte.");
    }
  }
}

bool CEngine::Init(const SInitParams& _InitParams,  HWND hWnd)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"Engine::Init");

  m_pTimer = new CTimer(30);
  m_pCore = new CCore();

  m_pCore->Init(hWnd, _InitParams); //TODO passar els paràmetres

  if(m_pProcess) //TODO: Comprovar excepcio m_pProcess == NULL i logejar
    m_pProcess->Init(); 


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
    float l_fFrameRate = m_pTimer->GetFPS();
		m_pProcess->DebugInformation(l_fFrameRate);
	}

	
}
void CEngine::SetProcess(CProcess* _pProcess)
{
	m_pProcess = _pProcess;
}