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
#define GLOW_PROPERTY_TYPE     5

class CMaterial
{
public:
  CMaterial() {};
  virtual ~CMaterial() {};

  virtual bool ReloadTextures()     const = 0;

  virtual void Activate() = 0;
  virtual void Activate(int _iTextureMask) = 0;
  virtual int   GetMaterialType()   const = 0;
  virtual float GetParallaxHeight() const = 0;
  virtual float GetBump()           const = 0;
  virtual float GetGlossiness()     const = 0;
  virtual float GetSpecularFactor() const = 0;
  virtual float GetGlowIntensity()  const = 0;
  virtual void SetGlowIntensity(const float& _fGlowIntensity) = 0;
  virtual void SetSpecularFactor(const float& _fSpecularFactor) = 0;
  virtual void SetGlossines(const float& _fGlossiness) = 0;
  virtual void SetBump(const float& _fBump) = 0;

  virtual bool HasAlphaBlending()      const = 0;
  virtual bool HasSpecularParameters() const = 0;
};
