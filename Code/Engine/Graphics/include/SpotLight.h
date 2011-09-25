#pragma once

#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include "base.h"
#include "DirectionalLight.h"

class CRenderableVertexs;

class CSpotLight : public CDirectionalLight
{
public:
  CSpotLight(const string& _name) : 
      CDirectionalLight(_name),
      m_fAngle(0.0f), 
      m_fFallOff(0.0f) {m_Type = CLight::SPOT;};

  ~CSpotLight(){};

  virtual void Init(CXMLTreeNode& _XMLParams);

  void SetAngle(float _fAngle) {m_fAngle = _fAngle;};
  float GetAngle() const {return m_fAngle;};
  void SetFallOff(const float _fFallOff) {m_fFallOff = _fFallOff;};
  float GetFallOff() const {return m_fFallOff;};
  
  virtual bool UsesGeometryInDeferred() { return true; };
  virtual void RenderDeferredLight(CRenderManager* _pRM, CEffect* _pGeometryEffect);

protected:
  void Release() {};

  float m_fAngle;
  float m_fFallOff;
};

#endif
