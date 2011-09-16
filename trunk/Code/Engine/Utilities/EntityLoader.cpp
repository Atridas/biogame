#include "EntityDefines.h"
#include "EntityManager.h"

#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "ComponentPhysXController.h"
#include "ComponentPlayerController.h"
#include "ComponentIAWalkToPlayer.h"
#include "Component3rdPSCamera.h"
#include "ComponentPhysXBox.h"
#include "ComponentPhysXMesh.h"
#include "ComponentCollisionReport.h"
#include "ComponentRenderableObject.h"
#include "ComponentTrigger.h"
#include "ComponentLowCover.h"
#include "ComponentAnimation.h"
#include "ComponentVida.h"
#include "ComponentMirilla.h"
#include "ComponentStateMachine.h"
#include "ComponentIABrain.h"
#include "ComponentHighCover.h"
#include "ComponentDoor.h"
#include "ComponentShield.h"
#include "ComponentArma.h"
#include "ComponentInteractive.h"
#include "ComponentDestroyable.h"
#include "ComponentNavNode.h"
#include "ComponentRagdoll.h"
#include "ComponentEmiter.h"
#include "ComponentSpawner.h"
#include "ComponentLaser.h"
#include "ComponentLifetime.h"
#include "ComponentExplosive.h"
#include "ComponentRotative.h"
#include "ComponentBillboard.h"
#include "ParticleConstants.h"

#include "RenderableObject.h"
#include "PhysicsManager.h"

#include "base.h"
#include "Core.h"

#include <XML/XMLTreeNode.h>
#include <sstream>

void LoadComponentObject3D(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component d'Object3D.");
  if(CComponentObject3D* l_pComponentObject3D = CComponentObject3D::AddToEntity(_pEntity))
  {
    Vect3f l_vPos   = _TreeComponent.GetVect3fProperty("position", Vect3f(0,0,0), false);
    float  l_fYaw   = _TreeComponent.GetFloatProperty("yaw"  , 0.0f, false) * FLOAT_PI_VALUE / 180.0f;
    float  l_fPitch = _TreeComponent.GetFloatProperty("pitch", 0.0f, false) * FLOAT_PI_VALUE / 180.0f;
    float  l_fRoll  = _TreeComponent.GetFloatProperty("roll" , 0.0f, false) * FLOAT_PI_VALUE / 180.0f;
    
    l_pComponentObject3D->SetPosition(l_vPos);
    l_pComponentObject3D->SetYaw(l_fYaw);
    l_pComponentObject3D->SetPitch(l_fPitch);
    l_pComponentObject3D->SetRoll(l_fRoll);
  } else
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }
}

void LoadComponentRenderableObject(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  if(_TreeComponent.ExistsProperty("name"))
  {
    string l_szResource = _TreeComponent.GetPszISOProperty("name");
    string l_szName     = _pEntity->GetName();

    //static
    if(!_TreeComponent.GetBoolProperty("animated",false,false))
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component de Renderable Object Estàtic amb nom \"%s\".", l_szResource.c_str());
      if(!CComponentRenderableObject::AddToEntity(_pEntity, _pEntity->GetName(), l_szResource))
      {
        LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
      }
    } else
    //animated
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component de Renderable Object Animat amb nom \"%s\" i core \"%s\".", l_szName.c_str(), l_szResource.c_str());
      if(!CComponentRenderableObject::AddToEntityWithAnimatedModel(_pEntity, l_szName, _TreeComponent.GetPszISOProperty("name")))
      {
        LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
      }
    }
  } else
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\tFalta parametre \"name\".");
  }
}

