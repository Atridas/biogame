#include "Core.h"
#include "EntityManager.h"
#include "PortalManager.h"
#include "RenderableObjectsManager.h"
#include "StaticMeshManager.h"
#include "IAManager.h"
#include "ScriptManager.h"
#include "PhysicsManager.h"
#include "EmiterManager.h"
#include "Renderer.h"
#include "LightManager.h"
#include "SoundManager.h"
#include "Utils/Timer.h"
#include "XML/XMLTreeNode.h"

#include "LevelChanger.h"


bool CLevelChanger::Init(const string& _szXMLLevels)
{
  CXMLTreeNode l_xmlLevels;

  if(!l_xmlLevels.LoadFile(_szXMLLevels.c_str()))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CLevelChanger::Init Error al carregar el fitxer XML %s",_szXMLLevels.c_str());
    SetOk(false);
  }
  else
  {
    if(strcmp(l_xmlLevels.GetName(), "Levels") != 0)
    {
      LOGGER->AddNewLog(ELL_ERROR,"CLevelChanger::Init Element arrel es %s i hauria de ser Levels",l_xmlLevels.GetName());
    }

    int l_numChild = l_xmlLevels.GetNumChildren();
    for(int i = 0; i < l_numChild; ++i)
    {
      CXMLTreeNode l_xmlLevel = l_xmlLevels(i);
      if(strcmp(l_xmlLevel.GetName(), "Level") == 0)
      {
        SLevel* l_pLevel = new SLevel;
        string l_szName = l_xmlLevel.GetPszISOProperty("name");
        if(m_szCurrentLevel != "")
          m_szCurrentLevel = l_szName;
        l_pLevel->RenderPath = l_xmlLevel.GetPszISOProperty("render_path");

        l_pLevel->Script = l_xmlLevel.GetPszISOProperty("script", "", false);

        int l_numChild = l_xmlLevel.GetNumChildren();
        for(int i = 0; i < l_numChild; ++i)
        {
          CXMLTreeNode l_xmlElement = l_xmlLevel(i);
          if(strcmp(l_xmlElement.GetName(), "StaticMesh") == 0)
          {
            l_pLevel->StaticMeshes.push_back( l_xmlElement.GetPszISOProperty("file") );
          }
          else if(strcmp(l_xmlElement.GetName(), "RenderableObjects") == 0)
          {
            l_pLevel->RenderableObjects.push_back( l_xmlElement.GetPszISOProperty("file") );
          }
          else if(strcmp(l_xmlElement.GetName(), "Portals") == 0)
          {
            l_pLevel->Portals.push_back( l_xmlElement.GetPszISOProperty("file") );
          }
          else if(strcmp(l_xmlElement.GetName(), "Entities") == 0)
          {
            l_pLevel->Entities.push_back( l_xmlElement.GetPszISOProperty("file") );
          }
          else if(strcmp(l_xmlElement.GetName(), "Lights") == 0)
          {
            l_pLevel->Lights.push_back( l_xmlElement.GetPszISOProperty("file") );
          }
          else if(!l_xmlElement.IsComment())
          {
            LOGGER->AddNewLog(ELL_ERROR,"CLevelChanger::Init Element no reconegut %s",l_xmlElement.GetName());
          }
        }
        AddResource(l_szName, l_pLevel);
      }
      else if(!l_xmlLevel.IsComment())
      {
        LOGGER->AddNewLog(ELL_ERROR,"CLevelChanger::Init Element no reconegut %s",l_xmlLevel.GetName());
      }
    }
    SetOk(true);
  }


  return IsOk();
}

