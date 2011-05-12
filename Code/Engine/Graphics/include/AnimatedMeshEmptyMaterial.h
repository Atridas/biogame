#pragma once
#include "EmptyMaterial.h"

class CAnimatedMeshEmptyMaterial : public CEmptyMaterial
{
public:
  CAnimatedMeshEmptyMaterial() {};
  ~CAnimatedMeshEmptyMaterial() {};

  virtual int GetMaterialType() {return ANIMATED_MESH_MATERIAL_MASK;};
};

