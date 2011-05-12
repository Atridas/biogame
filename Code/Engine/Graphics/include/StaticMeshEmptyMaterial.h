#pragma once
#include "EmptyMaterial.h"

class CStaticMeshEmptyMaterial : public CEmptyMaterial
{
public:
  CStaticMeshEmptyMaterial() {};
  ~CStaticMeshEmptyMaterial() {};

  virtual int GetMaterialType() {return STATIC_MESH_MATERIAL_MASK;};
};
