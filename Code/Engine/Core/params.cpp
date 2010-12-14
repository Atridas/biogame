#include "params.h"

#include <XML/XMLTreeNode.h>

void ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML)
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
      /*CXMLTreeNode l_TreeScreenResolution = l_TreeRenderManager["ScreenResolution"];
      CXMLTreeNode l_TreeWindowsPosition = l_TreeRenderManager["WindowsPosition"];
      CXMLTreeNode l_TreeRendermode = l_TreeRenderManager["Rendermode"];
    
      if(l_TreeScreenResolution.Exists())
      {
        //InitParams_.RenderManagerParams.uiWidth = l_TreeScreenResolution.GetIntProperty("width",InitParams_.RenderManagerParams.uiWidth);
        //InitParams_.RenderManagerParams.uiHeight = l_TreeScreenResolution.GetIntProperty("height",InitParams_.RenderManagerParams.uiHeight);

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
      }*/

      InitParams_.RenderManagerParams.v2iResolution = l_TreeRenderManager.GetVect2iProperty("resolution",InitParams_.RenderManagerParams.v2iResolution);
      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Screen resolution: %dx%d",InitParams_.RenderManagerParams.v2iResolution.x,InitParams_.RenderManagerParams.v2iResolution.y);

      InitParams_.RenderManagerParams.v2iPosition = l_TreeRenderManager.GetVect2iProperty("position",InitParams_.RenderManagerParams.v2iPosition);
      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Screen position: %dx%d",InitParams_.RenderManagerParams.v2iPosition.x,InitParams_.RenderManagerParams.v2iPosition.y);
      
      InitParams_.RenderManagerParams.bFullscreen = l_TreeRenderManager.GetBoolProperty("fullscreenMode",InitParams_.RenderManagerParams.bFullscreen);
      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Fullscreen: %s",InitParams_.RenderManagerParams.bFullscreen? "true":"false");

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

    //---------------------------------------------------------------
    //Input Manager -------------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeInputManager = l_TreeConfig["InputManager"];
    if(l_TreeLanguages.Exists())
    {
      InitParams_.InputManagerParams.bExclusiveMouse = l_TreeInputManager.GetBoolProperty("exclusiveMouse", InitParams_.InputManagerParams.bExclusiveMouse);
      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Mouse exclusiu: %s", (InitParams_.InputManagerParams.bExclusiveMouse)?"cert":"falç");
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"InputManager\". Usant valors per defecte.");
    }

    //---------------------------------------------------------------
    //Action to input -----------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeActionToInput = l_TreeConfig["ActionToInput"];
    if(l_TreeActionToInput.Exists())
    {
      InitParams_.ActionToInputParams.pcFile = l_TreeActionToInput.GetPszProperty("file", InitParams_.ActionToInputParams.pcFile);
      LOGGER->AddNewLog(ELL_INFORMATION, "Engine:: Action To input: %s", InitParams_.ActionToInputParams.pcFile);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "Engine:: No s'ha trobat element \"ActionToInput\". Usant valors per defecte.");
    }
  }
}