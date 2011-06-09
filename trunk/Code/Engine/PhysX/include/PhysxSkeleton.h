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
  CPhysxSkeleton(bool _bActive) : m_pCalSkeleton(0), m_bRagdollActive(_bActive) {};
  ~CPhysxSkeleton() {Done();};

  virtual void Release();

  bool              Init(const string& _szFileName, CalModel* _pCalModel, Mat44f _vMat);
  CalSkeleton*      GetSkeleton()                         {return m_pCalSkeleton;};
  void              SetSkeleton(CalSkeleton* _pSkeleton)  {m_pCalSkeleton = _pSkeleton;};
  const Mat44f&     GetRenderableMatrix()                 {return m_vMat44;};


  bool              IsRagdollActive()                     {return m_bRagdollActive;};  
  void              ActivateRagdoll()                     {m_bRagdollActive = true;};
  void              ActivateCal3d()                       {m_bRagdollActive = false;};
  void              ToogleRagdollActive();
  void              WakeUpPhysxBones();
  void              SleepPhysxBones();

  //Funcions per inicialitzar el esquelet corresponent
  bool              Load(string _szFileName);
  void              InitParents();
  bool              InitPhysXJoints(string _szFileName);
  CPhysxBone*       GetPhysxBoneByName(string _szName);

  //Funcions per afegir joints al Skeleton.
  bool				              AddSphericalJoint(CXMLTreeNode _XMLObjects);
  bool				              AddFixedJoint(CXMLTreeNode _XMLObjects);
  bool				              AddRevoluteJoint(CXMLTreeNode _XMLObjects);
  SSphericalLimitInfo				GetJointParameterInfo(CXMLTreeNode _XMLObjects);
  

  void Update();
  void UpdateCal3dFromPhysx();
  void UpdatePhysxFromCal3d();

private:
  CalSkeleton*                    m_pCalSkeleton;
  Mat44f                          m_vMat44;
  vector<CPhysxBone*>             m_vBones;
  vector<CPhysicFixedJoint*>      m_vFixedJoints;
  vector<CPhysicSphericalJoint*>  m_vSphericalJoints;
  vector<CPhysicRevoluteJoint*>   m_vRevoluteJoints;
  bool                            m_bRagdollActive;
};

#endif 

