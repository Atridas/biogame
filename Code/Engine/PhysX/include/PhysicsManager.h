//----------------------------------------------------------------------------------
// CPhysicsManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary physic functions.
// It internally uses the PhysX library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSICS_MANAGER_H_
#define INC_PHYSICS_MANAGER_H_

#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
#include "Named.h"
#include <vector>
//#include "Script/ScriptRegister.h"

//---Forward Declarations---
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

//--------------------------



enum ECollisionGroup {
  ECG_ESCENARI,
  ECG_PERSONATGE,
  ECG_ENEMICS,
  ECG_TRIGGERS,
  ECG_COBERTURES,
  ECG_OBJECTES_DINAMICS,
  ECG_EXPLOSIONS
};

int GetCollisionGroup(const string& _szGroup);

//---- Declaracion de nuevos tipos------------
struct SPhysicsInitParams
{
	SPhysicsInitParams::SPhysicsInitParams():	m_fSkinWidth(0.001f), m_fGravity(-9.81f), 
																						m_Restitution_DefMat(0.5f), m_StaticFriction_DefMat(0.5f),
																						m_DynamicFriction_DefMat(0.5f) {}
	float		m_fSkinWidth;
	float		m_fGravity;
	float		m_Restitution_DefMat;				//Restitution of the default material
	float		m_StaticFriction_DefMat;		//Static Friction of the default material
	float		m_DynamicFriction_DefMat;		//Dynamic Friction of the default material
};

struct SCollisionInfo
{
	float		m_fDistance;
	Vect3f	m_Normal;
	Vect3f	m_CollisionPoint;
};

class CGameEntity;

class CPhysicUserData : public CNamed
{
public:
	CPhysicUserData(const string& _szName) : CNamed(_szName), m_bPaintPhysicObject(false), m_ColorPhysicObject(colWHITE), m_pEntity(0) {}
	~CPhysicUserData() {/*Nothing*/}

	void					SetPaint	(bool flag)						  {m_bPaintPhysicObject = flag;}
	void					SetColor	(const CColor& color)   {m_ColorPhysicObject = color;}
  void          SetEntity (CGameEntity* _pEntity) {m_pEntity = _pEntity;}

	bool					GetPaint	() const							{return m_bPaintPhysicObject;}
	const CColor&	GetColor	() const							{return m_ColorPhysicObject;}
	CGameEntity* 	GetEntity	() const							{return m_pEntity;}


private:
	bool		m_bPaintPhysicObject;
	CColor	m_ColorPhysicObject;

  CGameEntity* m_pEntity;
};
//--------------------------------------------

class CPhysicsManager
{

public:
	//--- Init and End protocols	
	CPhysicsManager()	:	m_bIsOk(false), m_bDebugRenderMode(true),
											m_pPhysicsSDK(0), m_pScene(0),
											m_pControllerManager(0), m_pMyAllocator(0),
											m_pCookingMesh(0), m_InitParams() {}

	~CPhysicsManager() {Done();}

	bool								Init												(const string& _physXConfig);
	void								Done												();
	bool								IsOk												() const { return m_bIsOk; }
	void								SetInitParams								(const SPhysicsInitParams& initParams) {m_InitParams = initParams;}

	//----CScriptRegister interface-------------------
	virtual void				RegisterFunctions					  (CScriptManager* scriptManager);
  bool						    Load	                      (const string& _szFileName, bool _bReload);

	//--- Intersection Functions:
	CPhysicUserData*		RaycastClosestActor					(const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, CPhysicUserData* impactObject, SCollisionInfo& info);
  CPhysicUserData*		RaycastClosestActorShoot		(const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, CPhysicUserData* impactObject, SCollisionInfo& info, float _fPower);
	void								OverlapSphereActor					(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects);
  void								OverlapSphereActorGrenade		(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects);
  void                ApplyExplosion              ();


	//--- Get CookingMesh
	CPhysicCookingMesh*	GetCookingMesh							() {return m_pCookingMesh;}


	//--- Rendering Stuff:
	void								DebugRender									(CRenderManager* render);
	void								SetDebugRenderMode					(bool flag) {m_bDebugRenderMode = flag;}
	bool								GetDebugRenderMode					() const		{return m_bDebugRenderMode ;}

	//----Update
	void								Update											(float elapsedTime);

	//--- Add/Release Actors
	bool								AddPhysicActor							(CPhysicActor* actor);
	bool								ReleasePhysicActor					(CPhysicActor* actor);

	//--- Add/Release CharacterControllers
	bool								AddPhysicController					(CPhysicController* controller);
	bool								ReleasePhysicController			(CPhysicController* controller);

	//--- Add/Release Joints
	bool								AddPhysicSphericalJoint			(CPhysicSphericalJoint* joint);
	bool								RelasePhysicSphericalJoint	(CPhysicSphericalJoint* joint);
	bool								AddPhysicRevoluteJoint			(CPhysicRevoluteJoint* joint);
	bool								RelasePhysicRevoluteJoint		(CPhysicRevoluteJoint* joint);
	bool								AddPhysicFixedJoint					(CPhysicFixedJoint* joint);
	bool								RelasePhysicFixedJoint			(CPhysicFixedJoint* joint);

	//--- Create CCDSkeleton
	NxCCDSkeleton*			CreateCCDSkeleton						(float size);

	void								SetTriggerReport						(CPhysicTriggerReport* report);
  
  int GetCollisionMask (ECollisionGroup _szGroup) {return m_CollisionMasks[_szGroup];};

private:
	void								Release											();
	void								DrawActor										(NxActor* actor, CRenderManager* render);


private:
	bool									m_bIsOk;
	bool									m_bDebugRenderMode;

  map<int, int> m_CollisionMasks;

	//---PhysX------------------------------
	NxPhysicsSDK*					m_pPhysicsSDK;
	NxScene*							m_pScene;
	NxControllerManager*	m_pControllerManager;
	CPhysicUserAllocator*	m_pMyAllocator;
	CPhysicCookingMesh*		m_pCookingMesh;
	SPhysicsInitParams		m_InitParams;
	//-------------------------------------------
};

#endif // INC_PHYSICS_MANAGER_H_