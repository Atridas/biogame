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

class CPhysxSkeleton : public CBaseControl
{
public:
  CPhysxSkeleton() : m_pCalSkeleton(0) {};
  ~CPhysxSkeleton() {Done();};

  virtual void Release();

  bool              Init(const string& _szFileName, CalModel* _pCalModel);
  CalSkeleton*      GetSkeleton() {return m_pCalSkeleton;};
  void              SetSkeleton(CalSkeleton* _pSkeleton) {m_pCalSkeleton = _pSkeleton;};

  //Funcions per inicialitzar el esquelet corresponent
  bool              Load(string _szFileName);
  void              InitBoneMatrices();
  void              InitPhysXActors();
  bool              InitPhysXJoints(string _szFileName);
  CPhysxBone*       GetPhysxBoneByName(string _szName);


  void UpdateCal3dFromPhysx();

private:
  CalSkeleton* m_pCalSkeleton;
  vector<CPhysxBone*> m_vBones;
  vector<CPhysicFixedJoint*> m_vFixedJoints;
  vector<CPhysicSphericalJoint*> m_vSphericalJoints;
  vector<CPhysicRevoluteJoint*> m_vRevoluteJoints;
};

#endif 

