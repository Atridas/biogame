#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysicSphericalJoint.h"
#include "NxSphericalJointDesc.h"
#include "PhysicActor.h"

#include "base.h"

#include "Utils/MemLeaks.h"

CPhysicSphericalJoint::CPhysicSphericalJoint()
: m_pJoint(NULL)
, m_pSphericalDesc(NULL)
{
	m_pSphericalDesc = new NxSphericalJointDesc;
}

CPhysicSphericalJoint::~CPhysicSphericalJoint()
{
	CHECKED_DELETE(m_pSphericalDesc);
}

void CPhysicSphericalJoint::CreateJoint(NxJoint* joint)
{
	assert(joint);
	m_pJoint = joint;
	CHECKED_DELETE(m_pSphericalDesc);
}

void CPhysicSphericalJoint::SetInfo	(const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicSphericalJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

	NxVec3 pos(anchor.x, anchor.y, anchor.z );

	m_pSphericalDesc->setToDefault();
	m_pSphericalDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pSphericalDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pSphericalDesc->actor[1] = NULL;	
	}


	m_pSphericalDesc->setGlobalAnchor(pos);

   
 
}

void CPhysicSphericalJoint::SetInfoComplete	(const Vect3f& anchor, const Vect3f& _axis, CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicSphericalJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

	NxVec3 pos(anchor.x, anchor.y, anchor.z );
  NxVec3 axis(_axis.x, _axis.y, _axis.z );

	m_pSphericalDesc->setToDefault();
	m_pSphericalDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pSphericalDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pSphericalDesc->actor[1] = NULL;	
	}

  m_pSphericalDesc->flags |= NX_SJF_SWING_LIMIT_ENABLED;
  m_pSphericalDesc->swingLimit.value = 0.3f*NxPi;

  m_pSphericalDesc->flags |= NX_SJF_TWIST_LIMIT_ENABLED;
  m_pSphericalDesc->twistLimit.low.value = -0.05f*NxPi;
  m_pSphericalDesc->twistLimit.high.value = 0.05f*NxPi;

	m_pSphericalDesc->setGlobalAnchor(pos);
  m_pSphericalDesc->setGlobalAxis(axis);


  m_pSphericalDesc->projectionDistance = NX_JPM_LINEAR_MINDIST;
  m_pSphericalDesc->projectionDistance = 0.01f;
  /*m_pSphericalDesc->projectionMode |= NX_JPM_LINEAR_MINDIST;
  NxJointProjection*/


}