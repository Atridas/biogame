#pragma once
#include "Material.h"

class CMaterialDecorator : public CMaterial
{
public:
  CMaterialDecorator(CMaterial* _pDecoratedMaterial) : m_pDecoratedMaterial(_pDecoratedMaterial) {};
  virtual ~CMaterialDecorator() {CHECKED_DELETE(m_pDecoratedMaterial)};
  
  virtual float GetParallaxHeight() {return m_pDecoratedMaterial->GetParallaxHeight();};
  virtual float GetBump() {return m_pDecoratedMaterial->GetBump();};
  virtual float GetGlossiness() {return m_pDecoratedMaterial->GetGlossiness();};
  virtual float GetSpecularFactor() {return m_pDecoratedMaterial->GetSpecularFactor();};

protected:
  CMaterial* m_pDecoratedMaterial;
};

