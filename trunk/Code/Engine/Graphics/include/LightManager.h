#pragma once

#ifndef __LIGHT_MANAGER_H__
#define __LIGHT_MANAGER_H__

#include "base.h"
#include "Light.h"
#include <Utils/MapManager.h>

// Forward declarations -------------
class CRenderManager;
class CDirectionalLight;
class CSpotLight;
class COmniLight;
//----------------------------------

//TODO fer que puguis canviar de XML dinàmicament BÉ
class CLightManager : public CMapManager<CLight>
{
public:
  CLightManager() : m_vAmbientLight(0) {};
  ~CLightManager()  {Done();};

  void Release(void);

  bool Load(const string& _szFileName);
  bool Reload();

  void Render(CRenderManager* _pRM) const;
  
  CLight* GetLight (int _i)  const;
  const vector<CLight*>& GetLights() const {return m_vLights;};

  const Vect3f& GetAmbientLight() const {return m_vAmbientLight;};
  void SetAmbientLight(Vect3f _vAmbient) {m_vAmbientLight = _vAmbient;};
  void SetLightsEnabled(bool _bEnabled);

  CDirectionalLight* CreateDirectionalLight(string _szName,
                                            Vect3f& _vPosition,
                                            Vect3f& _vDirection,
                                            CColor& _colColor,
                                            float _fStartRangeAtt,
                                            float _fEndRangeAtt,
                                            bool _bRenderShadows);

  CSpotLight* CreateSpotLight(string _szName,
                                            Vect3f& _vPosition,
                                            Vect3f& _vDirection,
                                            CColor& _colColor,
                                            float _fStartRangeAtt,
                                            float _fEndRangeAtt,
                                            float _fAngle,
                                            float _fFallOff,
                                            bool _bRenderShadows);

  COmniLight* CreateOmniLight(string _szName,
                                          Vect3f& _vPosition,
                                          CColor& _colColor,
                                          float _fStartRangeAtt,
                                          float _fEndRangeAtt);

private:
  Vect3f m_vAmbientLight;

  vector<CLight*> m_vLights;
  set<string>     m_sFileNames;
};

#endif
