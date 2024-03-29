#include "params.h"

#include <time.h>
#include <XML/XMLTreeNode.h>

void ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "Carregant el fitxer de configuraci� de l'Engine \"%s\"", _pcPathXML);


  CXMLTreeNode l_TreeConfig;
  if(!l_TreeConfig.LoadFile(_pcPathXML))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tNo s'ha trobat el Fitxer");
  } else 
  {
    //llegir XML
    LOGGER->AddNewLog(ELL_INFORMATION,"\tLlegint XML");
    
    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Engine ---------------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_Engine = l_TreeConfig["Engine"];
      if(l_Engine.Exists())
      {
        InitParams_.EngineParams.szHDRFile = l_Engine.GetPszProperty("HDR",InitParams_.EngineParams.szHDRFile.c_str());

        LOGGER->AddNewLog(ELL_INFORMATION, "\tHDR: \"%s\"", InitParams_.EngineParams.szHDRFile);
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"Engine\". Usant valors per defecte.");
      }
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------------
    //Render Manager -----------------------------------------------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeRenderManager = l_TreeConfig["RenderManager"];

      if(l_TreeRenderManager.Exists())
      {

        InitParams_.RenderManagerParams.v2iResolution = l_TreeRenderManager.GetVect2iProperty("resolution",InitParams_.RenderManagerParams.v2iResolution);
        LOGGER->AddNewLog(ELL_INFORMATION, "\tScreen resolution: %dx%d",InitParams_.RenderManagerParams.v2iResolution.x,InitParams_.RenderManagerParams.v2iResolution.y);

        InitParams_.RenderManagerParams.v2iPosition = l_TreeRenderManager.GetVect2iProperty("position",InitParams_.RenderManagerParams.v2iPosition);
        LOGGER->AddNewLog(ELL_INFORMATION, "\tScreen position: %dx%d",InitParams_.RenderManagerParams.v2iPosition.x,InitParams_.RenderManagerParams.v2iPosition.y);
      
        InitParams_.RenderManagerParams.bFullscreen = l_TreeRenderManager.GetBoolProperty("fullscreenMode",InitParams_.RenderManagerParams.bFullscreen);
        LOGGER->AddNewLog(ELL_INFORMATION, "\tFullscreen: %s",InitParams_.RenderManagerParams.bFullscreen? "true":"false");

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"RenderManager\". Usant valors per defecte.");
      }
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Static Mesh Manager --------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeStaticMeshManager = l_TreeConfig["StaticMeshManager"];
      int l_iNumChildren = l_TreeStaticMeshManager.GetNumChildren();
      
      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_TreeChild = l_TreeStaticMeshManager(i);

        if(!l_TreeChild.IsComment())
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"StaticMeshManager\" sense parametre \"xml\"");
          } else {
            InitParams_.StaticMeshManagerParams.vRenderableMeshes.push_back(string(l_pcXML));
            LOGGER->AddNewLog(ELL_INFORMATION, "\tStaticMeshes \"%s\"",l_pcXML);
          }
        } 
      }
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Animated Model Manager -----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeAnimatedModelManager = l_TreeConfig["AnimatedModelManager"];
      int l_iNumChildren = l_TreeAnimatedModelManager.GetNumChildren();
      
      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_TreeChild = l_TreeAnimatedModelManager(i);

        if(!l_TreeChild.IsComment())
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"AnimatedModelManager\" sense parametre \"xml\"");
          } else {
            InitParams_.AnimatedModelManagerParams.vAnimatedModels.push_back(string(l_pcXML));
            LOGGER->AddNewLog(ELL_INFORMATION, "\tAnimatedModels \"%s\"",l_pcXML);
          }
        } 
      }
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Effect Manager -------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeEffectManager = l_TreeConfig["EffectManager"];
      int l_iNumChildren = l_TreeEffectManager.GetNumChildren();
      
      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_TreeChild = l_TreeEffectManager(i);

        if(!l_TreeChild.IsComment())
        {
          const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
          if(l_pcXML == 0)
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement \"EffectManager\" sense parametre \"xml\"");
          } else {
            InitParams_.EffectManagerParams.szFile = string(l_pcXML);
            LOGGER->AddNewLog(ELL_INFORMATION, "\tEffects \"%s\"",l_pcXML);
          }
        } 
      }
    }


    //-----------------------------------------------------------------------------------------------------------------------------------------------
    //Paticle Manager -------------------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeParticleManager = l_TreeConfig["ParticleManager"];
      if(l_TreeParticleManager.Exists())
      {
        int l_iNumChildren = l_TreeParticleManager.GetNumChildren();
      
        for(int i = 0; i < l_iNumChildren; i++)
        {
          CXMLTreeNode l_TreeChild = l_TreeParticleManager(i);

          if(!l_TreeChild.IsComment())
          {
            const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
            if(l_pcXML == 0)
            {
              LOGGER->AddNewLog(ELL_WARNING, "\tElement \"ParticleManager\" sense parametre \"xml\"");
            } else
            {
              InitParams_.PaticleManagerParams.szFile = string(l_pcXML);
              LOGGER->AddNewLog(ELL_INFORMATION, "\tParticle \"%s\"",l_pcXML);
            }
          } 
        }
      }
    }


     //-----------------------------------------------------------------------------------------------------------------------------------------------
    //BillBoard Manager -------------------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeParticleManager = l_TreeConfig["BillBoardManager"];
      if(l_TreeParticleManager.Exists())
      {
        int l_iNumChildren = l_TreeParticleManager.GetNumChildren();
      
        for(int i = 0; i < l_iNumChildren; i++)
        {
          CXMLTreeNode l_TreeChild = l_TreeParticleManager(i);

          if(!l_TreeChild.IsComment())
          {
            const char* l_pcXML = l_TreeChild.GetPszProperty("xml",0);
            if(l_pcXML == 0)
            {
              LOGGER->AddNewLog(ELL_WARNING, "\tElement \"BillBoardManager\" sense parametre \"xml\"");
            } else
            {
              InitParams_.PaticleManagerParams.szFile = string(l_pcXML);
              LOGGER->AddNewLog(ELL_INFORMATION, "\tBillBoard \"%s\"",l_pcXML);
            }
          } 
        }
      }
    }


    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Font Manager ---------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeFontManager = l_TreeConfig["FontManager"];
      if(l_TreeFontManager.Exists())
      {
        InitParams_.FontManagerParams.pcFontsXML = l_TreeFontManager.GetPszProperty("fontsXML",InitParams_.FontManagerParams.pcFontsXML);

        LOGGER->AddNewLog(ELL_INFORMATION, "\tFonts: \"%s\"", InitParams_.FontManagerParams.pcFontsXML);
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"FontManager\". Usant valors per defecte.");
      }
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    //Language Manager -------------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    {
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
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    //Input Manager ----------------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeInputManager = l_TreeConfig["InputManager"];
      if(l_TreeInputManager.Exists())
      {
        InitParams_.InputManagerParams.bExclusiveMouse = l_TreeInputManager.GetBoolProperty("exclusiveMouse", InitParams_.InputManagerParams.bExclusiveMouse);
        InitParams_.InputManagerParams.fSensitivity = l_TreeInputManager.GetFloatProperty("sensitivity", InitParams_.InputManagerParams.fSensitivity);
        LOGGER->AddNewLog(ELL_INFORMATION, "\tMouse exclusiu: %s", (InitParams_.InputManagerParams.bExclusiveMouse)?"cert":"fal�");
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat element \"InputManager\". Usant valors per defecte.");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Action to input ---------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeActionToInput = l_TreeConfig["ActionToInput"];
      if(l_TreeActionToInput.Exists())
      {
        InitParams_.ActionToInputParams.pcFile = l_TreeActionToInput.GetPszProperty("file", InitParams_.ActionToInputParams.pcFile);
        LOGGER->AddNewLog(ELL_INFORMATION, "\tAction To input: %s", InitParams_.ActionToInputParams.pcFile);
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"ActionToInput\". Usant valors per defecte.");
      }
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Renderable Objects Manager ----------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeRenderableObjectsManager = l_TreeConfig["RenderableObjectsManager"];
      if(l_TreeRenderableObjectsManager.Exists())
      {
        const char* l_pcFile = l_TreeRenderableObjectsManager.GetPszProperty("file", 0);

        if(l_pcFile == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del RenderableObjectsManager.");
        } else {
          InitParams_.RenderableObjectsManagerParams.vXMLFiles.push_back(string(l_pcFile));
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
              InitParams_.RenderableObjectsManagerParams.vXMLFiles.push_back(string(l_pcFile));
              LOGGER->AddNewLog(ELL_INFORMATION, "\tRenderableObjectsManager \"%s\"",l_pcFile);
            }
          } else if(!l_TreeChild.IsComment()) {
            LOGGER->AddNewLog(ELL_WARNING, "\tParametre del RenderableObjectsManager desconegut : %s", l_TreeChild.GetName());
          }
        }

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"RenderableObjectsManager\". Usant valors per defecte [Res].");
      }
    }
    
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Lights Manager ----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeLightsManager = l_TreeConfig["LightsManager"];
      if(l_TreeLightsManager.Exists())
      {
        const char* l_pcFile = l_TreeLightsManager.GetPszProperty("file", 0);

        if(l_pcFile == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del LightsManager.");
        } else {
          InitParams_.LightsManagerParams.szFile = l_pcFile;
          LOGGER->AddNewLog(ELL_INFORMATION, "\tLightsManager base \"%s\"",l_pcFile);
        }

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"LightsManager\". Usant valors per defecte.");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Scene Effect Manager ----------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeSceneEffectManager = l_TreeConfig["SceneEffectManager"];
      if(l_TreeSceneEffectManager.Exists())
      {
        const char* l_pcFile = l_TreeSceneEffectManager.GetPszProperty("file", 0);

        if(l_pcFile == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del SceneEffectManager.");
        } else {
          InitParams_.SceneEffectParams.szFile = l_pcFile;
          LOGGER->AddNewLog(ELL_INFORMATION, "\tSceneEffectManager base \"%s\"",l_pcFile);
        }

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"SceneEffectManager\". Usant valors per defecte.");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Script Manager ----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeScriptManager = l_TreeConfig["ScriptManager"];
      if(l_TreeScriptManager.Exists())
      {
        const char* l_pcFile = l_TreeScriptManager.GetPszProperty("file", 0);

        if(l_pcFile == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del ScriptManager.");
        } else {
          InitParams_.ScriptManagerParams.szFile = l_pcFile;
          LOGGER->AddNewLog(ELL_INFORMATION, "\tScriptManager base \"%s\"",l_pcFile);
        }

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"ScriptManager\". Usant valors per defecte.");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Log Render --------------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeLogRender = l_TreeConfig["LogRender"];
      if(l_TreeLogRender.Exists())
      {
        InitParams_.LogRenderParams.vPosition = l_TreeLogRender.GetVect2iProperty("position", InitParams_.LogRenderParams.vPosition);

        LOGGER->AddNewLog(ELL_INFORMATION, "\tLogRender (%d, %d)",InitParams_.LogRenderParams.vPosition.x,InitParams_.LogRenderParams.vPosition.y);
      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"LogRender\". Usant valors per defecte.");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //GUI Manager -------------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeGUIManager = l_TreeConfig["GUIManager"];
      if(l_TreeGUIManager.Exists())
      {
        const char* l_pcFile = l_TreeGUIManager.GetPszProperty("xml", 0);

        if(l_pcFile == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha fitxer base del GUI Manager.");
        } else {
          InitParams_.GUIManagerParams.szXML = l_pcFile;
          LOGGER->AddNewLog(ELL_INFORMATION, "\tGUI Manager base \"%s\"",l_pcFile);
        }

        const char* l_pcWindow = l_TreeGUIManager.GetPszProperty("start", 0);

        if(l_pcFile == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha finestra inicial del GUI Manager.");
        } else {
          InitParams_.GUIManagerParams.szInitWindow = l_pcWindow;
          LOGGER->AddNewLog(ELL_INFORMATION, "\tGUI Manager finestra inicial \"%s\"",l_pcWindow);
        }

        InitParams_.GUIManagerParams.bRenderMouse = l_TreeGUIManager.GetBoolProperty("render_mouse", false);

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"GUIManager\". Usant valors per defecte.");
      }
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Sound Manager -----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeSoundManager = l_TreeConfig["SoundManager"];
      if(l_TreeSoundManager.Exists())
      {
        string l_pcFile = l_TreeSoundManager.GetPszISOProperty("file", InitParams_.SoundManagerParams.szFile.c_str(), true);

        InitParams_.SoundManagerParams.szFile = l_pcFile;
        LOGGER->AddNewLog(ELL_INFORMATION, "\tSoundManager base \"%s\"",l_pcFile.c_str());

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"SoundManager\". Usant valors per defecte.");
      }
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //PhysX Manager -----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_PhysXManager = l_TreeConfig["PhysXManager"];
      if(l_PhysXManager.Exists())
      {
        string l_pcFile = l_PhysXManager.GetPszISOProperty("file", InitParams_.PhysXManagerParams.szFile.c_str(), true);

        InitParams_.PhysXManagerParams.szFile = l_pcFile;
        LOGGER->AddNewLog(ELL_INFORMATION, "\tPhysXManager base \"%s\"",l_pcFile.c_str());

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"PhysXManager\". Usant valors per defecte.");
      }
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Portal Manager -----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_xmlPortalManager = l_TreeConfig["PortalManager"];
      if(l_xmlPortalManager.Exists())
      {
        if(l_xmlPortalManager.ExistsProperty("file"))
        {
          string l_szFile = l_xmlPortalManager.GetPszISOProperty("file", "", true);

          InitParams_.PortalManagerParams.szFiles.push_back( l_szFile );
          LOGGER->AddNewLog(ELL_INFORMATION, "\tPortalManager base \"%s\"",l_szFile.c_str());
        }
        for(int i = 0; i < l_xmlPortalManager.GetNumChildren(); ++i)
        {
          CXMLTreeNode l_xmlLevel = l_xmlPortalManager(i);
          if(strcmp(l_xmlLevel.GetName(), "Level") == 0)
          {
            string l_szFile = l_xmlLevel.GetPszISOProperty("file", "", true);

            InitParams_.PortalManagerParams.szFiles.push_back( l_szFile );
            LOGGER->AddNewLog(ELL_INFORMATION, "\tPortalManager base \"%s\"",l_szFile.c_str());
          }
          else if(!l_xmlLevel.IsComment())
          {
            LOGGER->AddNewLog(ELL_WARNING, "\tElement extrany \"%s\".", l_xmlLevel.GetName());
          }

        }

        if(InitParams_.PortalManagerParams.szFiles.size() == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "\tNo hi ha portals valids, inicialitzant per defecte \"Data/XML/Empty Portals.xml\".");
          InitParams_.PortalManagerParams.szFiles.push_back("Data/XML/Empty Portals.xml");
        }

      } else {
        LOGGER->AddNewLog(ELL_WARNING, "\tNo s'ha trobat l'element \"PortalManager\". Usant valors per defecte.");
      }
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Core Emiter Manager -----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_TreeCoreEmiterManager = l_TreeConfig["EmiterCorelManager"];
      if(l_TreeCoreEmiterManager.Exists())
      {
        int l_iNumChildren = l_TreeCoreEmiterManager.GetNumChildren();
      
        for(int i = 0; i < l_iNumChildren; i++)
        {
          CXMLTreeNode l_TreeChild = l_TreeCoreEmiterManager(i);

          if(strcmp("Core", l_TreeChild.GetName()) == 0)
          {
            string l_szFile = l_TreeChild.GetPszProperty("file","Data/XML/Emiters.xml");

            if(InitParams_.CoreEmiterManagerParams.sFiles.find(l_szFile) == InitParams_.CoreEmiterManagerParams.sFiles.end())
            {
              InitParams_.CoreEmiterManagerParams.sFiles.insert(l_szFile);
              LOGGER->AddNewLog(ELL_INFORMATION, "\tFitxer \"%s\"", l_szFile.c_str());
            }
            else
            {
              LOGGER->AddNewLog(ELL_WARNING, "\tRepetint fitxer \"%s\"", l_szFile.c_str());
            }
          } 
          else if(!l_TreeChild.IsComment())
          {
            LOGGER->AddNewLog(ELL_WARNING, "No s'ha identificat l'element \"%s\"", l_TreeChild.GetName());
          }
        }
      }
      else
      {
        LOGGER->AddNewLog(ELL_WARNING, "No s'ha trobat l'element \"CoreEmiterlManager\", insertant \"Data/XML/CoreEmiters.xml\" per defecte.");
        InitParams_.CoreEmiterManagerParams.sFiles.insert("Data/XML/CoreEmiters.xml");
      }
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------
    //Level Changer -----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_LevelsManager = l_TreeConfig["Levels"];
      if(l_LevelsManager.Exists() && l_LevelsManager.ExistsProperty("file"))
      {
        InitParams_.LevelChangerParams.szFile = l_LevelsManager.GetPszISOProperty("file");
      }
      else
      {
        LOGGER->AddNewLog(ELL_WARNING, "No s'ha trobat l'element \"Levels\", usant \"%s\" per defecte.", InitParams_.LevelChangerParams.szFile.c_str());
      }
    }

    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //Random Seed -----------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    {
      CXMLTreeNode l_xmlRandomSeed = l_TreeConfig["RandomSeed"];
      if(l_xmlRandomSeed.Exists())
      {
        InitParams_.RandomSeed = (uint32)l_xmlRandomSeed.GetIntProperty("value", (int)time(0), true);
        
        LOGGER->AddNewLog(ELL_INFORMATION, "\tUsant llavor rnd: %d", InitParams_.RandomSeed);
      } else {
        InitParams_.RandomSeed = (uint32)time(0);
        LOGGER->AddNewLog(ELL_INFORMATION, "\tNo s'ha trobat l'element \"RandomSeed\". Usant temps (%d).", InitParams_.RandomSeed);
      }
    }
  }
  
  LOGGER->AddNewLog(ELL_INFORMATION, "Fi carregar configuraci�");
}