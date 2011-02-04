#pragma once
#ifndef __EFFECT_MATERIAL_H__
#define __EFFECT_MATERIAL_H__

#include "base.h"

#define MATERIAL_INFO_VERTEXTYPE  0X0001
#define MATERIAL_INFO_TEXTURETYPE 0x0002

#define TEXTURE_TYPE_DIFFUSSE    0X0001
#define TEXTURE_TYPE_NORMALMAP   0X0002
#define TEXTURE_TYPE_LIGHTMAN    0X0004
#define TEXTURE_TYPE_ENVIRONMENT 0X0008
#define TEXTURE_TYPE_GLOW        0X0010

//---- Forward Declarations ----------------------
class CEffect;
class CTexture;
class CEffectTechnique;
class CRenderManager;
//------------------------------------------------

class CEffectMaterial: public CBaseControl
{
public:
  CEffectMaterial(void):m_pTechnique(0), m_usTextureMask(0), m_usVertexType(0), m_usMaterialInfo(0) {};
  ~CEffectMaterial(void) {Done();};
  
  //bool Init(bool _bAnimated, int _iTextureMask, int _iVertexType, const vector<CTexture*>& _vTextures);
  bool Init(fstream& _File);

  int GetTextureMask() const {return m_usTextureMask;};
  int GetVertexType()  const {return m_usVertexType;};
  int GetMaterialInfo()const {return m_usMaterialInfo;};
  bool IsAnimated()    const {return m_bAnimated;};

  CEffectTechnique* ActivateMaterial(const CRenderManager* _pRM) const;

  static int NumTextures(int mask);

protected:
  void Release();

private:
  bool               m_bAnimated;
  uint16             m_usTextureMask;
  uint16             m_usVertexType;
  uint16             m_usMaterialInfo;
  vector<CTexture *> m_vTextures;
  CEffectTechnique*  m_pTechnique;
};

#endif
