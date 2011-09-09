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
              m_iTextureMask(0),
              m_pD3DEffect(0),
              m_pD3DTechnique(0),
              m_pD3DInstancedTechnique(0),
              m_pD3DAlphaTechnique(0) {};

  virtual ~CEffect(void) {Done();};

  bool Init(const CXMLTreeNode& _xmlEffect,LPD3DXEFFECTPOOL _pEffectPool);
  bool Reload(LPD3DXEFFECTPOOL _pEffectPool);
  
  int GetTextureMask() const {return m_iTextureMask;};

  void ActivateDefaultRendering();
  void ActivateAlphaRendering();
  void ActivateInstancedRendering();

  LPD3DXEFFECT GetD3DEffect() const {return m_pD3DEffect;};

  void SetValue(const string& _szParameterName,int _iValue);
  void SetValue(const string& _szParameterName,float _fValue);
  void SetValue(const string& _szParameterName,Vect3f& _vValue);
  void SetValue(const string& _szParameterName,Mat44f _matValue);

  void SetValue(const string& _szParameterName,int _iValue[], uint32 _uiSize);
  void SetValue(const string& _szParameterName,float _fValue[], uint32 _uiSize);
  
protected:
  void Release();
private:

  bool LoadEffect(LPD3DXEFFECTPOOL _pEffectPool);

  string m_szFileName;
  string m_szTechniqueName;
  string m_szInstancedTechniqueName;
  string m_szD3DAlphaTechniqueName;

  int m_iTextureMask;

  LPD3DXEFFECT m_pD3DEffect;
  D3DXHANDLE m_pD3DTechnique;
  D3DXHANDLE m_pD3DInstancedTechnique;
  D3DXHANDLE m_pD3DAlphaTechnique;

};

#endif
