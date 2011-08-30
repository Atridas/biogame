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
      LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component de Renderable Object Est�tic amb nom \"%s\".", l_szResource.c_str());
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
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box f�sica des del Renderable Object.");
    if(!CComponentPhysXBox::AddToEntity(_pEntity, l_fDensity, l_iCollisionMask))
    {
      LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
    }
  } else if(strcmp(_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"false") == 0)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box f�sica des de XML.");
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

  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Mesh f�sica des del Renderable Object.");
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
  
  string l_szName = _pEntity->GetName();
  bool l_bOpen    = _TreeComponent.GetBoolProperty("open", false, false);

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Porta amb nom \"%s\" i estat \"%d\"",l_szName.c_str(), l_bOpen);

  //component porta
  if(!CComponentDoor::AddToEntity(_pEntity, l_bOpen))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  }

  //m�quina d'estats de la porta:
  string l_szInitialState = "";
  if(l_bOpen)
    l_szInitialState = "State_Porta_Open";
  else
    l_szInitialState = "State_Porta_Closed";

  if(!CComponentStateMachine::AddToEntity(_pEntity, l_szInitialState))
  {
    LOGGER->AddNewLog(ELL_WARNING, "\tError al carregar la m�quina d'estats del component Door.");
    assert(false);
  }

}

void LoadComponentInteractive(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  
  string l_szName   = _pEntity->GetName();
  string l_szAction = _TreeComponent.GetPszISOProperty("action", "", false);

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Interactiu amb nom \"%s\" i acci� \"%s\"",l_szName.c_str(), l_szAction.c_str());


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

  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Destroyable amb nom \"%s\", acci� \"%s\" i model destruit \"%s\".",l_szName.c_str(), l_szAction.c_str(), l_szResource.c_str());

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
  string l_szCore  = _TreeComponent.GetPszISOProperty("core",   "",            true);
  Vect3f l_vVolume = _TreeComponent.GetVect3fProperty("volume", Vect3f(1,1,1), true);

  CComponentEmiter *l_pCEmiter;

  if(!(l_pCEmiter = CComponentEmiter::AddToEntity(_pEntity, l_szCore, l_vVolume)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tError al crear el component Emiter de l'objecte \"%s\"",_pEntity->GetName());
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
  
  string l_szName       = _TreeMiner.GetPszISOProperty("name", "", false);
  string l_szPlayerName = _TreeMiner.GetPszISOProperty("player", "Player", false);
  Vect3f l_vPosition    = _TreeMiner.GetVect3fProperty("position", Vect3f(0,0,0),true);
  bool l_bActive        = _TreeMiner.GetBoolProperty("active", true, false);
  
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\t\tMiner name \"%s\", pos %f,%f,%f, Player %s", l_szName.c_str(),
                                      l_vPosition.x, l_vPosition.y, l_vPosition.z, l_szPlayerName.c_str());

  CGameEntity* l_pEntity = _pEM->InitMiner(l_szPlayerName, l_vPosition, l_szName);

  CComponentStateMachine* l_pSM = l_pEntity->GetComponent<CComponentStateMachine>();

  if(l_pSM)
    l_pSM->SetActive(l_bActive);
}

