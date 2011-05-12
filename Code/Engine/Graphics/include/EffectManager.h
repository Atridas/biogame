#pragma once
#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "base.h"
#include "Effect.h"
#include "EffectTechnique.h"
#include "Utils/MapManager.h"
#include "params.h"


#define MAX_LIGHTS_BY_SHADER 4

//forward declarations ----------------------------------------------
class CMaterial;
class CLightManager;
class CalSkeleton;
class CalHardwareModel;
//-------------------------------------------------------------------

class CEffectManager:
  public CMapManager<CEffect>
{
private:
  typedef map<int,string> TDefaultEffectMap;

public:
  CEffectManager(): m_pLightManager(0),
                    m_pEffectPool(0),
                    m_pForcedStaticMeshEffect(0),
                    m_pForcedAnimatedModelEffect(0),
                    m_szFileName(""),
                    m_vCameraEye(0.0f),
                    m_pCalSkeleton(0),
                    m_pCalHardwareModel(0),
                    m_bWorldMatrixUpdated(false),
                    m_bProjectionMatrixUpdated(false),
                    m_bViewMatrixUpdated(false),
                    m_bLightViewMatrixUpdated(false), 
                    m_bShadowProjectionMatrixUpdated(false),
                    m_bViewProjectionMatrixUpdated(false),
                    m_bWorldViewMatrixUpdated(false),
                    m_bWorldViewProjectionMatrixUpdated(false),
                    m_bCameraEyeUpdated(false),
                    m_bViewProjectionUpdated(false),
                    m_bWorldViewUpdated(false),
                    m_bWorldViewProjectionUpdated(false),
                    m_bSemanticsUpdated(false),
                    m_bLightsUpdated(false),
                    m_bSkeletonUpdated(false),
                    m_pWorldMatrixParameter(0),
                    m_pViewMatrixParameter(0),
                    m_pProjectionMatrixParameter(0),
                    m_pWorldViewMatrixParameter(0),
                    m_pViewProjectionMatrixParameter(0),
                    m_pWorldViewProjectionMatrixParameter(0),
                    m_pViewToLightProjectionMatrixParameter(0),
                    m_pCameraPositionParameter(0),
                    m_pAmbientLight(0),
                    m_pLightsEnabledParameter(0),
                    m_pLightsTypeParameter(0),
                    m_pLightsPositionParameter(0),
                    m_pLightsDirectionParameter(0),
                    m_pLightsAngleParameter(0),
                    m_pLightsColorParameter(0),
                    m_pLightsFallOffParameter(0),
                    m_pLightsStartRangeAttenuationParameter(0),
                    m_pLightsEndRangeAttenuationParameter(0),
                    m_pShadowsEnabledParameter(0),
                    m_pBonesParameter(0)
                    {SetOk(true);};

  ~CEffectManager() {Done();};
  
  void SetWorldMatrix(const Mat44f& _mMatrix) { m_mWorldMatrix = _mMatrix; m_bWorldMatrixUpdated=m_bWorldViewMatrixUpdated=m_bWorldViewProjectionMatrixUpdated=m_bWorldViewUpdated=m_bWorldViewProjectionUpdated=true;};
  void SetProjectionMatrix(const Mat44f& _mMatrix) { m_mProjectionMatrix = _mMatrix; m_bProjectionMatrixUpdated=m_bViewProjectionMatrixUpdated=m_bWorldViewProjectionMatrixUpdated=m_bViewProjectionUpdated=m_bWorldViewUpdated=m_bWorldViewProjectionUpdated=true;};
  void SetViewMatrix(const Mat44f& _mMatrix) { m_mViewMatrix = _mMatrix; m_bViewMatrixUpdated=m_bViewProjectionMatrixUpdated=m_bWorldViewMatrixUpdated=m_bWorldViewProjectionMatrixUpdated=m_bViewProjectionUpdated=m_bWorldViewProjectionUpdated=true;};
  void SetLightViewMatrix(const Mat44f& _mMatrix) { m_mLightViewMatrix = _mMatrix; m_bLightViewMatrixUpdated=true; };
  void SetShadowProjectionMatrix(const Mat44f& _mMatrix) { m_mShadowProjectionMatrix = _mMatrix; m_bShadowProjectionMatrixUpdated=true; };
  
  void SetCameraEye(const Vect3f& _vCameraEye) { m_vCameraEye = _vCameraEye; m_bCameraEyeUpdated=true; };

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

  //string GetTechniqueEffectNameByVertexDefault(unsigned short _sVertexType) const;
  //string GetInstancedTechniqueEffectNameByVertexDefault(unsigned short _sVertexType) const;

  CEffect* GetForcedStaticMeshEffect() const { return m_pForcedStaticMeshEffect; };
  CEffect* GetForcedAnimatedModelEffect() const { return m_pForcedAnimatedModelEffect; };
  void SetForcedAnimatedModelEffect(CEffect* _pForcedAnimatedModelEffect) { m_pForcedAnimatedModelEffect = _pForcedAnimatedModelEffect; };
  void SetForcedStaticMeshEffect(CEffect* _pForcedStaticMeshEffect) { m_pForcedStaticMeshEffect = _pForcedStaticMeshEffect; } ;
  
  CEffect* GetEffect(const string& _szName) {return GetResource(_szName);};
  
  CEffect* ActivateMaterial(CMaterial* _pMaterial);

  void SetSkeleton(CalSkeleton* _pSkeleton, CalHardwareModel* _pCalHardwareModel) {m_pCalSkeleton = _pSkeleton; m_pCalHardwareModel = _pCalHardwareModel; m_bSkeletonUpdated = true;};

  void ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye);

  void Begin(void) {m_bLightsUpdated = true;};

  bool Load(const SEffectManagerParams& _params);
  void Reload();
  void CleanUp() { Release(); };

