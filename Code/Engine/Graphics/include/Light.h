#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "base.h"
#include "Object3D.h"


// Forward declarations -------------
class CRenderManager;
class CColor;


//-----------------------------------
class CLight : public CObject3D
{
public:
  enum TLightType  {OMNI=0,DIRECTIONAL,SPOT};
protected:
  CColor m_Color;
  float m_Specular;
  TLightType m_Type;
  std::string m_Name;
  static TLightType GetLightTypeByName(const std::string &StrLightType);
public:
  CLight(){};
  virtual ~CLight(){};
  void SetName(const std::string &Name);
  const std::string &GetName();
  void SetColor(const CColor &Color);
  const CColor & GetColor() const;
  void SetSpecular(float Specular);
  float GetSpecular() const;
  void SetType(const TLightType Type);
  TLightType GetType() const;
  virtual void Render(CRenderManager *RM);
 
 
};
#endif