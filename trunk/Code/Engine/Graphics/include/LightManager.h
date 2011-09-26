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

  virtual void Remove(const std::string &_szName);

  CDirectionalLight* CreateDirectionalLight(const string& _szName,
                                            const Vect3f& _vPosition,
                                            const Vect3f& _vDirection,
                                            const CColor& _colColor,
                                            float _fStartRangeAtt,
                                            float _fEndRangeAtt,
                                            bool _bRenderShadows);

  CSpotLight* CreateSpotLight(const string& _szName,
                                            const Vect3f& _vPosition,
                                            const Vect3f& _vDirection,
                                            const CColor& _colColor,
                                            float _fStartRangeAtt,
                                            float _fEndRangeAtt,
                                            float _fAngle,
                                            float _fFallOff,
                                            bool _bRenderShadows);

  COmniLight* CreateOmniLight(const string& _szName,
                                          const Vect3f& _vPosition,
                                          const CColor& _colColor,
                                          float _fStartRangeAtt,
                                          float _fEndRangeAtt);

private:
  Vect3f m_vAmbientLight;

  vector<CLight*> m_vLights;
  set<string>     m_sFileNames;
};

#endif