void LoadComponentPhysXBox(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  float l_fDensity = _TreeComponent.GetFloatProperty("density", 0.f, false);

  //TODO mascares
  int l_iCollisionMask = GetCollisionGroup(_TreeComponent.GetPszISOProperty("collision_group", "", true));

  if(strcmp(_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"true") == 0)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box física des del Renderable Object.");
    if(!CComponentPhysXBox::AddToEntity(_pEntity, l_fDensity, l_iCollisionMask))
    {
      LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
    }
  } else if(strcmp(_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"false") == 0)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box física des de XML.");
    Vect3f l_vMidpoint = _TreeComponent.GetVect3fProperty("midpoint", Vect3f(), true);
    Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);

    if(!CComponentPhysXBox::AddToEntity( _pEntity, 
                                    l_vSize.x, l_vSize.y, l_vSize.z,
                                    l_vMidpoint.x, l_vMidpoint.y, l_vMidpoint.z,
                                    l_fDensity, l_iCollisionMask)
      )
    {
      LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
    }
  } else
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\tParametre \"fromRenderableObject\" incorrecte.");
  }
}

void LoadComponentPhysXMesh(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  float l_fDensity = _TreeComponent.GetFloatProperty("density", 0.f, false);

  //TODO mascares
  int l_iCollisionMask = GetCollisionGroup(_TreeComponent.GetPszISOProperty("collision_group", "", true));

  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Mesh física des del Renderable Object.");
  if(!CComponentPhysXMesh::AddToEntity(_pEntity, l_fDensity, l_iCollisionMask))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }
}


void LoadComponentTrigger(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Trigger.");

  //TODO mascares
  int l_iCollisionMask = GetCollisionGroup(_TreeComponent.GetPszISOProperty("collision_group", "", true));
  Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);
  string l_szOnEnter = _TreeComponent.GetPszISOProperty("onEnter", "", false);
  string l_szOnExit = _TreeComponent.GetPszISOProperty("onExit", "", false);
  
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\t\t Enter = \"%s\" Exit = \"%s\".",l_szOnEnter.c_str(), l_szOnExit.c_str());


  if(l_szOnEnter == "" && l_szOnExit == "")
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tTrigger sense cap script!.");
  }

  if(!CComponentTrigger::AddToEntity(_pEntity, l_vSize, l_szOnEnter, l_szOnExit, l_iCollisionMask))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }
}

void LoadComponentHighCover(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Cobertura Alta.");
  
  Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);
  int l_iSpots = _TreeComponent.GetIntProperty("cover_size", 1, false);
  
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\t\t HighCover: \"%d\" spots.", l_iSpots);

  if(!CComponentHighCover::AddToEntity(_pEntity, l_vSize, l_iSpots))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }
}

void LoadComponentLowCover(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Cobertura Baixa.");
  
  Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);
  int l_iSpots = _TreeComponent.GetIntProperty("cover_size", 1, false);
  
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\t\t LowCover: \"%d\" spots.", l_iSpots);

  if(!CComponentLowCover::AddToEntity(_pEntity, l_vSize, l_iSpots))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }
}

void LoadComponentDoor(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  
  string l_szName    = _pEntity->GetName();
  bool l_bOpen       = _TreeComponent.GetBoolProperty("open", false, false);
  Vect3f l_vSize     = _TreeComponent.GetVect3fProperty("size", Vect3f(1.0f), false);
  float l_fOpenTime  = _TreeComponent.GetFloatProperty("open_time", 2.0f, false);
  float l_fCloseTime = _TreeComponent.GetFloatProperty("close_time", 0.5f, false);

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Porta amb nom \"%s\" i estat \"%d\"",l_szName.c_str(), l_bOpen);

  //component porta
  if(!CComponentDoor::AddToEntity(_pEntity, l_bOpen, l_vSize, l_fOpenTime, l_fCloseTime))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }

  //màquina d'estats de la porta:
  string l_szInitialState = "";
  if(l_bOpen)
    l_szInitialState = "State_Porta_Open";
  else
    l_szInitialState = "State_Porta_Closed";

  if(!CComponentStateMachine::AddToEntity(_pEntity, l_szInitialState))
  {
    LOGGER->AddNewLog(ELL_WARNING, "\tError al carregar la màquina d'estats del component Door.");
    assert(false);
  }

}

