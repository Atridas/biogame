#pragma once
#include "EmptyMaterial.h"

class CAnimatedMeshEmptyMaterial : public CEmptyMaterial
{
public:
  CAnimatedMeshEmptyMaterial() {};
  ~CAnimatedMeshEmptyMaterial() {};

  virtual int GetMaterialType() const {return ANIMATED_MESH_MATERIAL_MASK;};
};

