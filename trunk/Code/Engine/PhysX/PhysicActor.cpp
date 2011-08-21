#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysicActor.h"
//#include "Base/Math/Matrix44.h"

//----PhysX Includes-------------
#undef min
#undef max
#include "NxPhysics.h"
//-------------------------------

CPhysicActor::CPhysicActor(CPhysicUserData* userData)
: m_pUserData(userData)
, m_pPhXActor(NULL)
, m_pPhXActorDesc(NULL)
, m_pPhXBodyDesc(NULL)
{

	assert(m_pUserData);
	m_pPhXActorDesc = new NxActorDesc();
	m_pPhXBodyDesc = new NxBodyDesc();
}

CPhysicActor::~CPhysicActor()
{
	DeInit();
}

void CPhysicActor::DeInit()
{
	CHECKED_DELETE(m_pPhXActorDesc);
	CHECKED_DELETE(m_pPhXBodyDesc);

	if (m_vBoxDesc.size() > 0)
	{
		std::vector<NxBoxShapeDesc*>::iterator it(m_vBoxDesc.begin());
		std::vector<NxBoxShapeDesc*>::iterator itEnd(m_vBoxDesc.end());
		while (it != itEnd)
		{
			NxBoxShapeDesc* shapeDesc = *it;
			CHECKED_DELETE(shapeDesc);
			it++;
		}
		m_vBoxDesc.clear();
	}

	if (m_vMeshDesc.size() > 0)
	{
		std::vector<NxTriangleMeshShapeDesc*>::iterator it(m_vMeshDesc.begin());
		std::vector<NxTriangleMeshShapeDesc*>::iterator itEnd(m_vMeshDesc.end());
		while (it != itEnd)
		{
			NxTriangleMeshShapeDesc* shapeDesc = *it;
			CHECKED_DELETE(shapeDesc);
			it++;
		}
		m_vMeshDesc.clear();
	}

	if (m_vCapsuleDesc.size() > 0)
	{
		std::vector<NxCapsuleShapeDesc*>::iterator it(m_vCapsuleDesc.begin());
		std::vector<NxCapsuleShapeDesc*>::iterator itEnd(m_vCapsuleDesc.end());
		while (it != itEnd)
		{
			NxCapsuleShapeDesc* shapeDesc = *it;
			CHECKED_DELETE(shapeDesc);
			it++;
		}
		m_vCapsuleDesc.clear();
	}

	if (m_vSphereDesc.size() > 0)
	{
		std::vector<NxSphereShapeDesc*>::iterator it(m_vSphereDesc.begin());
		std::vector<NxSphereShapeDesc*>::iterator itEnd(m_vSphereDesc.end());
		while (it != itEnd)
		{
			NxSphereShapeDesc* shapeDesc = *it;
			CHECKED_DELETE(shapeDesc);
			it++;
		}
		m_vSphereDesc.clear();
	}

	if (m_vPlaneDesc.size() > 0)
	{
		std::vector<NxPlaneShapeDesc*>::iterator it(m_vPlaneDesc.begin());
		std::vector<NxPlaneShapeDesc*>::iterator itEnd(m_vPlaneDesc.end());
		while (it != itEnd)
		{
			NxPlaneShapeDesc* shapeDesc = *it;
			CHECKED_DELETE(shapeDesc);
			it++;
		}
		m_vPlaneDesc.clear();
	}
}
void CPhysicActor::CreateActor(NxActor* actor)
{
	assert(actor);
	m_pPhXActor = actor;
	DeInit();
}

void CPhysicActor::SetActorSolverIterationCount(int _iCount)
{
  if(m_pPhXActor)
  {
    m_pPhXActor->setSolverIterationCount(_iCount);
  }
}

void CPhysicActor::SetLinearVelocity (const Vect3f& velocity)
{
	if (m_pPhXActor)
	{
		if (velocity != v3fZERO)
		{
			m_pPhXActor->setLinearVelocity( NxVec3( velocity.x, velocity.y, velocity.z) );
		}
	}
	else
	{
		//TODO log de error...

	}
}


void CPhysicActor::AddSphereShape	(float radius, const Vect3f& localPos, NxCCDSkeleton* skeleton, uint32 group)
{
	assert(m_pPhXActorDesc);

	// Add a sphere shape to the actor descriptor
	NxSphereShapeDesc* sphereDesc = new NxSphereShapeDesc();
	assert(sphereDesc);
	sphereDesc->group = group;
	m_vSphereDesc.push_back(sphereDesc);
	sphereDesc->radius = radius;
	sphereDesc->localPose.t = NxVec3(localPos.x, localPos.y, localPos.z);
	if (skeleton != NULL)
	{
		sphereDesc->ccdSkeleton = skeleton;
		sphereDesc->shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD; //Activate dynamic-dynamic CCD for this body
	}
	m_pPhXActorDesc->shapes.pushBack( sphereDesc );
  
}

