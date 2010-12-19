#include "params.h"

#include <XML/XMLTreeNode.h>

void ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "Carregant el fitxer de configuració de l'Engine \"%s\"", _pcPathXML);


  CXMLTreeNode l_TreeConfig;
  if(!l_TreeConfig.LoadFile(_pcPathXML))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tNo s'ha trobat el Fitxer");
  } else 
  {
    //llegir XML
    LOGGER->AddNewLog(ELL_INFORMATION,"\tLlegint XML");

    //---------------------------------------------------------------
    //Render Manager ------------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeRenderManager = l_TreeConfig["RenderManager"];

    if(l_TreeRenderManager.Exists())
    {

      InitParams_.RenderManagerParams.v2iResolution = l_TreeRenderManager.GetVect2iProperty("resolution",InitParams_.RenderManagerParams.v2iResolution);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tScreen resolution: %dx%d",InitParams_.RenderManagerParams.v2iResolution.x,InitParams_.RenderManagerParams.v2iResolution.y);

      InitParams_.RenderManagerParams.v2iPosition = l_TreeRenderManager.GetVect2iProperty("position",InitParams_.RenderManagerParams.v2iPosition);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tScreen position: %dx%d",InitParams_.RenderManagerParams.v2iPosition.x,InitParams_.RenderManagerParams.v2iPosition.y);
      
      InitParams_.RenderManagerParams.bFullscreen = l_TreeRenderManager.GetBoolProperty("fullscreenMode",InitParams_.RenderManagerParams.bFullscreen);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tFullscreen: %s",InitParams_.RenderManagerParams.bFullscreen? "true":"false");


      int l_iNumChildren = l_TreeRenderManager.GetNumChildren();
      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_TreeChild = l_TreeRenderManager(i);

        if(strcmp(l_TreeChild.GetName(), "\tStaticMeshManager") == 0)
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tParametre \"StaticMeshManager\" sense parametre \"xml\"");
          } else {
            InitParams_.RenderManagerParams.vRenderableMeshes.push_back(string(l_pcXML));
            LOGGER->AddNewLog(ELL_INFORMATION, "StaticMeshes \"%s\"",l_pcXML);
          }
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "\tParametre del RenderManager desconegut : %s", l_TreeChild.GetName());
        }
      }

    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"RenderManager\". Usant valors per defecte.");
    }
    //---------------------------------------------------------------
    //Font Manager --------------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeFontManager = l_TreeConfig["FontManager"];
    if(l_TreeFontManager.Exists())
    {
      InitParams_.FontManagerParams.pcFontsXML = l_TreeFontManager.GetPszProperty("fontsXML",InitParams_.FontManagerParams.pcFontsXML);

      LOGGER->AddNewLog(ELL_INFORMATION, "\tFonts: \"%s\"", InitParams_.FontManagerParams.pcFontsXML);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"FontManager\". Usant valors per defecte.");
    }
  
    //---------------------------------------------------------------
    //Language Manager ----------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeLanguages = l_TreeConfig["Languages"];
    if(l_TreeLanguages.Exists())
    {
      InitParams_.LanguageManagerParams.pcDefault = l_TreeLanguages.GetPszProperty("default", InitParams_.LanguageManagerParams.pcDefault);

      LOGGER->AddNewLog(ELL_INFORMATION, "\tDefault Language: \"%s\"", InitParams_.LanguageManagerParams.pcDefault);

      int l_iNumLanguages = l_TreeLanguages.GetNumChildren();
      for(int i = 0; i < l_iNumLanguages; i++)
      {
        string l_szXMLFile = l_TreeLanguages(i).GetPszProperty("languageXML", "");
        if(l_szXMLFile != "")
        {
          InitParams_.LanguageManagerParams.vXMLFiles.push_back(l_szXMLFile);
          LOGGER->AddNewLog(ELL_INFORMATION, "\tXML Language file: \"%s\"", l_szXMLFile.c_str());
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "\tError reading %d xml language entry. No file is added to the list.", i);
        }
      }
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"Languages\". Usant valors per defecte.");
    }

    //---------------------------------------------------------------
    //Input Manager -------------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeInputManager = l_TreeConfig["InputManager"];
    if(l_TreeLanguages.Exists())
    {
      InitParams_.InputManagerParams.bExclusiveMouse = l_TreeInputManager.GetBoolProperty("exclusiveMouse", InitParams_.InputManagerParams.bExclusiveMouse);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tMouse exclusiu: %s", (InitParams_.InputManagerParams.bExclusiveMouse)?"cert":"falç");
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"InputManager\". Usant valors per defecte.");
    }

    //---------------------------------------------------------------
    //Action to input -----------------------------------------------
    //---------------------------------------------------------------
    CXMLTreeNode l_TreeActionToInput = l_TreeConfig["ActionToInput"];
    if(l_TreeActionToInput.Exists())
    {
      InitParams_.ActionToInputParams.pcFile = l_TreeActionToInput.GetPszProperty("file", InitParams_.ActionToInputParams.pcFile);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tAction To input: %s", InitParams_.ActionToInputParams.pcFile);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"ActionToInput\". Usant valors per defecte.");
    }
  }
  
  LOGGER->AddNewLog(ELL_INFORMATION, "Fi carregar configuració");
}