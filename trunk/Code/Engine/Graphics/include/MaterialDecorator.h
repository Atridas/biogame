#pragma once
#include "Material.h"

class CMaterialDecorator : public CMaterial
{
public:
  CMaterialDecorator(CMaterial* _pDecoratedMaterial) : m_pDecoratedMaterial(_pDecoratedMaterial) {};
  virtual ~CMaterialDecorator() {CHECKED_DELETE(m_pDecoratedMaterial)};
  
  virtual float GetParallaxHeight()    const {return m_pDecoratedMaterial->GetParallaxHeight();};
  virtual float GetBump()              const {return m_pDecoratedMaterial->GetBump();};
  virtual float GetGlossiness()        const {return m_pDecoratedMaterial->GetGlossiness();};
  virtual float GetSpecularFactor()    const {return m_pDecoratedMaterial->GetSpecularFactor();};
  virtual bool  HasAlphaBlending()     const { return m_pDecoratedMaterial->HasAlphaBlending(); };
  virtual bool  HasSpecularParameters() const { return m_pDecoratedMaterial->HasSpecularParameters(); };

protected:
  CMaterial* m_pDecoratedMaterial;
};