void CLevelChanger::Update(float _fElapsedTime)
{
  //Ja estem canviant de nivell
  if(m_bChanging)
  {
    SLevel* l_pLevel = GetResource(m_szNewLevel);
    CCore* l_pCore = CORE;

    string l_szPhysxFile = l_pCore->m_pPhysicsManager->GetConfigFileName();
    //m_pPhysicsManager->Done();

    
    CHECKED_DELETE( l_pCore->m_pEntityManager            );
    CHECKED_DELETE( l_pCore->m_pPortalManager            );
    CHECKED_DELETE( l_pCore->m_pEmiterManager            );
    CHECKED_DELETE( l_pCore->m_pRenderableObjectsManager );
    CHECKED_DELETE( l_pCore->m_pStaticMeshManager        );
    CHECKED_DELETE( l_pCore->m_pIAManager                );
    CHECKED_DELETE( l_pCore->m_pScriptManager            );
    CHECKED_DELETE( l_pCore->m_pPhysicsManager           );
    //CHECKED_DELETE( l_pCore->m_pLightManager             );
    
    
    l_pCore->m_pEntityManager            = new CEntityManager           ();
    l_pCore->m_pPortalManager            = new CPortalManager           ();
    l_pCore->m_pRenderableObjectsManager = new CRenderableObjectsManager();
    l_pCore->m_pStaticMeshManager        = new CStaticMeshManager       ();
    l_pCore->m_pIAManager                = new CIAManager               ();
    l_pCore->m_pEmiterManager            = new CEmiterManager           ();
    l_pCore->m_pScriptManager            = new CScriptManager           ();
    l_pCore->m_pPhysicsManager           = new CPhysicsManager          ();
    //l_pCore->m_pLightManager             = new CLightManager            ();
    l_pCore->m_pLightManager->Done();
      

    l_pCore->m_pPhysicsManager->Init(l_szPhysxFile);
    if(l_pCore->m_pPhysicsManager->IsOk())
    {
      l_pCore->m_pPhysicsManager->SetTriggerReport  (l_pCore->GetPhysicTriggerReport());
      l_pCore->m_pPhysicsManager->SetCollisionReport(l_pCore->GetPhysicCollisionReport());
    }

    
    l_pCore->m_pScriptManager->Initialize();
    l_pCore->m_pScriptManager->Load(l_pCore->GetLuaInitFile());
    //Creem l'entitat de control de nivell.
    //TODO: Comprovar si s'està al main menu i enviar true o false
    l_pCore->m_pEntityManager->CreateLevelControllerEntity(false);

    vector<string>::iterator l_it;
    for(l_it = l_pLevel->StaticMeshes.begin(); l_it != l_pLevel->StaticMeshes.end(); ++l_it)
    {
      l_pCore->m_pStaticMeshManager->Load(*l_it);
    }
    for(l_it = l_pLevel->RenderableObjects.begin(); l_it != l_pLevel->RenderableObjects.end(); ++l_it)
    {
      l_pCore->m_pRenderableObjectsManager->Load(*l_it);
    }
    l_pCore->m_pPortalManager->Init(l_pLevel->Portals);
    for(l_it = l_pLevel->Lights.begin(); l_it != l_pLevel->Lights.end(); ++l_it)
    {
      l_pCore->m_pLightManager->Load(*l_it);
    }
   
    l_pCore->m_pIAManager->Init();

    //LoadEntities
    for(l_it = l_pLevel->Entities.begin(); l_it != l_pLevel->Entities.end(); ++l_it)
    {
      l_pCore->m_pEntityManager->LoadEntitiesFromXML(*l_it);
    }
    

    l_pCore->m_pIAManager->CompleteGraph();
    l_pCore->m_pTimer->Reset();


    m_bChanging = false;
    m_iCountdown = INITIAL_LEVEL_CHANGER_COUNTDOWN;
  }
  else if(m_iCountdown > 0)
  {
    m_iCountdown--;
    if(m_iCountdown == 0)
    {
      SLevel* l_pLevel = GetResource(m_szNewLevel);
      CCore* l_pCore = CORE;
      CRenderer* l_pRenderer = l_pCore->GetRenderer();
      l_pRenderer->DeactivateRenderPath(l_pLevel->RenderPath);
      for(set<string>::iterator l_it = m_RenderPathsToActivate.begin(); l_it != m_RenderPathsToActivate.end(); ++l_it)
      {
        l_pRenderer->ActivateRenderPath(*l_it);
      }

      if(l_pLevel->Script != "")
      {
        CORE->GetScriptManager()->RunCode(l_pLevel->Script);
      }

      m_szNewLevel = "";
    }
  }
  else if(m_szNewLevel != "")
  {
    SLevel* l_pLevel = GetResource(m_szNewLevel);
    if(l_pLevel)
    {
      CCore* l_pCore = CORE;

      CRenderer* l_pRenderer = l_pCore->GetRenderer();
      l_pRenderer->GetActiveRenderPaths(m_RenderPathsToActivate);
      for(set<string>::iterator l_it = m_RenderPathsToActivate.begin(); l_it != m_RenderPathsToActivate.end(); ++l_it)
      {
        l_pRenderer->DeactivateRenderPath(*l_it);
      }

      l_pRenderer->ActivateRenderPath(l_pLevel->RenderPath);
      l_pCore->GetSoundManager()->StopSounds();
      m_bChanging = true;
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING, "Intentant canviar a nivell no definit! \"%s\"", m_szNewLevel.c_str());
      m_szNewLevel = "";
    }
  }
}
