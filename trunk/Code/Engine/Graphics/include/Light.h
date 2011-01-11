#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "base.h"
#include "Object3D.h"
#include <XML/XMLTreeNode.h>

// Forward declarations -------------
class CRenderManager;
class CColor;
//-----------------------------------

class CLight : public CObject3D
{
public:
  
	enum TLightType
  {
	  OMNI=0,
	  DIRECTIONAL,
	  SPOT
  };

protected:
  CColor m_colColor;
  //float m_fSpecular;
  TLightType m_Type;
  string m_szName;
  bool m_bRenderShadows;
  float m_fStartRangeAttenuation;
  float m_fEndRangeAttenuation;

  static TLightType GetLightTypeByName(const string& _szLightType);

public:
  CLight() :  m_colColor(colWHITE),
              m_szName(""),
              m_Type(OMNI),
              m_bRenderShadows(false),
              m_fStartRangeAttenuation(0.0f),
              m_fEndRangeAttenuation(0.0f)    {};

  virtual ~CLight(){};

  virtual void Init(CXMLTreeNode& _XMLParams);

  void SetName(const string& _szName) {m_szName = _szName;};
  const string& GetName() {return m_szName;};
  void SetColor(const CColor& _colColor) {m_colColor = _colColor;};
  const CColor& GetColor() const {return m_colColor;};
  void SetPosition(Vect3f& _vPosition) {m_vPosition = _vPosition;};
  Vect3f GetPosition() {return m_vPosition;};
  //void SetSpecular(float _fSpecular);
  //float GetSpecular() const;

  void SetStartRangeAttenuation(const float _fStartRangeAttenuation) {m_fStartRangeAttenuation = _fStartRangeAttenuation;};
  float GetStartRangeAttenuation() const {return m_fStartRangeAttenuation;};
  void SetEndRangeAttenuation(const float _fEndRangeAttenuation) {m_fEndRangeAttenuation = _fEndRangeAttenuation;};
  float GetEndRangeAttenuation() const {return m_fEndRangeAttenuation;};
  void SetRenderShadows(bool _bRenderShadows) {m_bRenderShadows = _bRenderShadows;};
  bool RenderShadows() const {return false;};

  void SetType(const TLightType _Type) {m_Type = _Type;};
  TLightType GetType() const {return m_Type;};


  virtual void Render(CRenderManager* _pRM) const = 0;
 
 
};
#endif