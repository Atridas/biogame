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
#include "ComponentRenderableObject.h"
#include "ComponentTrigger.h"

#include "base.h"
#include "Core.h"

#include <XML/XMLTreeNode.h>

/*
enum ECollisionGroup {
  ECG_ESCENARI = 1,
  ECG_PERSONATGE = 2,
  ECG_ENEMICS = 4,
  ECG_TRIGGERS = 8,
  ECG_COBERTURES = 16,
  ECG_OBJECTES_DINAMICS = 32,
  ECG_EXPLOSIONS = 64
}
*/

int GetCollisionGroups(const string& _szGroup)
{
  if(_szGroup == "escenari")
  {
    return ECG_ESCENARI | ECG_PERSONATGE | ECG_ENEMICS | ECG_OBJECTES_DINAMICS;
  }
  else if(_szGroup == "personatge")
  {
    return ECG_ESCENARI | ECG_PERSONATGE | ECG_ENEMICS | ECG_OBJECTES_DINAMICS | ECG_TRIGGERS | ECG_EXPLOSIONS;
  }
  else if(_szGroup == "enemic")
  {
    return ECG_ESCENARI | ECG_PERSONATGE | ECG_ENEMICS | ECG_OBJECTES_DINAMICS | ECG_EXPLOSIONS;
  }
  else if(_szGroup == "triggers")
  {
    return ECG_PERSONATGE | ECG_TRIGGERS;
  }
  else if(_szGroup == "cobertura")
  {
    return ECG_COBERTURES;
  }
  else if(_szGroup == "objecte dinamic")
  {
    return ECG_ESCENARI | ECG_PERSONATGE | ECG_ENEMICS | ECG_OBJECTES_DINAMICS | ECG_EXPLOSIONS;
  }
  else if(_szGroup == "explosio")
  {
    return ECG_PERSONATGE | ECG_ENEMICS | ECG_OBJECTES_DINAMICS | ECG_EXPLOSIONS;
  }
  else
  {
    return 0;
  }
}

void LoadComponentObject3D(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component d'Object3D.");
  CComponentObject3D *l_pComponentObject3D = new CComponentObject3D();
  if(l_pComponentObject3D->Init(_pEntity))
  {
    Vect3f l_vPos   = _TreeComponent.GetVect3fProperty("position", Vect3f(0,0,0), false);
    float  l_fYaw   = _TreeComponent.GetFloatProperty("yaw"  , 0.0f, false);
    float  l_fPitch = _TreeComponent.GetFloatProperty("pitch", 0.0f, false);
    float  l_fRoll  = _TreeComponent.GetFloatProperty("roll" , 0.0f, false);
                
    l_pComponentObject3D->SetPosition(l_vPos);
    l_pComponentObject3D->SetYaw(l_fYaw);
    l_pComponentObject3D->SetPitch(l_fPitch);
    l_pComponentObject3D->SetRoll(l_fRoll);
  } else
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
    delete l_pComponentObject3D;
  }
}

void LoadComponentRenderableObject(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  if(_TreeComponent.ExistsProperty("name"))
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component de Renderable Object amb nom \"%s\".", _TreeComponent.GetPszISOProperty("name").c_str());
    CComponentRenderableObject *l_pComponentRenderableObject = new CComponentRenderableObject();
    if(!l_pComponentRenderableObject->Init(_pEntity, _TreeComponent.GetPszISOProperty("name")))
    {
      LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
      delete l_pComponentRenderableObject;
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
  int l_iCollisionMask = GetCollisionGroups(_TreeComponent.GetPszISOProperty("collision_group", "", true));

  if(strcmp(_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"true") == 0)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box f�sica des del Renderable Object.");
    CComponentPhysXBox* l_pComponentPhysXBox = new CComponentPhysXBox();
    if(!l_pComponentPhysXBox->Init(_pEntity, l_fDensity, l_iCollisionMask))
    {
      LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
      delete l_pComponentPhysXBox;
    }
  } else if(strcmp(_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"false") == 0)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box f�sica des de XML.");
    Vect3f l_vMidpoint = _TreeComponent.GetVect3fProperty("midpoint", Vect3f(), true);
    Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);

    CComponentPhysXBox* l_pComponentPhysXBox = new CComponentPhysXBox();
    if(!l_pComponentPhysXBox->Init( _pEntity, 
                                    l_vSize.x, l_vSize.y, l_vSize.z,
                                    l_vMidpoint.x, l_vMidpoint.y, l_vMidpoint.z,
                                    l_fDensity, l_iCollisionMask)
      )
    {
      LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
      delete l_pComponentPhysXBox;
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
  int l_iCollisionMask = GetCollisionGroups(_TreeComponent.GetPszISOProperty("collision_group", "", true));

  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Mesh f�sica des del Renderable Object.");
  CComponentPhysXMesh* l_pComponentPhysXMesh = new CComponentPhysXMesh();
  if(!l_pComponentPhysXMesh->Init(_pEntity, l_fDensity, l_iCollisionMask))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
    delete l_pComponentPhysXMesh;
  }
}


