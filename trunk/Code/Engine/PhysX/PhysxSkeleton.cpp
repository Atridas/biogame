#include "PhysicsManager.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "PhysicFixedJoint.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysxBone.h"
#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>
#include "RenderManager.h"
#include "Core.h"
#include "base.h"



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
  InitPhysXJoints(_szFileName);

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

//void CPhysxSkeleton::InitPhysXJoints()
//{
//  for (size_t i=0;i<m_vBones.size();++i)
//  {
//
//    int l_iIdParent = m_vBones[i]->GetCalBone()->getCoreBone()->getParentId();
//
//    if (!m_vBones[i]->IsBoneRoot())
//    {
//     
//      CalBone* l_pParent = m_pCalSkeleton->getBone(l_iIdParent);
//      CPhysxBone* l_pPhysxBone = GetPhysxBoneByName(l_pParent->getCoreBone()->getName());
//      m_vBones[i]->InitPhysXJoint(l_pPhysxBone);
//    }
//  }
//}


bool CPhysxSkeleton::InitPhysXJoints(string _szFileName)
{

  CXMLTreeNode l_XML;
  CXMLTreeNode l_XMLObjects;
	if(!l_XML.LoadFile(_szFileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_WARNING,"CPhysxRagdoll:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
		return false;
	}


  l_XMLObjects = l_XML(1);

  int l_iNumObjects = l_XMLObjects.GetNumChildren();

  
	for(int i = 0; i < l_iNumObjects; i++)
	{
    string l_szType,l_szActor1,l_szActor2;
    CXMLTreeNode l_XMLObject = l_XMLObjects(i);
    if(l_XMLObject.IsComment())
		{
			continue;
		}

    l_szType			= l_XMLObject.GetPszISOProperty("type" ,"");
    l_szActor1		= l_XMLObject.GetPszISOProperty("Actor1" ,"");
    l_szActor2		= l_XMLObject.GetPszISOProperty("Actor2" ,"");

    CPhysxBone* l_pBone1 = GetPhysxBoneByName(l_szActor1);
    CPhysxBone* l_pBone2 = GetPhysxBoneByName(l_szActor2);

    /*CPhysicActor* l_pActor1 = l_pBone1->GetPhysxActor();
    CPhysicActor* l_pActor2 = l_pBone2->GetPhysxActor();*/
    CPhysicActor* l_pActor1 = 0;
    CPhysicActor* l_pActor2 = 0;


    if (l_szType=="spherical")
    {
      CPhysicSphericalJoint* l_pSphericalJoint = 0;
      l_pSphericalJoint = new CPhysicSphericalJoint();
      CalVector l_vCalVect = l_pBone1->GetCalBone()->getTranslationAbsolute();
      Vect3f l_vJointPointMiddle(-l_vCalVect.x,l_vCalVect.y,l_vCalVect.z);
      l_pActor1 = l_pBone1->GetPhysxActor();
      Mat44f l_vMatActor;
      l_pActor1->GetMat44(l_vMatActor);
      Vect3f l_vTrans = l_vMatActor.GetTranslationVector();
      Vect3f l_vRot = l_vMatActor.GetPitchRollYaw();

      Mat44f l_vRotMat = l_pBone1->GetBoneLeftHandedAbsoluteTransformation(l_pBone1->GetCalBone());
      l_vRotMat.Translate(Vect3f(1.0f,0.0f,0.0f));
      Vect3f l_vPosCenter = l_vRotMat.GetTranslationVector();

      Vect3f l_vMiddle = l_pBone1->GetMiddlePoint();
      l_vMiddle.x = -l_vMiddle.x;
      Vect3f l_vAxis(l_vMiddle.x-l_vJointPointMiddle.x,l_vMiddle.y-l_vJointPointMiddle.y,l_vMiddle.z-l_vJointPointMiddle.z);
      //Vect3f l_vAxis(l_vTrans.x-l_vJointPointMiddle.x,l_vTrans.y-l_vJointPointMiddle.y,l_vTrans.z-l_vJointPointMiddle.z);
      l_vAxis.Normalize();



      if (l_szActor2=="NULL")
      {
        //l_pSphericalJoint->SetInfo(l_vJointPointMiddle,l_pActor1);
        //l_pSphericalJoint->SetInfoComplete(l_vJointPointMiddle,/*Vect3f(1.0f,0.0f,0.0f)*/l_vRot,l_pActor1);
        l_pSphericalJoint->SetInfoComplete(l_vJointPointMiddle,Vect3f(1.0f,0.0f,0.0f),l_pActor1);
      }
      else
      {
        l_pActor2 = l_pBone2->GetPhysxActor();
        //l_pSphericalJoint->SetInfo(l_vJointPointMiddle,l_pActor1,l_pActor2);
        //l_pSphericalJoint->SetInfoComplete(l_vJointPointMiddle,/*Vect3f(1.0f,0.0f,0.0f)*/l_vRot,l_pActor1,l_pActor2);
        l_pSphericalJoint->SetInfoComplete(l_vJointPointMiddle,Vect3f(1.0f,0.0f,0.0f),l_pActor1,l_pActor2);
      }
      CORE->GetPhysicsManager()->AddPhysicSphericalJoint(l_pSphericalJoint);
      m_vSphericalJoints.push_back(l_pSphericalJoint);
    
    }
    
    if (l_szType=="fixed")
    {
      CPhysicFixedJoint* l_pFixedJoint = 0;
      l_pFixedJoint = new CPhysicFixedJoint();

      if (l_szActor2=="NULL")
      {
        l_pActor1 = l_pBone1->GetPhysxActor();
        l_pFixedJoint->SetInfo(l_pActor1);
      }
      else
      {
        l_pActor1 = l_pBone1->GetPhysxActor();
        l_pActor2 = l_pBone2->GetPhysxActor();
        l_pFixedJoint->SetInfo(l_pActor1,l_pActor2);
      }

      CORE->GetPhysicsManager()->AddPhysicFixedJoint(l_pFixedJoint);
      m_vFixedJoints.push_back(l_pFixedJoint);
    
    }

    if (l_szType=="revolute")
    {
    
    
    }


  }

  return true;
  
}



void CPhysxSkeleton::Release()
{
  for (size_t i=0;i<m_vBones.size();++i)
  {
    CHECKED_DELETE(m_vBones[i])
  }

  for (size_t i=0;i<m_vFixedJoints.size();++i)
  {
    CHECKED_DELETE(m_vFixedJoints[i])
  }

  for (size_t i=0;i<m_vRevoluteJoints.size();++i)
  {
    CHECKED_DELETE(m_vRevoluteJoints[i])
  }

  for (size_t i=0;i<m_vSphericalJoints.size();++i)
  {
    CHECKED_DELETE(m_vSphericalJoints[i])
  }
  
  m_vFixedJoints.clear();
  m_vRevoluteJoints.clear();
  m_vSphericalJoints.clear();
  m_vBones.clear();
}

void CPhysxSkeleton::UpdateCal3dFromPhysx()
{


}

bool CPhysxSkeleton::Load(string _szFileName)
{

  CXMLTreeNode l_XML;
  CXMLTreeNode l_XMLObjects;
	if(!l_XML.LoadFile(_szFileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_WARNING,"CPhysxRagdoll:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
		return false;
	}


  l_XMLObjects = l_XML(0);

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