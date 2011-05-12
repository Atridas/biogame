#pragma once
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include <base.h>
#include <Named.h>

#include <d3d9.h>

//Forward declarations-------------
class CXMLTreeNode;
//---------------------------------

class CEffect :
  public CBaseControl,
  public CNamed
{
public:
  CEffect() : CNamed(""),
              m_szFileName(""),
              m_pD3DEffect(0),
              m_pD3DTechnique(0),
              m_pD3DInstancedTechnique(0),
              m_pEffectPool(0) {};

  virtual ~CEffect(void) {Done();};

  //bool SetLights(size_t _iNumOfLights);
  bool Init(const CXMLTreeNode& _xmlEffect,LPD3DXEFFECTPOOL _pEffectPool);
  bool Reload();

  LPD3DXEFFECT GetD3DEffect() const {return m_pD3DEffect;};
  //D3DXHANDLE GetTechniqueByName(const string& _szTechniqueName) const;
  
protected:
  void Release();
private:
  //void SetNullParameters();
  //bool InitParameters();
  //void GetParameterBySemantic(const string& _szSemanticName, D3DXHANDLE& _pHandle);
  bool LoadEffect();

  /*
  string m_szFileName;
  LPD3DXEFFECT m_pD3DEffect;
  float m_aAmbientLight[3];
  BOOL m_aLightsEnabled[MAX_LIGHTS_BY_SHADER];
  int m_aLightsType[MAX_LIGHTS_BY_SHADER];
  float m_aLightsAngle[MAX_LIGHTS_BY_SHADER];
  float m_aLightsFallOff[MAX_LIGHTS_BY_SHADER];
  float m_aLightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  float m_aLightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  Vect3f m_aLightsPosition[MAX_LIGHTS_BY_SHADER];
  Vect3f m_aLightsDirection[MAX_LIGHTS_BY_SHADER];
  CColor m_aLightsColor[MAX_LIGHTS_BY_SHADER];
  BOOL m_aShadowsEnabled[MAX_LIGHTS_BY_SHADER];


  D3DXHANDLE m_pWorldMatrixParameter, m_pViewMatrixParameter, m_pProjectionMatrixParameter;
  D3DXHANDLE m_pWorldViewMatrixParameter,  m_pViewProjectionMatrixParameter, m_pWorldViewProjectionMatrixParameter;
  D3DXHANDLE m_pViewToLightProjectionMatrixParameter;

  D3DXHANDLE m_pAmbientLight;
  D3DXHANDLE m_pLightsEnabledParameter, m_pLightsTypeParameter, m_pLightsPositionParameter, m_pLightsDirectionParameter, m_pLightsAngleParameter, m_pLightsColorParameter;
  D3DXHANDLE m_pLightsFallOffParameter, m_pLightsStartRangeAttenuationParameter, m_pLightsEndRangeAttenuationParameter;

  D3DXHANDLE m_pCameraPositionParameter;
  D3DXHANDLE m_pBonesParameter;
  D3DXHANDLE m_pTimeParameter;
  D3DXHANDLE m_pGlowActive;
  D3DXHANDLE m_pTextureWidth, m_pTextureHeight; 
  D3DXHANDLE m_pPoissonBlurKernelParameter;
  
  D3DXHANDLE m_pShadowsEnabledParameter;
  */

  string m_szFileName;
  string m_szTechniqueName;
  string m_szInstancedTechniqueName;
  LPD3DXEFFECT m_pD3DEffect;
  D3DXHANDLE m_pD3DTechnique;
  D3DXHANDLE m_pD3DInstancedTechnique;
  LPD3DXEFFECTPOOL m_pEffectPool;
};

#endif
