#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "base.h"
#include "Utils/Object3D.h"
#include "Activable.h"
#include <XML/XMLTreeNode.h>
#include "Named.h"

// Forward declarations -------------
class CRenderManager;
class CColor;
class CEffect;
//-----------------------------------

class CLight : 
  public CObject3D,
  public CNamed,
  public CBaseControl,
  public CActivable
{
public:
  
	enum TLightType
  {
	  OMNI=0,
	  DIRECTIONAL,
	  SPOT
  };

public:
  CLight(const string& _name) :  
              CNamed(_name),
              m_colColor(colWHITE),
              m_Type(OMNI),
              m_bRenderShadows(false),
              m_bDynamicObjectsOnly(false),
              m_fStartRangeAttenuation(0.0f),
              m_fEndRangeAttenuation(0.0f)    {};

  virtual ~CLight(){};
  virtual void Init(CXMLTreeNode& _XMLParams);
  void SetColor(const CColor& _colColor) {m_colColor = _colColor;};
  const CColor& GetColor() const {return m_colColor;};
  void SetPosition(const Vect3f& _vPosition) {m_vPosition = _vPosition;};
  Vect3f GetPosition() const {return m_vPosition;};
  //void SetSpecular(float _fSpecular);
  //float GetSpecular() const;

  virtual bool UsesGeometryInDeferred() = 0;
  virtual void RenderDeferredLight(CRenderManager* _pRM, CEffect* _pGeometryEffect) {};

  void SetStartRangeAttenuation(const float _fStartRangeAttenuation) {m_fStartRangeAttenuation = _fStartRangeAttenuation;};
  float GetStartRangeAttenuation() const {return m_fStartRangeAttenuation;};
  void SetEndRangeAttenuation(const float _fEndRangeAttenuation) {m_fEndRangeAttenuation = _fEndRangeAttenuation;};
  float GetEndRangeAttenuation() const {return m_fEndRangeAttenuation;};
  void SetRenderShadows(bool _bRenderShadows) {m_bRenderShadows = _bRenderShadows;};
  bool GetRenderShadows() const {return m_bRenderShadows;};

  void SetDynamicObjectsOnly(bool _bDynamicObjectsOnly) {m_bDynamicObjectsOnly = _bDynamicObjectsOnly;};
  bool GetDynamicObjectsOnly() {return m_bDynamicObjectsOnly;};

  void SetType(const TLightType _Type) {m_Type = _Type;};
  TLightType GetType() const {return m_Type;};

  void SetName(const string& _name) {CNamed::SetName(_name);};


  virtual void Render(CRenderManager* _pRM) const = 0;
 

protected:
  static TLightType GetLightTypeByName(const string& _szLightType);

  CColor m_colColor;
  //float m_fSpecular;
  TLightType m_Type;
  bool m_bRenderShadows;
  float m_fStartRangeAttenuation;
  float m_fEndRangeAttenuation;

  bool m_bDynamicObjectsOnly;
 
};
#endif