void CPhysicActor::AddBoxSphape (const Vect3f& size, const Vect3f& localPos, NxCCDSkeleton* skeleton, uint32 group)
{
	assert(m_pPhXActorDesc);

	NxBoxShapeDesc* boxDesc = new NxBoxShapeDesc();
	assert(boxDesc);
	boxDesc->group = group;
	m_vBoxDesc.push_back(boxDesc);
	// Add a box shape to the actor descriptor
	boxDesc->dimensions = NxVec3( size.x, size.y, size.z);
	boxDesc->localPose.t = NxVec3(localPos.x, localPos.y, localPos.z);
	if (skeleton != NULL)
	{
		boxDesc->ccdSkeleton = skeleton;
		boxDesc->shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD; //Activate dynamic-dynamic CCD for this body
	}
	m_pPhXActorDesc->shapes.pushBack( boxDesc );
  
}

void CPhysicActor::AddCapsuleShape (float radius, float height, const Vect3f& localPos, NxCCDSkeleton* skeleton, uint32 group)
{
	assert(m_pPhXActorDesc);

	// Add a capsule shape to the actor descriptor
	NxCapsuleShapeDesc* capsuleDesc = new NxCapsuleShapeDesc();
	assert(capsuleDesc);
	capsuleDesc->group = group;
	m_vCapsuleDesc.push_back(capsuleDesc);
	capsuleDesc->height = height;
	capsuleDesc->radius = radius;
	capsuleDesc->localPose.t = NxVec3(localPos.x, localPos.y, localPos.z);
	if (skeleton != NULL)
	{
		capsuleDesc->ccdSkeleton = skeleton;
		capsuleDesc->shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD; //Activate dynamic-dynamic CCD for this body
	}
	m_pPhXActorDesc->shapes.pushBack( capsuleDesc );
}

void CPhysicActor::AddMeshShape	(NxTriangleMesh* mesh, const Vect3f& localPos, uint32 group)
{
	assert(m_pPhXActorDesc);
	assert(mesh);

	// Add a mesh shape to the actor descriptor
	NxTriangleMeshShapeDesc* triangleMeshDesc = new NxTriangleMeshShapeDesc();
	assert(triangleMeshDesc);
	triangleMeshDesc->group = group;
	m_vMeshDesc.push_back(triangleMeshDesc);
	// The actor has mesh shape
	triangleMeshDesc->meshData = mesh;
	m_pPhXActorDesc->shapes.pushBack( triangleMeshDesc );
}

void CPhysicActor::AddPlaneShape (const Vect3f& normal, float distance, uint32 group)
{
	assert(m_pPhXActorDesc);

	// Add a plane shape to the actor descriptor
	NxPlaneShapeDesc *planeDesc = new NxPlaneShapeDesc();
	assert(planeDesc);
	planeDesc->group = group;
	m_vPlaneDesc.push_back(planeDesc);
	planeDesc->normal	= NxVec3( normal.x, normal.y, normal.z);
	planeDesc->d	 = distance;
	m_pPhXActorDesc->shapes.pushBack( planeDesc );

}

void CPhysicActor::CreateBody (float density, float angularDamping, float linearDamping)
{
	if( density != 0 )
	{
		assert(m_pPhXBodyDesc);

		// Create body
		m_pPhXBodyDesc->angularDamping	= angularDamping;
    m_pPhXBodyDesc->linearDamping = linearDamping;
		m_pPhXActorDesc->body			= m_pPhXBodyDesc;
		m_pPhXActorDesc->density = density;
	}	
}

void CPhysicActor::SetGlobalPosition	(const Vect3f& pos)
{
	assert(m_pPhXActorDesc);
	m_pPhXActorDesc->globalPose.t  = NxVec3(pos.x, pos.y, pos.z);;
}

Vect3f CPhysicActor::GetPosition ()
{
	assert(m_pPhXActor);
	NxVec3 pos = m_pPhXActor->getGlobalPosition();
	return Vect3f(pos.x, pos.y, pos.z);
}

void CPhysicActor::CreateBoxTrigger(const Vect3f& size, uint32 group)
{
	assert(m_pPhXActorDesc);
	NxBoxShapeDesc* boxDesc = new NxBoxShapeDesc();
	assert(boxDesc);
	boxDesc->group = group;
	m_vBoxDesc.push_back(boxDesc);
	boxDesc->dimensions = NxVec3(size.x, size.y, size.z);
	boxDesc->shapeFlags |= NX_TRIGGER_ENABLE;
	m_pPhXActorDesc->shapes.pushBack(boxDesc);
}