void LoadComponentInteractive(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  
  string l_szName   = _pEntity->GetName();
  string l_szAction = _TreeComponent.GetPszISOProperty("action", "", false);

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Interactiu amb nom \"%s\" i acció \"%s\"",l_szName.c_str(), l_szAction.c_str());


  if(!CComponentInteractive::AddToEntity(_pEntity, l_szAction))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }

}

void LoadComponentDestroyable(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  string l_szName   = _pEntity->GetName();
  string l_szAction = _TreeComponent.GetPszISOProperty("onDestroy", "", false);
  string l_szResource = _TreeComponent.GetPszISOProperty("destroyedResource", "", false);
  float l_fHP = _TreeComponent.GetFloatProperty("hitPoints", 20.f, false);

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Destroyable amb nom \"%s\", acció \"%s\" i model destruit \"%s\".",l_szName.c_str(), l_szAction.c_str(), l_szResource.c_str());

  if(_pEntity->GetComponent<CComponentVida>() == 0)
  {
    if(!CComponentVida::AddToEntity(_pEntity, l_fHP, l_fHP))
    {
      LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component de vida per l'objecte destructible \"%s\"",_pEntity->GetName());
    }
  }

  if(!CComponentDestroyable::AddToEntity(_pEntity, l_szAction, l_szResource))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component destructible de l'objecte destructible \"%s\"",_pEntity->GetName());
  }

}

void LoadComponentExplosive(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  string l_szName   = _pEntity->GetName();

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Explosive amb nom \"%s\".",l_szName.c_str());

  if(!CComponentExplosive::AddToEntity(_pEntity))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component explosiu de l'objecte \"%s\"", _pEntity->GetName());
  }
}

void LoadComponentRotative(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant component de rotació.");
  
  float l_fYaw   = _TreeComponent.GetFloatProperty("yaw", 0.0f, false);
  float l_fPitch = _TreeComponent.GetFloatProperty("pitch", 0.0f, false);
  float l_fRoll  = _TreeComponent.GetFloatProperty("roll", 0.0f, false);

  Vect3f l_vRotation = Vect3f(l_fYaw, l_fPitch, l_fRoll);
  
  if(!CComponentRotative::AddToEntity(_pEntity, l_vRotation))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }
}

