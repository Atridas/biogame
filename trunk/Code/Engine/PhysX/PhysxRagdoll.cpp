#include "PhysxRagdoll.h"
#include "Core.h"
#include "base.h"
#include <XML/XMLTreeNode.h>
#include <PhysicsManager.h>
#include "PhysicActor.h"
#include "PhysicController.h"
#include "PhysicFixedJoint.h"
#include "AnimatedModelManager.h"


void CPhysxRagdoll::Release() 
{ 
  //CHECKED_DELETE(m_pPhysXActor)
 for (size_t i=0; i<m_vBoneActors.size();++i)
 {
   CHECKED_DELETE(m_vBoneActors[i].m_pActor)
   CHECKED_DELETE(m_vBoneActors[i].m_pUserData)
 }
  m_vBoneActors.clear();
}; 

bool CPhysxRagdoll::Load(const string& _szFileName, bool _bReload)
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
    string l_szType,l_szBoneName,l_szName;
    Vect3f l_vSize; 
    float l_fDensity;

		if(l_XMLObject.IsComment())
		{
			continue;
		}

    l_fDensity            = l_XMLObject.GetFloatProperty("Density");
    l_szType	            = l_XMLObject.GetPszISOProperty("Type" ,"");
    l_szName			        = l_XMLObject.GetPszISOProperty("Name" ,"");
    l_szBoneName			    = l_XMLObject.GetPszISOProperty("BoneName" ,"");
    l_vSize               = l_XMLObject.GetVect3fProperty("Size",Vect3f(0.0f), false);
	  
    SBoneActor l_sBoneActor;
    l_sBoneActor.m_Density = l_fDensity;
    l_sBoneActor.m_pActorName  = l_szName;
    l_sBoneActor.m_szType = l_szType;
    l_sBoneActor.m_szBoneName = l_szBoneName;
    l_sBoneActor.m_vSize = l_vSize;
    m_vBoneActors.push_back(l_sBoneActor);
		
  }

  return true;
}


bool CPhysxRagdoll::Init(CalSkeleton* _pSkeleton)
{
   CPhysicsManager* l_pPM = CORE->GetPhysicsManager();



  for (size_t i=0; i<m_vBoneActors.size();++i)
  {
    int l_iBoneId = _pSkeleton->getCoreSkeleton()->getCoreBoneId(m_vBoneActors[i].m_szBoneName);

    CalBone* l_pBone = _pSkeleton->getBone(l_iBoneId);
    CalVector l_vVectorPos = l_pBone->getTranslationAbsolute(); 
    Vect3f l_vBonePos(l_vVectorPos.x,l_vVectorPos.y,l_vVectorPos.z);
    //l_pBone->calculateBoundingBox();
    
    CalVector l_vPoints[8];
    l_pBone->getBoundingBox().computePoints(l_vPoints);
    l_pBone->getBoundingBox().computePoints(l_vPoints);
    CalMatrix l_vMatrix = l_pBone->getTransformMatrix();

    CalQuaternion l_vQuaternion = l_pBone->getRotationAbsolute();
    
    Mat33f l_vMat33(l_vMatrix.dxdx,l_vMatrix.dxdy,l_vMatrix.dxdz,l_vMatrix.dydx,l_vMatrix.dydy,l_vMatrix.dydz,l_vMatrix.dzdx,l_vMatrix.dzdy,l_vMatrix.dzdz);
    float l_fYaw = l_vMat33.GetYaw();
    float l_fPitch = l_vMat33.GetPitch();
    float l_fRoll = l_vMat33.GetRoll();

    
    
    //l_pBone->getCoreBone()->getBoundingBox().

    
    CBoundingBox* l_pBox = new CBoundingBox();
    Vect3f l_vect[8];
        
    for (int t=0;t<8;++t)
    {
      l_vect[t] = Vect3f(l_vPoints[t].x,l_vPoints[t].y,l_vPoints[t].z);
    }

    l_pBox->Init(l_vect);
    Vect3f l_vMiddlePoint = l_pBox->GetMiddlePoint();
    Vect3f l_vPos(-l_vMiddlePoint.x,l_vMiddlePoint.y,l_vMiddlePoint.z);
    //l_pBox->CalcDimension();
    Mat44f l_vTrans;
    l_vTrans.SetIdentity();
    l_vTrans.Translate(l_vPos);


    Mat44f r, r2, r3, t, total;
    
    t.SetIdentity();
    r.SetIdentity();
    r2.SetIdentity();
    r3.SetIdentity();

    t.Translate(l_vPos);
    r.SetFromAngleY(l_fPitch);
    r2.SetFromAngleZ(l_fYaw);
    //r3.SetFromAngleX(l_fRoll);
    //s.Scale(1.5f,1.5f,1.5f);

    total = t*r*r2;

    //Mat44f l_vMat44(l_vMat33); 




    if (m_vBoneActors[i].m_szType == "Box")
    {
      CPhysicUserData* l_pUserData = new CPhysicUserData(m_vBoneActors[i].m_pActorName);
      l_pUserData->SetPaint(true);
      l_pUserData->SetColor(colGREEN);
      CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
      l_pActor->AddBoxSphape(l_pBox->GetDimension()*0.5f,v3fZERO,NULL,GROUP_COLLIDABLE_NON_PUSHABLE);
      l_pActor->CreateBody(m_vBoneActors[i].m_Density);

      
      //l_pActor->SetGlobalPosition(l_vPos);

      l_pPM->AddPhysicActor(l_pActor);
      l_pActor->SetMat44(total);

      m_vBoneActors[i].m_pActor = l_pActor;
      m_vBoneActors[i].m_pUserData = l_pUserData;
      
    }

    if (m_vBoneActors[i].m_szType == "Sphere")
    {
      CPhysicUserData* l_pUserData = new CPhysicUserData(m_vBoneActors[i].m_pActorName);
      l_pUserData->SetPaint(true);
      l_pUserData->SetColor(colYELLOW);
      CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
      l_pActor->AddSphereShape(m_vBoneActors[i].m_vSize.x,l_vPos,NULL,GROUP_COLLIDABLE_NON_PUSHABLE);
      l_pActor->CreateBody(m_vBoneActors[i].m_Density);
      l_pPM->AddPhysicActor(l_pActor);

      m_vBoneActors[i].m_pActor = l_pActor;
      m_vBoneActors[i].m_pUserData = l_pUserData;
      
    }

    CHECKED_DELETE(l_pBox)
  }

  return true;
}