void CPhysicActor::ActivateAllTriggers()
{
  uint32 size = m_pPhXActorDesc->shapes.size();
  for(int i = 0; i < size; ++i)
  {
    m_pPhXActorDesc->shapes[i]->shapeFlags |= NX_TRIGGER_ENABLE;
  }
}


void CPhysicActor::SetMat44 (const Mat44f& matrix)
{
	assert(m_pPhXActor);
	NxF32 m_aux[16];
	m_aux[0]	= matrix.m00;
	m_aux[4]	= matrix.m01;
	m_aux[8]	= matrix.m02;
	m_aux[12]	= matrix.m03;
	m_aux[1]	= matrix.m10;
	m_aux[5]	= matrix.m11;
	m_aux[9]	= matrix.m12;
	m_aux[13]	= matrix.m13;
	m_aux[2]	= matrix.m20;
	m_aux[6]	= matrix.m21;
	m_aux[10]	= matrix.m22;
	m_aux[14]	= matrix.m23;
	m_aux[3]	= matrix.m30;
	m_aux[7]	= matrix.m31;
	m_aux[11]	= matrix.m32;
	m_aux[15]	= matrix.m33; 
	NxMat34 mat;
	mat.setColumnMajor44(m_aux);
	m_pPhXActor->setGlobalPose(mat);
}

void CPhysicActor::MoveGlobalPoseMat44 (const Mat44f& matrix)
{
	assert(m_pPhXActor);
	NxF32 m_aux[16];
	m_aux[0]	= matrix.m00;
	m_aux[4]	= matrix.m01;
	m_aux[8]	= matrix.m02;
	m_aux[12]	= matrix.m03;
	m_aux[1]	= matrix.m10;
	m_aux[5]	= matrix.m11;
	m_aux[9]	= matrix.m12;
	m_aux[13]	= matrix.m13;
	m_aux[2]	= matrix.m20;
	m_aux[6]	= matrix.m21;
	m_aux[10]	= matrix.m22;
	m_aux[14]	= matrix.m23;
	m_aux[3]	= matrix.m30;
	m_aux[7]	= matrix.m31;
	m_aux[11]	= matrix.m32;
	m_aux[15]	= matrix.m33; 
	NxMat34 mat;
	mat.setColumnMajor44(m_aux);
	m_pPhXActor->moveGlobalPose(mat);
}

void CPhysicActor::GetMat44 (Mat44f& matrix) const
{
	assert(m_pPhXActor);
	NxF32 m_aux[16];
	m_pPhXActor->getGlobalPose().getColumnMajor44(m_aux);
	matrix.m00 = m_aux[0];
	matrix.m01 = m_aux[4];
	matrix.m02 = m_aux[8];
	matrix.m03 = m_aux[12];
	matrix.m10 = m_aux[1];
	matrix.m11 = m_aux[5];
	matrix.m12 = m_aux[9];
	matrix.m13 = m_aux[13];
	matrix.m20 = m_aux[2];
	matrix.m21 = m_aux[6];
	matrix.m22 = m_aux[10];
	matrix.m23 = m_aux[14];
	matrix.m30 = m_aux[3];
	matrix.m31 = m_aux[7];
	matrix.m32 = m_aux[11];
	matrix.m33 = m_aux[15];
}

void CPhysicActor::Activate(bool _bActivate)
{
  if(m_pPhXActor)
  {
    if(_bActivate)
    {
      m_pPhXActor->clearBodyFlag(NX_BF_KINEMATIC);
      m_pPhXActor->clearActorFlag(NX_AF_DISABLE_COLLISION);
    }
    else
    {
      m_pPhXActor->raiseBodyFlag(NX_BF_KINEMATIC);
      m_pPhXActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
    }
  }
}

void CPhysicActor::AddForceAtLocalPos(Vect3f _vDirection, Vect3f _vLocalPos, float _fPower)
{
  if(m_pPhXActor)
  {
    NxVec3 l_vDirection(_vDirection.x,_vDirection.y,_vDirection.z);
    NxVec3 l_vLocalPos(_vLocalPos.x,_vLocalPos.y,_vLocalPos.z);
    NxF32 l_fCoeff = m_pPhXActor->getMass() * _fPower;
    m_pPhXActor->addForceAtLocalPos(l_vDirection*l_fCoeff, l_vLocalPos, NX_IMPULSE,true);
  }
}