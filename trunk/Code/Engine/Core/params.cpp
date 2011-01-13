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

    //--------------------------------------------------------------------------------------------------------------------------------------------------------
    //Render Manager -----------------------------------------------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------------------------------------------
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

        if(strcmp(l_TreeChild.GetName(), "StaticMeshManager") == 0)
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"StaticMeshManager\" sense parametre \"xml\"");
          } else {
            InitParams_.RenderManagerParams.vRenderableMeshes.push_back(string(l_pcXML));
            LOGGER->AddNewLog(ELL_INFORMATION, "\tStaticMeshes \"%s\"",l_pcXML);
          }
        } 
        else if (strcmp(l_TreeChild.GetName(), "AnimatedModelManager") == 0)
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"AnimatedModelManager\" sense parametre \"xml\"");
          } else {
            InitParams_.RenderManagerParams.vAnimatedModels.push_back(string(l_pcXML));
            LOGGER->AddNewLog(ELL_INFORMATION, "\tAnimatedModels \"%s\"",l_pcXML);
          }
        } else if (strcmp(l_TreeChild.GetName(), "EffectManager") == 0)
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"EffectManager\" sense parametre \"xml\"");
          } else {
            InitParams_.RenderManagerParams.szEffectsXML = string(l_pcXML);
            LOGGER->AddNewLog(ELL_INFORMATION, "\tEffects \"%s\"",l_pcXML);
          }
        }
        else 
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tParametre del RenderManager desconegut : %s", l_TreeChild.GetName());
        }
      }

    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"RenderManager\". Usant valors per defecte.");
    }

    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Font Manager ---------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    CXMLTreeNode l_TreeFontManager = l_TreeConfig["FontManager"];
    if(l_TreeFontManager.Exists())
    {
      InitParams_.FontManagerParams.pcFontsXML = l_TreeFontManager.GetPszProperty("fontsXML",InitParams_.FontManagerParams.pcFontsXML);

      LOGGER->AddNewLog(ELL_INFORMATION, "\tFonts: \"%s\"", InitParams_.FontManagerParams.pcFontsXML);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"FontManager\". Usant valors per defecte.");
    }
  
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    //Language Manager -------------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------------------------------
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

    //------------------------------------------------------------------------------------------------------------------------------------------------------
    //Input Manager ----------------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    CXMLTreeNode l_TreeInputManager = l_TreeConfig["InputManager"];
    if(l_TreeLanguages.Exists())
    {
      InitParams_.InputManagerParams.bExclusiveMouse = l_TreeInputManager.GetBoolProperty("exclusiveMouse", InitParams_.InputManagerParams.bExclusiveMouse);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tMouse exclusiu: %s", (InitParams_.InputManagerParams.bExclusiveMouse)?"cert":"falç");
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"InputManager\". Usant valors per defecte.");
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Action to input ---------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    CXMLTreeNode l_TreeActionToInput = l_TreeConfig["ActionToInput"];
    if(l_TreeActionToInput.Exists())
    {
      InitParams_.ActionToInputParams.pcFile = l_TreeActionToInput.GetPszProperty("file", InitParams_.ActionToInputParams.pcFile);
      LOGGER->AddNewLog(ELL_INFORMATION, "\tAction To input: %s", InitParams_.ActionToInputParams.pcFile);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"ActionToInput\". Usant valors per defecte.");
    }


    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Renderable Objects Manager ----------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    CXMLTreeNode l_TreeRenderableObjectsManager = l_TreeConfig["RenderableObjectsManager"];
    if(l_TreeRenderableObjectsManager.Exists())
    {
      const char* l_pcFile = l_TreeRenderableObjectsManager.GetPszProperty("file", 0);

      if(l_pcFile == 0)
      {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del RenderableObjectsManager.");
      } else {
        InitParams_.RenderableObjectsManager.vXMLFiles.push_back(string(l_pcFile));
        LOGGER->AddNewLog(ELL_INFORMATION, "\tRenderableObjectsManager base \"%s\"",l_pcFile);
      }

      int l_iNumChildren = l_TreeRenderableObjectsManager.GetNumChildren();
      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_TreeChild = l_TreeRenderableObjectsManager(i);

        if(strcmp(l_TreeChild.GetName(), "Level") == 0)
        {
          l_pcFile = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcFile == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"Level\" sense parametre \"xml\"");
          } else {
            InitParams_.RenderableObjectsManager.vXMLFiles.push_back(string(l_pcFile));
            LOGGER->AddNewLog(ELL_INFORMATION, "\tRenderableObjectsManager \"%s\"",l_pcFile);
          }
        } else if(!l_TreeChild.IsComment()) {
          LOGGER->AddNewLog(ELL_WARNING, "\tParametre del RenderableObjectsManager desconegut : %s", l_TreeChild.GetName());
        }
      }

    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"RenderableObjectsManager\". Usant valors per defecte [Res].");
    }


    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Lights Manager ----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    CXMLTreeNode l_TreeLightsManager = l_TreeConfig["LightsManager"];
    if(l_TreeRenderableObjectsManager.Exists())
    {
      const char* l_pcFile = l_TreeLightsManager.GetPszProperty("file", 0);

      if(l_pcFile == 0)
      {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del LightsManager.");
      } else {
        InitParams_.LightsManager.szFile = l_pcFile;
        LOGGER->AddNewLog(ELL_INFORMATION, "\tLightsManager base \"%s\"",l_pcFile);
      }

    } else {
      LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"LightsManager\". Usant valors per defecte [Res].");
    }
  }
  
  LOGGER->AddNewLog(ELL_INFORMATION, "Fi carregar configuració");
}