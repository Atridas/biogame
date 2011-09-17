#include "Core.h"
#include "EntityManager.h"
#include "PortalManager.h"
#include "RenderableObjectsManager.h"
#include "StaticMeshManager.h"
#include "IAManager.h"
#include "ScriptManager.h"
#include "PhysicsManager.h"
#include "EmiterManager.h"

#include "LevelChanger.h"


bool CLevelChanger::Init(const string& _szXMLLevels)
{
  //TODO com una casa
  SLevel* l_pLevel = new SLevel;
  
  l_pLevel->StaticMeshes.insert("Data/XML/StaticMeshes.xml");
  l_pLevel->StaticMeshes.insert("Data/Levels/Hangar/XML/StaticMeshes.xml");
  
  l_pLevel->RenderableObjects.insert("Data/Levels/Hangar/XML/RenderableObjects.xml");

  l_pLevel->Portals.insert("Data/Levels/Hangar/XML/Portals.xml");

  l_pLevel->Entities.insert("Data/Levels/Hangar/XML/GameEntities.xml");

  AddResource("Hangar", l_pLevel);

  SetOk(true);

  return IsOk();
}

void CLevelChanger::Update(float _fElapsedTime)
{
  if(m_szNewLevel != "")
  {
    SLevel* l_pLevel = GetResource(m_szNewLevel);
    if(l_pLevel)
    {
      set<string>::iterator l_it;

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
    
    
      l_pCore->m_pEntityManager            = new CEntityManager           ();
      l_pCore->m_pPortalManager            = new CPortalManager           ();
      l_pCore->m_pRenderableObjectsManager = new CRenderableObjectsManager();
      l_pCore->m_pStaticMeshManager        = new CStaticMeshManager       ();
      l_pCore->m_pIAManager                = new CIAManager               ();
      l_pCore->m_pEmiterManager            = new CEmiterManager           ();
      l_pCore->m_pScriptManager            = new CScriptManager           ();
      l_pCore->m_pPhysicsManager           = new CPhysicsManager          ();
      
      

      l_pCore->m_pPhysicsManager->Init(l_szPhysxFile);
      if(l_pCore->m_pPhysicsManager->IsOk())
      {
        l_pCore->m_pPhysicsManager->SetTriggerReport  (l_pCore->GetPhysicTriggerReport());
        l_pCore->m_pPhysicsManager->SetCollisionReport(l_pCore->GetPhysicCollisionReport());
      }
    
      for(l_it = l_pLevel->StaticMeshes.begin(); l_it != l_pLevel->StaticMeshes.end(); ++l_it)
      {
        l_pCore->m_pStaticMeshManager->Load(*l_it);
      }
      for(l_it = l_pLevel->RenderableObjects.begin(); l_it != l_pLevel->RenderableObjects.end(); ++l_it)
      {
        l_pCore->m_pRenderableObjectsManager->Load(*l_it);
      }
      for(l_it = l_pLevel->Portals.begin(); l_it != l_pLevel->Portals.end(); ++l_it)
      {
        l_pCore->m_pPortalManager->Init(*l_it);
      }
    
      l_pCore->m_pScriptManager->Initialize();
      l_pCore->m_pScriptManager->Load(l_pCore->GetLuaInitFile());
   
      l_pCore->m_pIAManager->Init();
      for(l_it = l_pLevel->Entities.begin(); l_it != l_pLevel->Entities.end(); ++l_it)
      {
        l_pCore->m_pEntityManager->LoadEntitiesFromXML(*l_it);
      }

      l_pCore->m_pIAManager->CompleteGraph();

      m_szNewLevel = "";
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING, "Intentant canviar a nivell no definit! \"%s\"", m_szNewLevel.c_str());
    }
  }
}
