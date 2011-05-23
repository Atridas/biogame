#pragma once
#include "base.h"

#define STATIC_MESH_MATERIAL_MASK   0x0000
#define ANIMATED_MESH_MATERIAL_MASK 0x0001
#define DIFFUSE_MATERIAL_MASK       0x0002
#define NORMALMAP_MATERIAL_MASK     0x0004
#define LIGHTMAP_MATERIAL_MASK      0x0008
#define SPECULARMAP_MATERIAL_MASK   0x0010
#define GLOW_MATERIAL_MASK          0x0020
#define PARALLAX_MATERIAL_MASK      0x0040

#define PARALLAX_PROPERTY_TYPE 1
#define BUMP_PROPERTY_TYPE     2
#define SPECULAR_PROPERTY_TYPE 3
#define ALPHA_PROPERTY_TYPE    4

class CMaterial
{
public:
  CMaterial() {};
  virtual ~CMaterial() {};

  virtual void Activate() = 0;
  virtual int   GetMaterialType()   const = 0;
  virtual float GetParallaxHeight() const = 0;
  virtual float GetBump()           const = 0;
  virtual float GetGlossiness()     const = 0;
  virtual float GetSpecularFactor() const = 0;

  virtual bool HasAlphaBlending()      const = 0;
  virtual bool HasSpecularParameters() const = 0;
};