protected:
  void Release();

private:

  void LoadShaderData(CEffect* _pEffect);
  bool Load(bool _bReload);

  CLightManager* m_pLightManager;
  LPD3DXEFFECTPOOL m_pEffectPool;

  CEffect* m_pForcedStaticMeshEffect;
  CEffect* m_pForcedAnimatedModelEffect;

  CalSkeleton* m_pCalSkeleton;
  CalHardwareModel* m_pCalHardwareModel;

  string m_szFileName;
  
  TDefaultEffectMap m_DefaultEffectMap;

  Mat44f m_mWorldMatrix;
  Mat44f m_mProjectionMatrix;
  Mat44f m_mViewMatrix;
  Mat44f m_mLightViewMatrix;
  Mat44f m_mShadowProjectionMatrix;
  Mat44f m_mViewProjectionMatrix;
  Mat44f m_mWorldViewMatrix;
  Mat44f m_mWorldViewProjectionMatrix;
  Vect3f m_vCameraEye;
  
  //Variables actualitzadeds al shader
  bool m_bWorldMatrixUpdated;
  bool m_bProjectionMatrixUpdated;
  bool m_bViewMatrixUpdated;
  bool m_bLightViewMatrixUpdated;
  bool m_bShadowProjectionMatrixUpdated;
  bool m_bViewProjectionMatrixUpdated;
  bool m_bWorldViewMatrixUpdated;
  bool m_bWorldViewProjectionMatrixUpdated;
  bool m_bCameraEyeUpdated;
  bool m_bSkeletonUpdated;

  //Matrius compostes recalculades
  bool m_bViewProjectionUpdated;
  bool m_bWorldViewUpdated;
  bool m_bWorldViewProjectionUpdated;

  bool m_bSemanticsUpdated;
  bool m_bLightsUpdated;

  D3DXHANDLE m_pWorldMatrixParameter;
  D3DXHANDLE m_pViewMatrixParameter;
  D3DXHANDLE m_pProjectionMatrixParameter;
  D3DXHANDLE m_pWorldViewMatrixParameter;
  D3DXHANDLE m_pViewProjectionMatrixParameter;
  D3DXHANDLE m_pWorldViewProjectionMatrixParameter;
  D3DXHANDLE m_pViewToLightProjectionMatrixParameter;
  D3DXHANDLE m_pCameraPositionParameter;

  D3DXHANDLE m_pAmbientLight;
  D3DXHANDLE m_pLightsEnabledParameter;
  D3DXHANDLE m_pLightsTypeParameter;
  D3DXHANDLE m_pLightsPositionParameter;
  D3DXHANDLE m_pLightsDirectionParameter;
  D3DXHANDLE m_pLightsAngleParameter;
  D3DXHANDLE m_pLightsColorParameter;
  D3DXHANDLE m_pLightsFallOffParameter;
  D3DXHANDLE m_pLightsStartRangeAttenuationParameter;
  D3DXHANDLE m_pLightsEndRangeAttenuationParameter;
  D3DXHANDLE m_pShadowsEnabledParameter;

  D3DXHANDLE m_pBonesParameter;

  //bool m_bInverseProjectionUpdated, m_bInverseViewUpdated, m_bInverseWorldUpdated;
  

  //Mat44f m_mInverseProjectionMatrix, m_mInverseViewMatrix, m_mInverseWorldMatrix;
  
};

#endif
