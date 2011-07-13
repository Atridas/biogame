#pragma once
#include "Material.h"

class CMaterialDecorator : public CMaterial
{
public:
  CMaterialDecorator(CMaterial* _pDecoratedMaterial) : m_pDecoratedMaterial(_pDecoratedMaterial) {};
  virtual ~CMaterialDecorator() {CHECKED_DELETE(m_pDecoratedMaterial)};

  virtual bool ReloadTextures()           const {return m_pDecoratedMaterial->ReloadTextures();};
                                          
  virtual float GetParallaxHeight()       const {return m_pDecoratedMaterial->GetParallaxHeight();};
  virtual float GetBump()                 const {return m_pDecoratedMaterial->GetBump();};
  virtual float GetGlossiness()           const {return m_pDecoratedMaterial->GetGlossiness();};
  virtual float GetSpecularFactor()       const {return m_pDecoratedMaterial->GetSpecularFactor();};
  virtual float GetGlowIntensity()        const {return m_pDecoratedMaterial->GetGlowIntensity();};
  virtual float GetEnvironmentIntensity() const {return m_pDecoratedMaterial->GetEnvironmentIntensity();};

  virtual void SetGlowIntensity(const float& _fGlowIntensity) {m_pDecoratedMaterial->SetGlowIntensity(_fGlowIntensity);};
  virtual void SetSpecularFactor(const float& _fSpecularFactor) {m_pDecoratedMaterial->SetSpecularFactor(_fSpecularFactor);};
  virtual void SetGlossines(const float& _fGlossiness) {m_pDecoratedMaterial->SetGlossines(_fGlossiness);};
  virtual void SetBump(const float& _fBump) {m_pDecoratedMaterial->SetBump(_fBump);};
  virtual void SetEnvironmentIntensity(const float& _fEnvironmentIntensity) {m_pDecoratedMaterial->SetEnvironmentIntensity(_fEnvironmentIntensity);};
  virtual void ActivateRadiosityNormal(ELightmapMode _eMode) {m_pDecoratedMaterial->ActivateRadiosityNormal(_eMode);};

  virtual bool  HasAlphaBlending()     const { return m_pDecoratedMaterial->HasAlphaBlending(); };
  virtual bool  HasSpecularParameters() const { return m_pDecoratedMaterial->HasSpecularParameters(); };

protected:
  CMaterial* m_pDecoratedMaterial;
};

