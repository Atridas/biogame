#pragma once
#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "base.h"
#include "Effect.h"
#include "Utils/MapManager.h"
#include "params.h"


#define MAX_LIGHTS_BY_SHADER 4

//forward declarations ----------------------------------------------
class CLightManager;
class CalSkeleton;
class CalHardwareModel;
class CLight;
//-------------------------------------------------------------------

class CEffectManager:
  public CMapManager<CEffect>
{
private:
  typedef map<int,string> TDefaultEffectMap;

public:
  CEffectManager(): m_pLightManager(0),
                    m_pEffectPool(0),
                    m_szFileName(""),
                    m_vCameraEye(0.0f),
                    m_pCalSkeleton(0),
                    m_pCalHardwareModel(0),
                    m_iTextureWidth(0),
                    m_iTextureHeight(0),
                    m_iViewportX(0),
                    m_iViewportY(0),
                    m_iViewportWidth(0),
                    m_iViewportHeight(0),
                    m_bWorldMatrixUpdated(false),
                    m_bProjectionMatrixUpdated(false),
                    m_bViewMatrixUpdated(false),
                    m_bLightViewMatrixUpdated(false), 
                    m_bViewProjectionMatrixUpdated(false),
                    m_bWorldViewMatrixUpdated(false),
                    m_bWorldViewProjectionMatrixUpdated(false),
                    m_bCameraEyeUpdated(false),
                    m_bCameraUpUpdated(false),
                    m_bCameraRightUpdated(false),
                    m_bViewProjectionUpdated(false),
                    m_bWorldViewUpdated(false),
                    m_bWorldViewProjectionUpdated(false),
                    m_bLightsUpdated(false),
                    m_bLightUpdated(false),
                    m_bSkeletonUpdated(false),
                    m_bTextureWidthHeightUpdated(false),
                    m_bAlphaFactorUpdated(false),
                    m_bPoissonBlurKernelUpdated(true),
                    m_bSpecularParamsUpdated(true),
                    m_bGlowUpdated(false),
                    m_bSpecularUpdated(false),
                    m_bSpecularActive(false),
                    m_bEnvironmentUpdated(false),
                    m_bTimeUpdated(false),
                    m_bSpriteSizeUpdated(false),
                    m_bHDRParamsUpdated(true),
                    m_bViewportUpdated(true),
                    m_fTime(0.0f),
                    m_fEnvironmentIntensity(0.f),
                    m_fGlossiness(0.f),
                    m_fSpecularLevel(0.f),
                    m_fBrightPassThreshold(.8f),
                    m_fExposure(.5f),
                    m_fGaussMultiplier(.4f),
                    m_fMaxLuminanceLowerLimit  (0.25f), m_fMaxLuminanceUpperLimit  (2.f),
                    m_fSceneLuminanceLowerLimit(0.25f), m_fSceneLuminanceUpperLimit(1.f),
                    m_fBloomFinalScale(0.25f),
                    m_fGlowToBloom(1.f),
                    m_fGlowLuminanceScale(0.5f),
                    m_fMaxGlowLuminance(4.f),
                    m_fGlowFinalScale(1.f)
                    {SetOk(true);};

  ~CEffectManager() {Done();};
  
  void SetWorldMatrix(const Mat44f& _mMatrix)
  {
    m_mWorldMatrix = _mMatrix;
    m_bWorldMatrixUpdated=
    m_bWorldViewMatrixUpdated=
    m_bWorldViewProjectionMatrixUpdated=
    m_bWorldViewUpdated=
    m_bWorldViewProjectionUpdated=true;
  };

  void SetProjectionMatrix(const Mat44f& _mMatrix)
  {
    m_mProjectionMatrix = _mMatrix;
    m_bProjectionMatrixUpdated=
    m_bViewProjectionMatrixUpdated=
    m_bWorldViewProjectionMatrixUpdated=
    m_bViewProjectionUpdated=
    m_bWorldViewUpdated=
    m_bWorldViewProjectionUpdated=true;
  };

  void SetViewMatrix(const Mat44f& _mMatrix)
  {
    m_mViewMatrix = _mMatrix;
    m_bLightViewMatrixUpdated =
    m_bViewMatrixUpdated =
    m_bViewProjectionMatrixUpdated =
    m_bWorldViewMatrixUpdated =
    m_bWorldViewProjectionMatrixUpdated =
    m_bViewProjectionUpdated=
    m_bWorldViewProjectionUpdated=true;
  };

  void SetLightViewMatrix(const Mat44f& _mMatrix)
  {
    m_mLightViewMatrix = _mMatrix;
    m_bLightViewMatrixUpdated=true;
  };

  void SetShadowProjectionMatrix(const Mat44f& _mMatrix)
  {
    m_mShadowProjectionMatrix = _mMatrix;
    m_bLightViewMatrixUpdated=true;
  };
  
  void SetCameraEye(const Vect3f& _vCameraEye)
  {
    m_vCameraEye = _vCameraEye;
    m_bCameraEyeUpdated=true;
  };

  void SetCameraUp(const Vect3f& _vCameraUp)
  {
    m_vCameraUp = _vCameraUp;
    m_bCameraUpUpdated=true;
  };

  void SetCameraRight(const Vect3f& _vCameraRight)
  {
    m_vCameraRight = _vCameraRight;
    m_bCameraRightUpdated=true;
  };

  void SetSpecular(bool _bSpecular)
  {
    m_bSpecularActive = _bSpecular;
    m_bSpecularUpdated = true;
  };

  void SetLight(CLight* _pLight);

  void SetSpecularParams(float _fGlossiness, float _fSpecularLevel);
  const Mat44f& GetWorldMatrix() const {return m_mWorldMatrix;};
  const Mat44f& GetProjectionMatrix() const {return m_mProjectionMatrix;};
  const Mat44f& GetViewMatrix() const {return m_mViewMatrix;};
  const Mat44f& GetLightViewMatrix() const {return m_mLightViewMatrix;};
  const Mat44f& GetShadowProjectionMatrix() {return m_mShadowProjectionMatrix;};
  //Compostes
  //const Mat44f& GetInverseProjectionMatrix();
  //const Mat44f& GetInverseViewMatrix();
  //const Mat44f& GetInverseWorldMatrix();
  const Mat44f& GetViewProjectionMatrix();
  const Mat44f& GetWorldViewMatrix();
  const Mat44f& GetWorldViewProjectionMatrix();
  //non matrix
  const Vect3f& GetCameraEye() {return m_vCameraEye;};
  size_t GetMaxLights() const { return MAX_LIGHTS_BY_SHADER; };

  CEffect* GetEffect(const string& _szName) {return GetResource(_szName);};

  void SetSkeleton(CalSkeleton* _pSkeleton, CalHardwareModel* _pCalHardwareModel) {m_pCalSkeleton = _pSkeleton; m_pCalHardwareModel = _pCalHardwareModel; m_bSkeletonUpdated = true;};
  void SetTextureWidthHeight(int _iWidth, int _iHeight) {m_iTextureWidth  = _iWidth; m_iTextureHeight = _iHeight; m_bTextureWidthHeightUpdated = true;};
  void GetTextureWidthHeight(int& _iWidth, int& _iHeight) {_iWidth = m_iTextureWidth; _iHeight = m_iTextureHeight;};
  void SetViewport(int  _iX, int  _iY, int  _iWidth, int  _iHeight) {m_iViewportX  = _iX; m_iViewportY = _iY; m_iViewportWidth  = _iWidth; m_iViewportHeight = _iHeight; m_bViewportUpdated = true;};
  void GetViewport(int& _iX, int& _iY, int& _iWidth, int& _iHeight) {_iX = m_iViewportX; _iY = m_iViewportY; _iWidth = m_iViewportWidth; _iHeight = m_iViewportHeight;};
  void SetAlphaFactor(float _fAlphaFactor) {m_fAlphaFactor = _fAlphaFactor; m_bAlphaFactorUpdated = true;};
  void SetTime(float _bTime) {m_fTime = _bTime; m_bTimeUpdated = true;};
  void SetGlow(bool _bGlow) {m_bGlowActive = _bGlow; m_bGlowUpdated = true;};
  void SetGlowIntensity(float _fGlowIntensity) {m_fGlowIntensity = _fGlowIntensity; m_bGlowUpdated = true;};
  void SetEnvironmentIntensity(float _fEnvironmentIntensity) {if(_fEnvironmentIntensity != m_fEnvironmentIntensity) {m_fEnvironmentIntensity = _fEnvironmentIntensity; m_bEnvironmentUpdated = true;}};
  void SetSpriteSize(const Vect2f _vSpriteSize) {if(_vSpriteSize != m_vSpriteSize) { m_vSpriteSize = _vSpriteSize; m_bSpriteSizeUpdated = true; } };

  void SetBrightPassThreshold (float _fBrightPassThreshold);
  void SetExposure            (float _fExposure);
  void SetGaussMultiplier     (float _fGaussMultiplier);
  void SetMaxLuminanceLimits  (float _fMaxLuminanceLowerLimit, float _fMaxLuminanceUpperLimit);
  void SetSceneLuminanceLimits(float _fSceneLuminanceLowerLimit, float _fSceneLuminanceUpperLimit);
  void SetBloomFinalScale     (float _fBloomFinalScale    );
  void SetGlowToBloom         (float _fGlowToBloom        );
  void SetGlowLuminanceScale  (float _fGlowLuminanceScale );
  void SetMaxGlowLuminance    (float _fMaxGlowLuminance   );
  void SetGlowFinalScale      (float _fGlowFinalScale     );

  void PrintHDRParams() const;
  
  void ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye, const Vect3f& _vCameraUp, const Vect3f& _vCameraRight);

  void Begin(void) {m_bLightsUpdated = true; ActivateDefaultRendering();};
  
  void ActivateDefaultRendering(void);
  void ActivateAlphaRendering(void);
  void ActivateInstancedRendering(void);

  void LoadShaderData(CEffect* _pEffect);

  bool Load(const SEffectManagerParams& _params);
  void Reload();
  void CleanUp() { Release(); };