void LoadComponentNavNode(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant NavNode.");
  bool l_bAutoroute = _TreeComponent.GetBoolProperty("autoroute", true, false);
  float l_fMaxautoroute = _TreeComponent.GetFloatProperty("maxDistance", -1, false);

  CComponentNavNode *l_pcNavNode;

  if(!(l_pcNavNode = CComponentNavNode::AddToEntity(_pEntity, l_bAutoroute, l_fMaxautoroute)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component NavNode de l'objecte \"%s\"",_pEntity->GetName());
  }
  else
  {
    if(_TreeComponent.GetPszISOProperty("cobertura", "", false) == "baixa")
    {
      l_pcNavNode->m_bCoberturaBaixa = true;
    }
  }
}


void LoadComponentEmiter(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Emiter.");
  string l_szCore        = _TreeComponent.GetPszISOProperty("core",   "",            true);
  Vect3f l_vVolume       = _TreeComponent.GetVect3fProperty("volume", Vect3f(1,1,1), true);
  int    l_iMaxParticles = _TreeComponent.GetIntProperty   ("max_particles", DEFAULT_MAX_PARTICLES_PER_EMITER, false);

  CComponentEmiter *l_pCEmiter;

  if(!(l_pCEmiter = CComponentEmiter::AddToEntity(_pEntity, l_szCore, l_vVolume, l_iMaxParticles)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component Emiter de l'objecte \"%s\"",_pEntity->GetName());
  }
}

void LoadComponentBillboard(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Billboard.");
  string l_szCore = _TreeComponent.GetPszISOProperty("core",   "",            true);

  CComponentBillboard *l_pCBillboard;

  if(!(l_pCBillboard = CComponentBillboard::AddToEntity(_pEntity, l_szCore)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component Billboard de l'objecte \"%s\"",_pEntity->GetName());
  }
}

void LoadComponentSpawner(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Spawner.");
  bool l_bActive = _TreeComponent.GetBoolProperty("active", true, false);
  float l_fSpawnTime = _TreeComponent.GetFloatProperty("spawn_time", -1, false);
  int l_iMaxEnemy = _TreeComponent.GetIntProperty("total_enemy", -1, false);
  string l_szEnemyType = _TreeComponent.GetPszISOProperty("enemy_type", "militar", false);
  CComponentSpawner::EEnemyType l_EnemyType = CComponentSpawner::EE_MILITAR;

  if(l_szEnemyType == "miner")
    l_EnemyType = CComponentSpawner::EE_MINER;

  CComponentSpawner *l_pcSpawner;

  if(!(l_pcSpawner = CComponentSpawner::AddToEntity(_pEntity, l_bActive, l_fSpawnTime, l_iMaxEnemy, l_EnemyType)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component Spawner de l'objecte \"%s\"",_pEntity->GetName());
  }
}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void LoadPlayer(CEntityManager* _pEM, CXMLTreeNode& _TreePlayer)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Player");

  string l_szName    = _TreePlayer.GetPszISOProperty("name", "Player", false);
  Vect3f l_vPosition = _TreePlayer.GetVect3fProperty("position", Vect3f(0,0,0),true);
  float  l_fYaw      = _TreePlayer.GetFloatProperty("yaw",0,true) * FLOAT_PI_VALUE / 180.0f;

  
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\t\tPlayer name \"%s\", pos %f,%f,%f, yaw %f", l_szName.c_str(),
                                     l_vPosition.x, l_vPosition.y, l_vPosition.z, l_fYaw);

  _pEM->InitPlayer(l_szName, l_vPosition, l_fYaw);
}

void LoadMiner(CEntityManager* _pEM, CXMLTreeNode& _TreeMiner)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Miner");
  
  string l_szName          = _TreeMiner.GetPszISOProperty("name", "", false);
  string l_szPlayerName    = _TreeMiner.GetPszISOProperty("player", "Player", false);
  Vect3f l_vPosition       = _TreeMiner.GetVect3fProperty("position", Vect3f(0,0,0),true);
  bool l_bActive           = _TreeMiner.GetBoolProperty("active", true, false);
  string l_szOnDeathScript = _TreeMiner.GetPszISOProperty("on_death", "", false);
  
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\t\tMiner name \"%s\", pos %f,%f,%f, Player %s", l_szName.c_str(),
                                      l_vPosition.x, l_vPosition.y, l_vPosition.z, l_szPlayerName.c_str());

  CGameEntity* l_pEntity = _pEM->InitMiner(l_szPlayerName, l_vPosition, l_szName,l_bActive, l_szOnDeathScript);

}

void LoadMilitar(CEntityManager* _pEM, CXMLTreeNode& _TreeMiner)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Militar");
  
  string l_szName          = _TreeMiner.GetPszISOProperty("name", "", false);
  string l_szPlayerName    = _TreeMiner.GetPszISOProperty("player", "Player", false);
  Vect3f l_vPosition       = _TreeMiner.GetVect3fProperty("position", Vect3f(0,0,0),true);
  bool l_bActive           = _TreeMiner.GetBoolProperty("active", true, false);
  string l_szOnDeathScript = _TreeMiner.GetPszISOProperty("on_death", "", false);
  
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\t\tMilitar name \"%s\", pos %f,%f,%f, Player %s", l_szName.c_str(),
                                      l_vPosition.x, l_vPosition.y, l_vPosition.z, l_szPlayerName.c_str());

  CGameEntity* l_pEntity = _pEM->InitMilitar(l_szPlayerName, l_vPosition, l_szName, l_bActive, l_szOnDeathScript);

}

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------


