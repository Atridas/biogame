#pragma once
#ifndef INC_PHYSICS_BONE_H_
#define INC_PHYSICS_BONE_H_

#include "base.h"

class CalBone;

class CPhysxBone : CBaseControl
{
public:
  CPhysxBone() : m_pCalBone(0) {};
  ~CPhysxBone() {Done();};

  bool Init();
  virtual void Release();

  void UpdateCal3dFromPhysx();

private:

  CalBone* m_pCalBone;
};

#endif