#include "GameObjectManager.h"
#include "RenderManager.h"
#include "InstanceMesh.h"
#include "StaticMesh.h"
#include <XML/XMLTreeNode.h>
#include "RenderableAnimatedInstanceModel.h"
#include "InstancedData.h"
#include "Core.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"

void CGameObjectManager::Release()
{
  CMapManager<CGameObject>::Release();
}

bool CGameObjectManager::Load(const string& _szFileName, bool _bReload)
{
	
	
	LOGGER->AddNewLog(ELL_INFORMATION, "CGameObjectManager::Load");

	CXMLTreeNode l_XMLObjects;
	if(!l_XMLObjects.LoadFile(_szFileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_WARNING,"CGameObjectManager:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
		return false;
	}

	int l_iNumObjects = l_XMLObjects.GetNumChildren();

	for(int i = 0; i < l_iNumObjects; i++)
	{
		string l_szName, l_bPhysx, l_szRenderObject, l_szPhysxType, l_szPhysxActor;

		CGameObject* l_pGameObject = 0;
		CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();

		CXMLTreeNode l_XMLObject = l_XMLObjects(i);

		if(l_XMLObject.IsComment())
		{
			continue;
		}

		l_szName			= l_XMLObject.GetPszISOProperty("name" ,"");
		l_bPhysx			= l_XMLObject.GetBoolProperty("Physx");
		l_szRenderObject	= l_XMLObject.GetPszISOProperty("RenderableObject" ,"");
		l_szPhysxActor		= l_XMLObject.GetPszISOProperty("PhysxActor" ,"");
		l_szPhysxType		= l_XMLObject.GetPszISOProperty("PhysxType" ,"");

		l_pGameObject = GetResource(l_szName);
		if(!l_pGameObject)
		{
			if(l_szPhysxType == "BoundingBox") 
			{
				
			

			} else if(l_szPhysxType == "BoundingSphere") 
			{
				


			} else if(l_szPhysxType == "Mesh") 
			{
				


			}
			else
			{
				LOGGER->AddNewLog(ELL_WARNING,"CGameObjectManager:: No suportem aquest sistema de Fisica");
				continue;
			}

			


		}else{
			if(!_bReload)
			{
				LOGGER->AddNewLog(ELL_WARNING,"CRenderableObjectsManager:: Repeated object \"%s\"", l_szName.c_str());
			}
			
		}
	}


	return true;


}
