#pragma once
#ifndef INC_PHYSICS_SKELETON_H_
#define INC_PHYSICS_SKELETON_H_

#include "base.h"

class CPhysxBone;
class CalSkeleton;
class CalModel;
class CPhysicFixedJoint;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CXMLTreeNode;
struct SSphericalLimitInfo;

class CPhysxSkeleton : public CBaseControl
{
public:
  CPhysxSkeleton() : m_pCalSkeleton(0) {};
  ~CPhysxSkeleton() {Done();};

  virtual void Release();

  bool              Init(const string& _szFileName, CalModel* _pCalModel, Mat44f _vMat);
  CalSkeleton*      GetSkeleton() {return m_pCalSkeleton;};
  void              SetSkeleton(CalSkeleton* _pSkeleton) {m_pCalSkeleton = _pSkeleton;};
  const Mat44f&     GetRenderableMatrix()                  {return m_vMat44;};

  //Funcions per inicialitzar el esquelet corresponent
  bool              Load(string _szFileName);
  void              InitBoneMatrices();
  void              InitParents();
  void              InitPhysXActors();
  bool              InitPhysXJoints(string _szFileName);
  CPhysxBone*       GetPhysxBoneByName(string _szName);

  //Funcions per afegir joints al Skeleton.
  bool				              AddSphericalJoint(CXMLTreeNode _XMLObjects);
  bool				              AddFixedJoint(CXMLTreeNode _XMLObjects);
  bool				              AddRevoluteJoint(CXMLTreeNode _XMLObjects);
  SSphericalLimitInfo				GetJointParameterInfo(CXMLTreeNode _XMLObjects);
  


  void UpdateCal3dFromPhysx();
  void UpdatePhysxBone(CPhysxBone* _pPhysxBone);

private:
  CalSkeleton*                    m_pCalSkeleton;
  Mat44f                          m_vMat44;
  vector<CPhysxBone*>             m_vBones;
  vector<CPhysicFixedJoint*>      m_vFixedJoints;
  vector<CPhysicSphericalJoint*>  m_vSphericalJoints;
  vector<CPhysicRevoluteJoint*>   m_vRevoluteJoints;
};

#endif 

