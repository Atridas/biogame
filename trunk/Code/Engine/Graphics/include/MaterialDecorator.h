#pragma once
#include "Material.h"

class CMaterialDecorator : public CMaterial
{
public:
  CMaterialDecorator(CMaterial* _pDecoratedMaterial) : m_pDecoratedMaterial(_pDecoratedMaterial) {};
  virtual ~CMaterialDecorator() {CHECKED_DELETE(m_pDecoratedMaterial)};

  virtual bool ReloadTextures()           const {return m_pDecoratedMaterial->ReloadTextures();};
                                          
  virtual float  GetParallaxHeight()       const {return m_pDecoratedMaterial->GetParallaxHeight();};
  virtual float  GetBump()                 const {return m_pDecoratedMaterial->GetBump();};
  virtual float  GetGlossiness()           const {return m_pDecoratedMaterial->GetGlossiness();};
  virtual float  GetSpecularFactor()       const {return m_pDecoratedMaterial->GetSpecularFactor();};
  virtual float  GetGlowIntensity()        const {return m_pDecoratedMaterial->GetGlowIntensity();};
  virtual float  GetEnvironmentIntensity() const {return m_pDecoratedMaterial->GetEnvironmentIntensity();};
  virtual Vect2f GetSpriteSize()           const {return m_pDecoratedMaterial->GetSpriteSize();};

  virtual void SetGlowIntensity(float _fGlowIntensity) {m_pDecoratedMaterial->SetGlowIntensity(_fGlowIntensity);};
  virtual void SetSpecularFactor(float _fSpecularFactor) {m_pDecoratedMaterial->SetSpecularFactor(_fSpecularFactor);};
  virtual void SetGlossines(float _fGlossiness) {m_pDecoratedMaterial->SetGlossines(_fGlossiness);};
  virtual void SetBump(float _fBump) {m_pDecoratedMaterial->SetBump(_fBump);};
  virtual void SetEnvironmentIntensity(float _fEnvironmentIntensity) {m_pDecoratedMaterial->SetEnvironmentIntensity(_fEnvironmentIntensity);};
  virtual void ActivateRadiosityNormal(ELightmapMode _eMode) {m_pDecoratedMaterial->ActivateRadiosityNormal(_eMode);};
  virtual void SetSpriteSize(const Vect2f& _vSize) {m_pDecoratedMaterial->SetSpriteSize(_vSize);};

  virtual bool  HasAlphaBlending()     const { return m_pDecoratedMaterial->HasAlphaBlending(); };
  virtual bool  HasSpecularParameters() const { return m_pDecoratedMaterial->HasSpecularParameters(); };

protected:
  CMaterial* m_pDecoratedMaterial;
};

