#pragma once
#ifndef INC_PHYSICS_SKELETON_H_
#define INC_PHYSICS_SKELETON_H_

#include "base.h"

class CPhysxBone;
class CalSkeleton;
class CalModel;

class CPhysxSkeleton : public CBaseControl
{
public:
  CPhysxSkeleton() : m_pCalSkeleton(0) {};
  ~CPhysxSkeleton() {Done();};

  virtual void Release();

  bool Init(const string& _szFileName, CalModel* _pCalModel);

  void UpdateCal3dFromPhysx();

private:
  CalSkeleton* m_pCalSkeleton;
  vector<CPhysxBone*> m_vBones;
};

#endif 

