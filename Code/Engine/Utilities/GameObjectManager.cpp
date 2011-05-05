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
#include "RenderableObjectsManager.h"
#include "PhysicCookingMesh.h"
#include "StaticMeshManager.h"
#include "InstanceMesh.h"
#include "PhysicController.h"


void CGameObjectManager::Release()
{
  CMapManager<CGameObject>::Release();
  for(size_t i=0; i < m_vUserData.size() ; i++)
  {
    CHECKED_DELETE(m_vUserData[i]);
  }
  for(size_t i=0; i < m_vActors.size() ; i++)
  {
    CHECKED_DELETE(m_vActors[i]);
  }
}


//Afegir un ACTOR AMB LA BOUNDING BOX
CPhysicActor* CGameObjectManager::AddPhysicActor(CRenderableObject* _pRenderObject, string& _szName, float _fBody, bool _bPushable)
{
  CPhysicActor* l_pPhysicActor = 0;
  
  Vect3f l_vBoxDim = _pRenderObject->GetBoundingBox()->GetDimension();
  Vect3f l_vMiddlePos = _pRenderObject->GetBoundingBox()->GetMiddlePoint();
  Vect3f l_vPos = _pRenderObject->GetPosition();

  CPhysicUserData* l_pPhysicsUserData = new CPhysicUserData(_szName);
  l_pPhysicsUserData->SetColor(colWHITE);
  l_pPhysicsUserData->SetPaint(true);
  m_vUserData.push_back(l_pPhysicsUserData);
  l_pPhysicActor = new CPhysicActor(l_pPhysicsUserData);


  if (_bPushable)
  {
   l_pPhysicActor->AddBoxSphape(l_vBoxDim/2,l_vMiddlePos,NULL,GROUP_COLLIDABLE_PUSHABLE);
  }
  else
  {
    l_pPhysicActor->AddBoxSphape(l_vBoxDim/2,l_vMiddlePos,NULL,GROUP_COLLIDABLE_NON_PUSHABLE);
  } 

 
  if (_fBody != 0)
  {
    l_pPhysicActor->CreateBody(_fBody);
  }

  CORE->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  Mat44f l_vMat = _pRenderObject->GetMat44();
  l_pPhysicActor->SetMat44(l_vMat);

  m_vActors.push_back(l_pPhysicActor);
 
  
  return l_pPhysicActor;
}


//Afegir un ACTOR AMB LA MESH
CPhysicActor* CGameObjectManager::AddPhysicActorMesh(CRenderableObject* _pRenderObject, string& _szName, float _fBody, bool _bPushable)
{

  CPhysicActor* l_pPhysicActor = 0;
  Vect3f l_vPos = _pRenderObject->GetPosition();
  CPhysicUserData* l_pPhysicsUserData = new CPhysicUserData(_szName);
  l_pPhysicsUserData->SetColor(colWHITE);
  l_pPhysicsUserData->SetPaint(true);
  m_vUserData.push_back(l_pPhysicsUserData);
  l_pPhysicActor = new CPhysicActor(l_pPhysicsUserData);

  CInstanceMesh* l_pInstanceMesh = (CInstanceMesh*)_pRenderObject;
  CStaticMesh* l_pStaticMesh = l_pInstanceMesh->GetStaticMesh();
        
  const vector<uint32> l_vIndexBuff = l_pStaticMesh->GetIndexBuffer();
  const vector<Vect3f> l_vVertexBuff = l_pStaticMesh->GetVertexBuffer();
  CPhysicCookingMesh* l_pCookingMesh = CORE->GetPhysicsManager()->GetCookingMesh();
  l_pCookingMesh->CreatePhysicMesh(l_vVertexBuff,l_vIndexBuff,_szName);
  NxTriangleMesh* l_pMesh = l_pCookingMesh->GetPhysicMesh(_szName);

  if (_bPushable)
  {
    l_pPhysicActor->AddMeshShape(l_pMesh,v3fZERO,GROUP_COLLIDABLE_PUSHABLE);
  }
  else
  {
    l_pPhysicActor->AddMeshShape(l_pMesh,v3fZERO,GROUP_COLLIDABLE_NON_PUSHABLE);
  }

  
  
  if (_fBody != 0)
  {
    l_pPhysicActor->CreateBody(_fBody);
  }
  CORE->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  Mat44f l_vMat = l_pInstanceMesh->GetMat44();
  l_pPhysicActor->SetMat44(l_vMat);
  m_vActors.push_back(l_pPhysicActor);

  return l_pPhysicActor;
}

bool CGameObjectManager::Load(const string& _szFileName, bool _bReload)
{
	
  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();
  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();
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
    float l_fBody;
    bool l_bPushable;

		CGameObject* l_pGameObject = 0;
    CRenderableObject* l_pRenderObject = 0;
    CPhysicActor* l_pPhysicsActor = 0;
		CPhysicsManager* l_pPhysManager = CORE->GetPhysicsManager();

		CXMLTreeNode l_XMLObject = l_XMLObjects(i);

		if(l_XMLObject.IsComment())
		{
			continue;
		}

		l_szName			    = l_XMLObject.GetPszISOProperty("name" ,"");
		l_bPhysx			    = l_XMLObject.GetBoolProperty("Physx");
		l_szRenderObject	= l_XMLObject.GetPszISOProperty("RenderableObject" ,"");
		l_szPhysxActor		= l_XMLObject.GetPszISOProperty("PhysxActor" ,"");
		l_szPhysxType		  = l_XMLObject.GetPszISOProperty("PhysxType" ,"");
    l_fBody           = l_XMLObject.GetFloatProperty("Body");
    l_bPushable       = l_XMLObject.GetBoolProperty("Pushable");

		l_pGameObject = GetResource(l_szName);
    l_pRenderObject = l_pROM->GetResource(l_szRenderObject);
		if((!l_pGameObject) && (l_pRenderObject))
		{
			if(l_szPhysxType == "BoundingBox") 
			{
			  l_pGameObject = new CGameObject(l_szName);
        l_pPhysicsActor = AddPhysicActor(l_pRenderObject,l_szPhysxActor,l_fBody,l_bPushable);
        l_pGameObject->Init(l_pRenderObject,l_pPhysicsActor,l_szPhysxType);
        m_vResources.push_back(l_pGameObject);
        AddResource(l_szName,l_pGameObject);
        l_pRenderObject->SetVisible(true);
			

			} else if(l_szPhysxType == "BoundingSphere") 
			{
				


			} else if(l_szPhysxType == "mesh") 
			{

        l_pGameObject = new CGameObject(l_szName);
        l_pPhysicsActor = AddPhysicActorMesh(l_pRenderObject,l_szPhysxActor,l_fBody,l_bPushable);
        l_pGameObject->Init(l_pRenderObject,l_pPhysicsActor,l_szPhysxType);
        m_vResources.push_back(l_pGameObject);
        AddResource(l_szName,l_pGameObject);
        l_pRenderObject->SetVisible(true);

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

void CGameObjectManager::Update(float _fElapsedTime)
{
  for(size_t i=0; i < m_vResources.size() ; i++)
    m_vResources[i]->Update(_fElapsedTime);
}