protected:
  void Release();

private:

  bool Load(bool _bReload);

  CLightManager* m_pLightManager;
  LPD3DXEFFECTPOOL m_pEffectPool;

  CalSkeleton* m_pCalSkeleton;
  CalHardwareModel* m_pCalHardwareModel;

  string m_szFileName;

  Mat44f m_mWorldMatrix;
  Mat44f m_mProjectionMatrix;
  Mat44f m_mViewMatrix;
  Mat44f m_mLightViewMatrix;
  Mat44f m_mShadowProjectionMatrix;
  Mat44f m_mViewProjectionMatrix;
  Mat44f m_mWorldViewMatrix;
  Mat44f m_mWorldViewProjectionMatrix;
  Vect3f m_vCameraEye;
  Vect3f m_vCameraUp;
  Vect3f m_vCameraRight;
  
  int m_iTextureWidth;
  int m_iTextureHeight;
  int m_iViewportX;
  int m_iViewportY;
  int m_iViewportWidth;
  int m_iViewportHeight;
  float m_fAlphaFactor;

  float m_pfPoissonBlurKernel[32];

  //Variables actualitzadeds al shader
  bool m_bWorldMatrixUpdated;
  bool m_bProjectionMatrixUpdated;
  bool m_bViewMatrixUpdated;
  bool m_bLightViewMatrixUpdated;
  //bool m_bShadowProjectionMatrixUpdated;
  bool m_bViewProjectionMatrixUpdated;
  bool m_bWorldViewMatrixUpdated;
  bool m_bWorldViewProjectionMatrixUpdated;
  bool m_bCameraEyeUpdated;
  bool m_bCameraUpUpdated;
  bool m_bCameraRightUpdated;
  bool m_bSkeletonUpdated;
  bool m_bTextureWidthHeightUpdated;
  bool m_bViewportUpdated;
  bool m_bAlphaFactorUpdated;
  bool m_bPoissonBlurKernelUpdated;
  bool m_bSpecularParamsUpdated;
  bool m_bGlowUpdated;
  bool m_bSpecularUpdated;
  bool m_bEnvironmentUpdated;
  bool m_bHDRParamsUpdated;

  //Matrius compostes recalculades
  bool m_bViewProjectionUpdated;
  bool m_bWorldViewUpdated;
  bool m_bWorldViewProjectionUpdated;

  bool m_bLightsUpdated;
  bool m_bLightUpdated;
  bool m_bSpriteSizeUpdated;

  float m_bTimeUpdated;

  //Parametres
  float m_fGlossiness;
  float m_fSpecularLevel;
  bool m_bSpecularActive;
  bool m_bGlowActive;
  float m_fGlowIntensity;
  float m_fEnvironmentIntensity;
  float m_fTime;
  Vect2f m_vSpriteSize;
  float m_fBrightPassThreshold;
  float m_fExposure;
  float m_fGaussMultiplier;
  float m_fMaxLuminanceLowerLimit,   m_fMaxLuminanceUpperLimit;
  float m_fSceneLuminanceLowerLimit, m_fSceneLuminanceUpperLimit;
  float m_fBloomFinalScale;
  float m_fGlowToBloom;
  float m_fGlowLuminanceScale;
  float m_fMaxGlowLuminance;
  float m_fGlowFinalScale;

  bool    m_bLightEnabled;
  int     m_iLightType;
  Vect3f  m_vLightPosition;
  Vect3f  m_vLightDirection;
  CColor  m_cLightColor;
  float   m_fLightAngleCos;
  float   m_fLightFallOffCos;
  float   m_fLightStartRangeSQ;
  float   m_fLightEndRangeSQ;
  bool    m_bLightShadowEnabled;
  bool    m_bLightDynamicOnly;

  //bool m_bInverseProjectionUpdated, m_bInverseViewUpdated, m_bInverseWorldUpdated;
  

  //Mat44f m_mInverseProjectionMatrix, m_mInverseViewMatrix, m_mInverseWorldMatrix;
  
};

#endif