void LoadComponentTrigger(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Trigger.");

  //TODO mascares
  int l_iCollisionMask = GetCollisionGroups(_TreeComponent.GetPszISOProperty("collision_group", "", true));
  Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);
  string l_szOnEnter = _TreeComponent.GetPszISOProperty("onEnter", "", false);
  string l_szOnExit = _TreeComponent.GetPszISOProperty("onExit", "", false);
  
  LOGGER->AddNewLog(ELL_INFORMATION,"\t\t\t Enter = \"%s\" Exit = \"%s\".",l_szOnEnter.c_str(), l_szOnExit.c_str());


  if(l_szOnEnter == "" && l_szOnExit == "")
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tTrigger sense cap script!.");
  }

  CComponentTrigger* l_pComponentTrigger = new CComponentTrigger();

  if(!l_pComponentTrigger->Init(_pEntity, l_vSize, l_szOnEnter, l_szOnExit, l_iCollisionMask))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
    delete l_pComponentTrigger;
  }
}

void LoadComponentHighCover(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  //LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Cobertura.");
  //
  ////TODO mascares
  //int l_iCollisionMask = 1;
  //Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);
  //int l_iSpots = _TreeComponent.GetPszISOProperty("cover_size", "", false);
  //
  //LOGGER->AddNewLog(ELL_INFORMATION,"\t\t\t HighCover: \"%d\" spots.", l_iSpots);

  //CComponentHighCover* l_pComponentHighCover = new CComponentHighCover();

  //if(!l_pComponentHighCover->Init(_pEntity, l_vSize, l_iSpots, l_iCollisionMask))
  //{
  //  LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  //  delete l_pComponentHighCover;
  //}
}

void LoadComponentLowCover(CXMLTreeNode& _TreeComponent, CGameEntity* _pEntity)
{
  //LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Cobertura.");
  //
  ////TODO mascares
  //int l_iCollisionMask = 1;
  //Vect3f l_vSize = _TreeComponent.GetVect3fProperty("size", Vect3f(1), true);
  //int l_iSpots = _TreeComponent.GetPszISOProperty("cover_size", "", false);
  //
  //LOGGER->AddNewLog(ELL_INFORMATION,"\t\t\t HighCover: \"%d\" spots.", l_iSpots);

  //CComponentLowCover* l_pComponentLowCover = new CComponentLowCover();

  //if(!l_pComponentLowCover->Init(_pEntity, l_vSize, l_iSpots, l_iCollisionMask))
  //{
  //  LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
  //  delete l_pComponentLowCover;
  //}
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
            SetName(l_TreeEntity.GetPszISOProperty("name"), l_pEntity);
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
            } else if(strcmp(l_TreeComponent.GetName(),"HighCover") == 0)
            {
              LoadComponentHighCover(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"LowCover") == 0)
            {
              LoadComponentLowCover(l_TreeComponent, l_pEntity);

            // -----------------------------------------------------------------------------------------------------------
            } else if(!l_TreeComponent.IsComment())
            {
              LOGGER->AddNewLog(ELL_WARNING,"\tNode \"%s\" no reconegut!", l_TreeEntities.GetName());
            }
          }

        } else if(!l_TreeEntity.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING,"\tNode \"%s\" no reconegut!", l_TreeEntities.GetName());
        }
      }
    }
  }
}
