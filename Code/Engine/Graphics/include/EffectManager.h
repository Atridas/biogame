#pragma once
#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "base.h"
#include "Effect.h"
#include "EffectTechnique.h"
#include "Utils/MapManager.h"

//forward declarations ----------------------------------------------
//class CEffect;
//class CEffectTechnique;
//-------------------------------------------------------------------

class CEffectManager:
  public CMapManager<CEffectTechnique>
{
private:
  typedef map<int,string> TDefaultTechniqueEffectMap;

public:
  CEffectManager(): m_vCameraEye(0.0f),
                    m_pStaticMeshTechnique(0),
                    m_pAnimatedModelTechnique(0)
                {};
  ~CEffectManager() {Done();};
  
  void SetWorldMatrix(const Mat44f& _mMatrix) { m_mWorldMatrix = _mMatrix; };
  void SetProjectionMatrix(const Mat44f& _mMatrix) { m_mProjectionMatrix = _mMatrix; };
  void SetViewMatrix(const Mat44f& _mMatrix) { m_mViewMatrix = _mMatrix; };
  void SetViewProjectionMatrix(const Mat44f& _mMatrix) { m_mViewProjectionMatrix = _mMatrix; };
  void SetLightViewMatrix(const Mat44f& _mMatrix) { m_mLightViewMatrix = _mMatrix; };
  void SetShadowProjectionMatrix(const Mat44f& _mMatrix) { m_mShadowProjectionMatrix = _mMatrix; };
  void SetCameraEye(const Vect3f& _vCameraEye) { m_vCameraEye = _vCameraEye; };
  void SetAnimatedModelTechnique(CEffectTechnique* _pAnimatedModelTechnique) { m_pAnimatedModelTechnique = _pAnimatedModelTechnique; };
  void SetStaticMeshTechnique(CEffectTechnique* _pStaticMeshTechnique) { m_pStaticMeshTechnique = _pStaticMeshTechnique; } ;

  const Mat44f& GetWorldMatrix() const {return m_mWorldMatrix;};
  const Mat44f& GetProjectionMatrix() const {return m_mProjectionMatrix;};
  const Mat44f& GetViewMatrix() const {return m_mViewMatrix;};
  const Mat44f& GetViewProjectionMatrix() {return m_mViewProjectionMatrix;};
  const Mat44f& GetLightViewMatrix() const {return m_mLightViewMatrix;};
  const Mat44f& GetShadowProjectionMatrix() {return m_mShadowProjectionMatrix;};
  const Vect3f& GetCameraEye() {return m_vCameraEye;};
  size_t GetMaxLights() const { return MAX_LIGHTS_BY_SHADER; }; //de Effect.h
  string GetTechniqueEffectNameByVertexDefault(unsigned short _sVertexType);
  CEffectTechnique * GetStaticMeshTechnique() const { return m_pStaticMeshTechnique; };
  CEffectTechnique * GetAnimatedModelTechnique() const { m_pAnimatedModelTechnique; };
  CEffectTechnique * GetEffectTechnique(const string& _szName);
  CEffect * GetEffect(const string& _szName);
  
  void ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye);
  bool Load(const string& _szFileName);
  void Reload();
  
  
  void CleanUp() { Release(); };

protected:
  void Release();

private:
  TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
  Mat44f m_mWorldMatrix, m_mProjectionMatrix, m_mViewMatrix, m_mViewProjectionMatrix;
  Mat44f m_mLightViewMatrix, m_mShadowProjectionMatrix;
  Vect3f m_vCameraEye;
  CMapManager<CEffect> m_Effects;
  CEffectTechnique* m_pStaticMeshTechnique;
  CEffectTechnique* m_pAnimatedModelTechnique;
};

#endif
