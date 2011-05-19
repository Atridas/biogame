#pragma once
#ifndef INC_PHYSIC_RAGDOLL_H
#define INC_PHYSIC_RAGDOLL_H

#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
#include "Named.h"
#include <vector>
#include <cal3d/cal3d.h>

//FORWARD DECLARATIONS/////////
class NxPhysicsSDK;
class NxScene;
class NxControllerManager;
class CPhysicUserAllocator;
class CPhysicCookingMesh;
class CPhysicActor;
class NxActor;
class NxCCDSkeleton;
class CPhysicController;
class CRenderManager;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CPhysicFixedJoint;
class CPhysicTriggerReport;
class CScriptManager;
class CPhysicUserData;
///////////////////////////////
struct SBoneActor {
 string m_pActorName;
 string m_szBoneName;
 string m_szType;
 Vect3f m_vSize;
 float  m_Density;
 CPhysicActor* m_pActor;
 CPhysicUserData* m_pUserData;
};

class CPhysxRagdoll:
  public CNamed,
  public CBaseControl
{
public:
  //Constructors
  CPhysxRagdoll(string _szName) : CNamed(_szName),m_pSkeleton(0) {};
  ~CPhysxRagdoll(){Done();Release();};

  //Funcions
  bool						Load	        (const string& _szFileName, bool _bReload);
  bool						Init	        (CalSkeleton* _pSkeleton);
  bool						InitSkeleton	(CalSkeleton* _pSkeleton);

  //CGameObject(string _szName) : m_pRenderableObject(0), m_pPhysXActor(0),CNamed(_szName),m_pUserData(0),m_szType("") {};
protected:
  void                    Release();   
  

private:
  vector<SBoneActor> m_vBoneActors;
  CalSkeleton* m_pSkeleton;
};


#endif //INC_PHYSIC_CONTROLLER_H