void CEntityManager::LoadEntitiesFromXML(const string& _szFile)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CEntityManager::LoadEntitiesFromXML Carregant el fitxer de Entitats \"%s\" [Utilities/EntityLoader.cpp]", _szFile.c_str());
  
  CXMLTreeNode l_TreeEntities;
  if(!l_TreeEntities.LoadFile(_szFile.c_str()))
  {
    LOGGER->AddNewLog(ELL_ERROR,"\tNo s'ha trobat el Fitxer");
  } else 
  {
    //llegir XML
    LOGGER->AddNewLog(ELL_INFORMATION,"\tLlegint XML");

    if(strcmp(l_TreeEntities.GetName(),"Entities") != 0)
    {
      LOGGER->AddNewLog(ELL_WARNING,"\tEl node arrel no és \"Entities\"!");
    } else
    {
      int l_iNumEntities = l_TreeEntities.GetNumChildren();
      for(int i = 0; i < l_iNumEntities; ++i)
      {
        CXMLTreeNode l_TreeEntity = l_TreeEntities(i);
        if(strcmp(l_TreeEntity.GetName(),"Entity") == 0)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"\tCreant nova entitat");
          CGameEntity *l_pEntity = CreateEntity();

          if(l_TreeEntity.ExistsProperty("name"))
          {
            LOGGER->AddNewLog(ELL_INFORMATION,"\t\tDefinint nom \"%s\"", l_TreeEntity.GetPszISOProperty("name").c_str());
            if(!SetName(l_TreeEntity.GetPszISOProperty("name"), l_pEntity))
            {
              LOGGER->AddNewLog(ELL_WARNING,"\t\tNo s'ha pogut definir nom \"%s\", probablement repetit", l_TreeEntity.GetPszISOProperty("name").c_str());
            }
          }

          int l_iNumComponents = l_TreeEntity.GetNumChildren();
          for(int j = 0; j < l_iNumComponents; ++j)
          {
            CXMLTreeNode l_TreeComponent = l_TreeEntity(j);
            
            // ----------------- Object 3D ------------------------------------------------------------------------------
            if(strcmp(l_TreeComponent.GetName(),"Object3D") == 0)
            {
              LoadComponentObject3D(l_TreeComponent, l_pEntity);
              
            // ----------------- Renderable Object ----------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"RenderableObject") == 0)
            {
              LoadComponentRenderableObject(l_TreeComponent, l_pEntity);
              
            // ----------------- PhysX Box ------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"PhysXBox") == 0)
            {
              LoadComponentPhysXBox(l_TreeComponent, l_pEntity);

            // ----------------- PhysX Mesh -----------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"PhysXMesh") == 0)
            {
              LoadComponentPhysXMesh(l_TreeComponent, l_pEntity);

            // ----------------- Trigger --------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Trigger") == 0)
            {
              LoadComponentTrigger(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"HighCover") == 0)
            {
              LoadComponentHighCover(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"LowCover") == 0)
            {
              LoadComponentLowCover(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Door") == 0)
            {
              LoadComponentDoor(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Interactive") == 0)
            {
              LoadComponentInteractive(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Destroyable") == 0)
            {
              LoadComponentDestroyable(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Explosive") == 0)
            {
              LoadComponentExplosive(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Rotative") == 0)
            {
              LoadComponentRotative(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"NavNode") == 0)
            {
              LoadComponentNavNode(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Emiter") == 0)
            {
              LoadComponentEmiter(l_TreeComponent, l_pEntity);
              
            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Billboard") == 0)
            {
              LoadComponentBillboard(l_TreeComponent, l_pEntity);
              
            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"EnemySpawner") == 0)
            {
              LoadComponentSpawner(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(!l_TreeComponent.IsComment())
            {
              LOGGER->AddNewLog(ELL_WARNING,"\tComponent \"%s\" no reconegut!", l_TreeComponent.GetName());
            }
          }
        } else if(strcmp(l_TreeEntity.GetName(),"Player") == 0)
        {
          LoadPlayer(this, l_TreeEntity);
        } else if(strcmp(l_TreeEntity.GetName(),"Miner") == 0)
        {
          LoadMiner(this, l_TreeEntity);
        } else if(strcmp(l_TreeEntity.GetName(),"Militar") == 0)
        {
          LoadMilitar(this, l_TreeEntity);
        } else if(!l_TreeEntity.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING,"\tNode \"%s\" no reconegut!", l_TreeEntities.GetName());
        }
      }
    }
  }
  SetOk(true);
}



CGameEntity* CEntityManager::InitPlayer(const string& _szEntityName, const Vect3f& _vPosition, float _fYaw)
{
  float l_fCapsuleHeigh = 0.5f;
  float l_fCapsuleRadius = 0.7f;
  float l_fCapsuleSkin   = 0.01f;

  float l_fTotalHeight = ((l_fCapsuleHeigh+2*l_fCapsuleRadius)*0.5f + l_fCapsuleSkin);

  CGameEntity* l_pPlayer = CreateEntity();
  if(!SetName(_szEntityName, l_pPlayer))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\tNo s'ha pogut definir nom \"%s\", probablement repetit", _szEntityName.c_str());
  }

  CComponentObject3D::AddToEntity(l_pPlayer);
  CComponentObject3D *l_pComponentObject3D = l_pPlayer->GetComponent<CComponentObject3D>();
  l_pComponentObject3D->SetPosition(_vPosition);
  l_pComponentObject3D->SetYaw(_fYaw);

  CComponentMovement::AddToEntity(l_pPlayer);

  CComponentRenderableObject * l_pComponentRenderableObject = 
                    CComponentRenderableObject::AddToEntityWithAnimatedModel(l_pPlayer, "Player Character", "riggle");
  l_pComponentRenderableObject->m_bBlockPitchRoll = true;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;
  l_pComponentRenderableObject->m_fHeightAdjustment = -l_fTotalHeight;


  CComponentPlayerController *l_pComponentPlayerController = CComponentPlayerController::AddToEntity(l_pPlayer);
  l_pComponentPlayerController->m_vPosInicial = _vPosition;

  CComponent3rdPSCamera::AddToEntity(l_pPlayer, 0.55f, 0.85f, 1.4f);

  CComponentPhysXController::AddToEntity(l_pPlayer, l_fCapsuleRadius, l_fCapsuleHeigh, 45.0f, l_fCapsuleSkin, 0.5f, ECG_PERSONATGE );

  CComponentShield::AddToEntity(l_pPlayer, 50.f, 50.f, 1.0f, 5.0f);

  CComponentVida::AddToEntity(l_pPlayer, 100.f, 100.f, true, 30.0f, 7.0f);

  CComponentStateMachine::AddToEntity(l_pPlayer, "State_Player_Neutre");

  CComponentMirilla::AddToEntity(l_pPlayer, "laser_pilota");

  CComponentArma::AddToEntity(l_pPlayer, "ARMA");

  CComponentRagdoll::AddToEntity(l_pPlayer, "Data/Animated Models/Riggle/Skeleton.xml", ECG_RAGDOLL_PLAYER);


  return l_pPlayer;
}

CGameEntity* CEntityManager::InitMiner(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName, const bool _bActive, const string& _szOnDeathScript)
{
  CGameEntity* l_pMiner = InitEnemy(_szPlayerName, _vPosition, 0.8f, 
                    "State_Enemy_Idle", "miner", "Data/Animated Models/Miner/Skeleton.xml",
                    _szEntityName, _szOnDeathScript);

  l_pMiner->GetComponent<CComponentRenderableObject>()->m_fHeightAdjustment = -1.1f;

  CComponentCollisionReport::AddToEntity(l_pMiner,"","","","enemy_on_start_colision","","",0.1f);

  l_pMiner->SetActive(_bActive);

  return l_pMiner;
}

CGameEntity* CEntityManager::InitMilitar(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName, const bool _bActive, const string& _szOnDeathScript)
{
  CGameEntity* l_pMilitar = InitEnemy(_szPlayerName, _vPosition, 0.8f, 
                    "State_Soldier_Idle", "Militar", "Data/Animated Models/Militar/Skeleton.xml",
                    _szEntityName, _szOnDeathScript);

  l_pMilitar->GetComponent<CComponentRenderableObject>()->m_fHeightAdjustment = -1.0f;

  CComponentCollisionReport::AddToEntity(l_pMilitar,"","","","enemy_on_start_colision","","",0.1f);

  l_pMilitar->SetActive(_bActive);

  return l_pMilitar;
}

CGameEntity* CEntityManager::InitEnemy(const string& _szPlayerName, const Vect3f& _vPosition, float _fRadius,
                         const string& _szInitialState, const string& _szRenderableModel, const string& _szRagdollModell,
                         const string& _szEntityName, const string& _szOnDeathScript)
{
  CGameEntity* l_peEnemy = CreateEntity();
  if(_szEntityName != "")
  {
    SetName(_szEntityName, l_peEnemy);
  }

  CComponentObject3D::AddToEntity(l_peEnemy);
  CComponentObject3D *l_pComponentObject3D = l_peEnemy->GetComponent<CComponentObject3D>();
  l_pComponentObject3D->SetPosition(_vPosition);
  CComponentMovement::AddToEntity(l_peEnemy);

  CComponentPhysXController::AddToEntity(l_peEnemy, _fRadius, 0.5f, 45.0f, 0.01f, 0.5f,  ECG_ENEMICS );


  stringstream l_szInstanceModelName(_szRenderableModel);

  l_szInstanceModelName << " " << l_peEnemy->GetGUID();
  
  CComponentRenderableObject *l_pComponentRenderableObject = CComponentRenderableObject::AddToEntityWithAnimatedModel(l_peEnemy, l_szInstanceModelName.str(), _szRenderableModel);
  l_pComponentRenderableObject->m_bBlockPitchRoll = true;
  l_pComponentRenderableObject->m_fHeightAdjustment = -1.5f;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;

  //(new CComponentIAWalkToPlayer())->Init(l_peEnemy,"Player",2,"walk","impact");

  //CComponentAnimation::AddToEntity(l_peEnemy);
  CComponentVida::AddToEntity(l_peEnemy, 100.f, 100.f);
  //Important IABrain despres de ComponentVida, sinó IABrain no te la informacio actualitzada de la vida
  CComponentIABrain::AddToEntity(l_peEnemy,_szPlayerName,_szRagdollModell, _szOnDeathScript);
  

  CComponentStateMachine::AddToEntity(l_peEnemy, _szInitialState);

  CComponentRagdoll::AddToEntity(l_peEnemy, _szRagdollModell, ECG_RAGDOLL);

  return l_peEnemy;
}

CGameEntity* CEntityManager::InitLaser(const Vect3f& _vPosInit, const Vect3f& _vDir, float _fDany, uint32 _uiCollisionMask)
{
  Mat33f l_mRot = GetFastestRotationFromDirToDir(Vect3f(0.0f,0.0f,-1.0f), _vDir);
  Mat44f l_mO3D(l_mRot);
  l_mO3D.SetPos(_vPosInit);

  CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();

  CComponentObject3D *l_pComponentObject3D = CComponentObject3D::AddToEntity(l_pLaser);
  l_pComponentObject3D->SetMat44(l_mO3D);

  CComponentLaser::AddToEntity( l_pLaser, _vDir, _fDany, _uiCollisionMask );

  CComponentRenderableObject *l_pCRO= CComponentRenderableObject::AddToEntity(l_pLaser, "laser " + l_pLaser->GetName(), "laser");
  l_pCRO->m_bRemoveRenderableObject = true;
  l_pComponentObject3D->SetMat44(l_mO3D);

  CComponentLifetime::AddToEntity(l_pLaser, 60);

  return l_pLaser;
}


CGameEntity* CEntityManager::InitParticles(const string& _szCore, const Vect3f& _vPos, const Vect3f& _vSize, float _fTime, const Vect3f& _vYdir)
{
  assert(abs(_vYdir.SquaredLength() - 1.f) < 0.01f && "CEntityManager::InitParticles _vYdir cal que sigui unitari");

  Mat33f l_mRot = GetFastestRotationFromDirToDir(Vect3f(0,1,0), _vYdir);
  Mat44f l_mO3D(l_mRot);
  l_mO3D.SetPos(_vPos);

  CGameEntity* l_pEmiterEntity = ENTITY_MANAGER->CreateEntity();
  CComponentObject3D* l_pCO3D = CComponentObject3D::AddToEntity(l_pEmiterEntity);
  l_pCO3D->SetMat44(l_mO3D);

  CComponentEmiter  ::AddToEntity(l_pEmiterEntity, _szCore, _vSize);
  if(_fTime > 0)
    CComponentLifetime::AddToEntity(l_pEmiterEntity, _fTime);

  return l_pEmiterEntity;
}

CGameEntity* CEntityManager::InitTriggerWithParticles(const string& _szCore, const Vect3f& _vPos, const Vect3f& _vSize, uint32 _uiCollisionMask, const string& _szOnEnter, const string& _szOnExit, float _fTime, const Vect3f& _vYdir)
{
  assert(abs(_vYdir.SquaredLength() - 1.f) < 0.01f && "CEntityManager::InitTriggerWithParticles _vYdir cal que sigui unitari");

  Mat33f l_mRot = GetFastestRotationFromDirToDir(Vect3f(0,1,0), _vYdir);
  Mat44f l_mO3D(l_mRot);
  l_mO3D.SetPos(_vPos);

  CGameEntity* l_pTriggerEmiterEntity = ENTITY_MANAGER->CreateEntity();
  CComponentObject3D* l_pCO3D = CComponentObject3D::AddToEntity(l_pTriggerEmiterEntity);
  l_pCO3D->SetMat44(l_mO3D);

  CComponentTrigger ::AddToEntity(l_pTriggerEmiterEntity, _vSize, _szOnEnter, _szOnExit, _uiCollisionMask);
  CComponentEmiter  ::AddToEntity(l_pTriggerEmiterEntity, _szCore, _vSize);
  if(_fTime > 0)
    CComponentLifetime::AddToEntity(l_pTriggerEmiterEntity, _fTime);

  return l_pTriggerEmiterEntity;
}

CGameEntity* CEntityManager::InitPickUp(const string& _szName, const string& _szCore, const Vect3f& _vPos, const string& _szOnPickUp)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CEntityManager::InitPickUp Creant Pick Up amb nom \"%s\" i core \"%s\".", _szName.c_str(), _szCore.c_str());
  CGameEntity* l_pPickUpEntity = ENTITY_MANAGER->CreateEntity();
  CComponentObject3D* l_pO3D = CComponentObject3D::AddToEntity(l_pPickUpEntity);

  CComponentRenderableObject* l_pRO = CComponentRenderableObject::AddToEntity(l_pPickUpEntity, _szName, _szCore);

  if(l_pRO)
  {
    l_pO3D->SetPosition(_vPos);
    CComponentTrigger::AddToEntity(l_pPickUpEntity, l_pRO->GetRenderableObject()->GetBoundingBox()->GetDimension(), _szOnPickUp, "", GetCollisionGroup("pickup"));
    CComponentRotative::AddToEntity(l_pPickUpEntity, Vect3f(0.5f,.0f,.0f));
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "CEntityManager::InitPickUp Error al crear el component Renderable Object.");
    l_pPickUpEntity->SetActive(false);
    ENTITY_MANAGER->RemoveEntity(l_pPickUpEntity);
    return 0;
  }

  return l_pPickUpEntity;
}