void LoadMilitar(CEntityManager* _pEM, CXMLTreeNode& _TreeMiner)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\tCarregant Militar");
  
  string l_szName       = _TreeMiner.GetPszISOProperty("name", "", false);
  string l_szPlayerName = _TreeMiner.GetPszISOProperty("player", "Player", false);
  Vect3f l_vPosition    = _TreeMiner.GetVect3fProperty("position", Vect3f(0,0,0),true);
  bool l_bActive        = _TreeMiner.GetBoolProperty("active", true, false);
  
  LOGGER->AddNewLog(ELL_INFORMATION, "\t\t\tMilitar name \"%s\", pos %f,%f,%f, Player %s", l_szName.c_str(),
                                      l_vPosition.x, l_vPosition.y, l_vPosition.z, l_szPlayerName.c_str());

  CGameEntity* l_pEntity = _pEM->InitMilitar(l_szPlayerName, l_vPosition, l_szName, l_bActive);

  CComponentStateMachine* l_pSM = l_pEntity->GetComponent<CComponentStateMachine>();

  if(l_pSM)
    l_pSM->SetActive(l_bActive);
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
      LOGGER->AddNewLog(ELL_WARNING,"\tEl node arrel no �s \"Entities\"!");
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
            } else if(strcmp(l_TreeComponent.GetName(),"NavNode") == 0)
            {
              LoadComponentNavNode(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"Emiter") == 0)
            {
              LoadComponentEmiter(l_TreeComponent, l_pEntity);
              
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


  //CComponentAnimation::AddToEntity(l_pPlayer);


  CComponentPlayerController *l_pComponentPlayerController = CComponentPlayerController::AddToEntity(l_pPlayer);
                                  /*,
                                      //Actions
                                     "MoveFwd",
                                     "MoveBack",
                                     "MoveLeft",
                                     "MoveRight",
                                     "Walk",
                                     "Run",
                                     "Aim",
                                     "Shoot",
                                      //Animations
                                     "idle",
                                     "walk",
                                     "walk",
                                     "walk",
                                     "walk",
                                     "point",
                                     "shoot",
                                      //Speed
                                     4, 10, 1, 1,
                                      FLOAT_PI_VALUE/3,
                                     -FLOAT_PI_VALUE/3);*/

  l_pComponentPlayerController->m_vPosInicial = _vPosition;

  //CComponent3rdPSCamera *l_pComponent3rdPSCamera = new CComponent3rdPSCamera();
  //l_pComponent3rdPSCamera->Init(m_pPlayerEntity, 0, 0);
  //((CThPSCamera*)l_pComponent3rdPSCamera->GetCamera())->SetZoom(0);
  //l_pComponent3rdPSCamera->Init(l_pPlayer, 0.55f, 0.85f, 1.4f);

  CComponent3rdPSCamera::AddToEntity(l_pPlayer, 0.55f, 0.85f, 1.4f);

  //m_pCamera = l_pComponent3rdPSCamera->GetCamera();

  CComponentPhysXController::AddToEntity(l_pPlayer, l_fCapsuleRadius, l_fCapsuleHeigh, 45.0f, l_fCapsuleSkin, 0.5f, ECG_PERSONATGE );

  l_pComponentRenderableObject->m_fHeightAdjustment = -l_fTotalHeight;

  CComponentShield::AddToEntity(l_pPlayer, 50.f, 50.f, 1.0f, 5.0f);
  CComponentVida::AddToEntity(l_pPlayer, 100.f, 100.f, true, 30.0f, 7.0f);

  CComponentStateMachine::AddToEntity(l_pPlayer, "State_Player_Neutre");


  //(new CComponentRagdoll())->Init(m_pPlayerEntity, "Data/Animated Models/Riggle/Skeleton.xml");
  CComponentMirilla::AddToEntity(l_pPlayer, "laser_pilota");
  CComponentArma::AddToEntity(l_pPlayer, "ARMA");

  CComponentRagdoll::AddToEntity(l_pPlayer, "Data/Animated Models/Riggle/Skeleton.xml", ECG_RAGDOLL_PLAYER);


  return l_pPlayer;
}

CGameEntity* CEntityManager::InitMiner(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName, const bool _bActive)
{
  CGameEntity* l_pMiner = InitEnemy(_szPlayerName, _vPosition, 0.8f, 
                    "State_Enemy_Idle", "miner", "Data/Animated Models/Miner/Skeleton.xml",
                    _szEntityName, _bActive);

  l_pMiner->GetComponent<CComponentRenderableObject>()->m_fHeightAdjustment = -1.1f;

  CComponentCollisionReport::AddToEntity(l_pMiner,"","","","enemy_on_start_colision","","",0.1f);

  return l_pMiner;
}

CGameEntity* CEntityManager::InitMilitar(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName, const bool _bActive)
{
  CGameEntity* l_pMilitar = InitEnemy(_szPlayerName, _vPosition, 0.8f, 
                    "State_Soldier_Idle", "Militar", "Data/Animated Models/Militar/Skeleton.xml",
                    _szEntityName, _bActive);

  l_pMilitar->GetComponent<CComponentRenderableObject>()->m_fHeightAdjustment = -1.0f;

  CComponentCollisionReport::AddToEntity(l_pMilitar,"","","","enemy_on_start_colision","","",0.1f);

  return l_pMilitar;
}

CGameEntity* CEntityManager::InitEnemy(const string& _szPlayerName, const Vect3f& _vPosition, float _fRadius,
                         const string& _szInitialState, const string& _szRenderableModel, const string& _szRagdollModell,
                         const string& _szEntityName, const bool _bActive)
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
  //Important IABrain despres de ComponentVida, sin� IABrain no te la informacio actualitzada de la vida
  CComponentIABrain::AddToEntity(l_peEnemy,_szPlayerName,_szRagdollModell);
  

  CComponentStateMachine::AddToEntity(l_peEnemy, _szInitialState);

  CComponentRagdoll::AddToEntity(l_peEnemy, _szRagdollModell, ECG_RAGDOLL);

  return l_peEnemy;
}