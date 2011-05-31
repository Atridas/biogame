#include "PhysicsManager.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>

bool CPhysxSkeleton::Init(const string& _szFileName, CalModel* _pCalModel)
{

  SetSkeleton(_pCalModel->getSkeleton());
  vector<CalBone*> l_vLlistaBones = m_pCalSkeleton->getVectorBone();

  //Inicialitzem l'estructura del PhysXSkeleton i creem les instancies dels PhysXBones.
  //Tindrem ja tota la info relativa de bones, pares i fills.
  for (size_t i=0;i<l_vLlistaBones.size();++i)
  {
    CalBone* l_pBone = l_vLlistaBones[i];
    CPhysxBone* l_pPhysXBone = new CPhysxBone(l_pBone->getCoreBone()->getName());
    l_pPhysXBone->Init(l_pBone);
    m_vBones.push_back(l_pPhysXBone);
    
  }


  //Load la info del XML I POSA ELS ACTORS!!!!
  Load(_szFileName);



  //Funcions de configuracio!!
  InitBoneMatrices();
  InitPhysXActors();
  InitPhysXJoints();

  SetOk(true);
  return IsOk();
}


void CPhysxSkeleton::InitBoneMatrices()
{

  for (size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->InitBoneMatrix();
  }


}


void CPhysxSkeleton::InitPhysXActors()
{

  for (size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->InitPhysXActor();
  }


}

void CPhysxSkeleton::InitPhysXJoints()
{
  for (size_t i=0;i<m_vBones.size();++i)
  {

    int l_iIdParent = m_vBones[i]->GetCalBone()->getCoreBone()->getParentId();

    if (!m_vBones[i]->IsBoneRoot())
    {
     
      CalBone* l_pParent = m_pCalSkeleton->getBone(l_iIdParent);
      CPhysxBone* l_pPhysxBone = GetPhysxBoneByName(l_pParent->getCoreBone()->getName());
      m_vBones[i]->InitPhysXJoint(l_pPhysxBone);
    }
  }
}

void CPhysxSkeleton::Release()
{
  for (size_t i=0;i<m_vBones.size();++i)
  {
    CHECKED_DELETE(m_vBones[i])
  }
  m_vBones.clear();
}

void CPhysxSkeleton::UpdateCal3dFromPhysx()
{


}

bool CPhysxSkeleton::Load(string _szFileName)
{

  CXMLTreeNode l_XMLObjects;
	if(!l_XMLObjects.LoadFile(_szFileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_WARNING,"CPhysxRagdoll:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
		return false;
	}


  int l_iNumObjects = l_XMLObjects.GetNumChildren();

	for(int i = 0; i < l_iNumObjects; i++)
	{
		CXMLTreeNode l_XMLObject = l_XMLObjects(i);
    string l_szType,l_szName;
    Vect3f l_vSize,l_fMiddlePoint; 
    float l_fDensity;

		if(l_XMLObject.IsComment())
		{
			continue;
		}


    l_szName			        = l_XMLObject.GetPszISOProperty("name" ,"");
    l_fDensity            = l_XMLObject.GetFloatProperty("density");
    l_szType	            = l_XMLObject.GetPszISOProperty("type" ,"");
    l_fMiddlePoint        = l_XMLObject.GetVect3fProperty("bounding_box_middle_point",Vect3f(0.0f), false);
    l_vSize               = l_XMLObject.GetVect3fProperty("bounding_box_size",Vect3f(0.0f), false);

    CPhysxBone* l_pBone = GetPhysxBoneByName(l_szName);

    if (l_pBone!=0)
    {
      l_pBone->Load(l_fDensity,l_szType,l_fMiddlePoint,l_vSize,l_szName);
    }
	  
	
  }


  return true;

}


CPhysxBone* CPhysxSkeleton::GetPhysxBoneByName(string _szName)
{

  CPhysxBone* l_pBone = 0;

  for(size_t i=0;i<m_vBones.size();++i)
  {
    string l_szName;
    l_szName = m_vBones[i]->GetName();
    if (l_szName == _szName)
    {
      l_pBone = m_vBones[i];
      return l_pBone;
    }

  }

  return 0;
}