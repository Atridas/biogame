#pragma once
#ifndef __EFFECT_TECHNIQUE_H__
#define __EFFECT_TECHNIQUE_H__

#include "base.h"
#include "Named.h"
#include <d3d9.h>

//forward declarations ----------------------------------------------
class CEffect;
class CXMLTreeNode;
//-------------------------------------------------------------------


class CEffectTechnique:
  public CBaseControl,
  public CNamed
{
public:
  CEffectTechnique(string& _szName):
                      CNamed(_szName),
                      m_bUseCameraPosition(false),
                      m_bUseInverseProjMatrix(false),
                      m_bUseInverseViewMatrix(false),
                      m_bUseInverseWorldMatrix(false),
                      m_bUseLights(false),
                      m_bUseLightAmbientColor(false),
                      m_bUseProjMatrix(false),
                      m_bUseViewMatrix(false),
                      m_bUseWorldMatrix(false),
                      m_bUseWorldViewMatrix(false),
                      m_bUseWorldViewProjectionMatrix(false),
                      m_bUseViewProjectionMatrix(false),
                      m_bUseViewToLightProjectionMatrix(false),
                      m_bUseTime(false),
                      m_iNumOfLights(0),
                      m_pEffect(0),
                      m_pD3DTechnique(0)
                  {};
  ~CEffectTechnique() {};

  void Init(CXMLTreeNode& _XMLParams);

  inline CEffect * GetEffect() const {return m_pEffect;}
  D3DXHANDLE GetD3DTechnique() { return m_pD3DTechnique; };

  bool BeginRender();
  bool Refresh();  
  

protected:
  void Release();

private:
  bool m_bUseCameraPosition;
  bool m_bUseInverseProjMatrix;
  bool m_bUseInverseViewMatrix;
  bool m_bUseInverseWorldMatrix;
  bool m_bUseLights;
  bool m_bUseLightAmbientColor;
  bool m_bUseProjMatrix;
  bool m_bUseViewMatrix;
  bool m_bUseWorldMatrix;
  bool m_bUseWorldViewMatrix;
  bool m_bUseWorldViewProjectionMatrix;
  bool m_bUseViewProjectionMatrix;
  bool m_bUseViewToLightProjectionMatrix;
  bool m_bUseTime;
  int m_iNumOfLights;
  CEffect* m_pEffect;
  D3DXHANDLE m_pD3DTechnique;
};

#endif