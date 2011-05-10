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

#include "base.h"
#include "Core.h"

#include <XML/XMLTreeNode.h>

void LoadEntitiesFromXML(const string& _szFile)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "Carregant el fitxer de Entitats \"%s\" [Utilities/EntityLoader.cpp]", _szFile.c_str());

  CEntityManager *l_pEntityManager = CORE->GetEntityManager();

  CXMLTreeNode l_TreeEntities;
  if(!l_TreeEntities.LoadFile(_szFile.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"\tNo s'ha trobat el Fitxer");
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
          CGameEntity *l_pEntity = l_pEntityManager->CreateEntity();

          if(l_TreeEntity.ExistsProperty("name"))
          {
            LOGGER->AddNewLog(ELL_INFORMATION,"\t\tDefinint nom \"%s\"", l_TreeEntity.GetPszISOProperty("name").c_str());
            l_pEntityManager->SetName(l_TreeEntity.GetPszISOProperty("name"), l_pEntity);
          }

          int l_iNumComponents = l_TreeEntity.GetNumChildren();
          for(int j = 0; j < l_iNumComponents; ++j)
          {
            CXMLTreeNode l_TreeComponent = l_TreeEntity(j);
            
            // ----------------- Object 3D ------------------------------------------------------------------------------
            if(strcmp(l_TreeComponent.GetName(),"Object3D") == 0)
            {
              LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component d'Object3D.");
              CComponentObject3D *l_pComponentObject3D = new CComponentObject3D();
              if(l_pComponentObject3D->Init(l_pEntity))
              {
                Vect3f l_vPos   = l_TreeComponent.GetVect3fProperty("position", Vect3f(0,0,0), false);
                float  l_fYaw   = l_TreeComponent.GetFloatProperty("yaw"  , 0.0f, false);
                float  l_fPitch = l_TreeComponent.GetFloatProperty("pitch", 0.0f, false);
                float  l_fRoll  = l_TreeComponent.GetFloatProperty("roll" , 0.0f, false);
                
                l_pComponentObject3D->SetPosition(l_vPos);
                l_pComponentObject3D->SetYaw(l_fYaw);
                l_pComponentObject3D->SetPitch(l_fPitch);
                l_pComponentObject3D->SetRoll(l_fRoll);
              } else
              {
                LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
                delete l_pComponentObject3D;
              }
              
            // ----------------- Renderable Object ----------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"RenderableObject") == 0)
            {
              if(l_TreeComponent.ExistsProperty("name"))
              {
                LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCreant component de Renderable Object amb nom \"%s\".", l_TreeComponent.GetPszISOProperty("name").c_str());
                CComponentRenderableObject *l_pComponentRenderableObject = new CComponentRenderableObject();
                if(!l_pComponentRenderableObject->Init(l_pEntity, l_TreeComponent.GetPszISOProperty("name")))
                {
                  LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
                  delete l_pComponentRenderableObject;
                }
              } else
              {
                LOGGER->AddNewLog(ELL_WARNING,"\t\tFalta parametre \"name\".");
              }
              
            // ----------------- PhysX Box ------------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"PhysXBox") == 0)
            {
              float l_fDensity = l_TreeComponent.GetFloatProperty("density", 0.f, false);

              //TODO mascares
              int l_iCollisionMask = 0;

              if(strcmp(l_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"true") == 0)
              {
                LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box física des del Renderable Object.");
                CComponentPhysXBox* l_pComponentPhysXBox = new CComponentPhysXBox();
                if(!l_pComponentPhysXBox->Init(l_pEntity, l_fDensity, l_iCollisionMask))
                {
                  LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
                  delete l_pComponentPhysXBox;
                }
              } else if(strcmp(l_TreeComponent.GetPszISOProperty("fromRenderableObject").c_str(),"false") == 0)
              {
                LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Box física des de XML.");
                //TODO
                LOGGER->AddNewLog(ELL_WARNING,"\t\t\tCarregar box no implementat!");
              } else
              {
                LOGGER->AddNewLog(ELL_WARNING,"\t\tParametre \"fromRenderableObject\" incorrecte.");
              }

            // ----------------- PhysX Mesh -----------------------------------------------------------------------------
            } else if(strcmp(l_TreeComponent.GetName(),"PhysXMesh") == 0)
            {
              float l_fDensity = l_TreeComponent.GetFloatProperty("density", 0.f, false);

              //TODO mascares
              int l_iCollisionMask = 0;

              LOGGER->AddNewLog(ELL_INFORMATION,"\t\tCarregant Mesh física des del Renderable Object.");
              CComponentPhysXMesh* l_pComponentPhysXMesh = new CComponentPhysXMesh();
              if(!l_pComponentPhysXMesh->Init(l_pEntity, l_fDensity, l_iCollisionMask))
              {
                LOGGER->AddNewLog(ELL_WARNING,"\t\t\tError al crear el component.");
                delete l_pComponentPhysXMesh;
              }
              

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
