#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "base.h"

class CRenderTarget;
class CRendererStep;
class CSceneRendererStep;
//class CPreSceneRendererStep;
class CPostSceneRendererStep;
class CProcess;
class CCamera;
class CObject3DRenderable;

class CRenderer :
  public CBaseControl
{
  struct SRenderPath
  {
    set<string> m_PreSceneRenderSteps;
    set<string> m_PostSceneRenderSteps;
    set<string> m_SceneRenderers;
    bool m_bActive;
  };

public:
  enum EBlendParameters
  {
    EBP_BRIGHT_PASS_THRESHOLD    ,
    EBP_EXPOSURE                 ,
    EBP_GAUSS_MULTIPLIER         ,
    EBP_MAX_LUMINANCE_LIMIT_MIN  ,
    EBP_MAX_LUMINANCE_LIMIT_MAX  ,
    EBP_SCENE_LUMINANCE_LIMIT_MIN,
    EBP_SCENE_LUMINANCE_LIMIT_MAX,
    EBP_BLOOM_FINAL_SCALE        ,
    EBP_GLOW_TO_BLOOM            ,
    EBP_GLOW_LUMINANCE_SCALE     ,
    EBP_MAX_GLOW_LUMINANCE       ,
    EBP_GLOW_FINAL_SCALE         ,
    
    EBP_BLUR_RADIUS              ,
    EBP_NEAR_FOCAL_PLANE_DEPTH   ,
    EBP_FAR_FOCAL_PLANE_DEPTH    ,
    EBP_NEAR_BLUR_DEPTH          ,
    EBP_FAR_BLUR_DEPTH    
  };

  struct SBlendObjective
  {
    float m_fValue, m_fTime;
    SBlendObjective():m_fValue(0), m_fTime(5000000000.f){}
    SBlendObjective(float _fValue, float _fTime):m_fValue(_fValue), m_fTime(_fTime){}
  };

public:

  CRenderer() : m_szFileName(""), m_bRenderPathsChanged(false),m_pActiveCamera(0) {};
  ~CRenderer() {Done();};

  bool Init(const string& _szFileName);
  
  CPostSceneRendererStep* GetPostSceneRendererStep(string _szName);
  
  void ActivateRenderPath  (const string& _szRenderPath);
  void DeactivateRenderPath(const string& _szRenderPath);
  void SetUniqueRenderPath(const string&  _szRenderPath);
  void GetActiveRenderPaths(set<string>& _RenderPaths) const;
  
  void     SetActiveCamera(CCamera * _pActiveCamera) { m_pActiveCamera = _pActiveCamera; };
  CCamera* GetActiveCamera() const                   { return m_pActiveCamera; };

  void Render(CProcess* _pProcess);
  void Update(float _fDeltaTime);

  CRenderTarget* GetRenderTarget(const string& _szRenderTarget) const 
  { 
    map<string,CRenderTarget*>::const_iterator l_it = m_mapRenderTargets.find(_szRenderTarget);
    if(l_it != m_mapRenderTargets.end())
    {
      return l_it->second;
    }
    else
    {
      return 0;
    }
  }

  void BlendParameter(EBlendParameters _Parameter, float _fValue, float _fTime)
  { m_BlendValueTime[_Parameter] = SBlendObjective(_fValue, _fTime); };


protected:
  virtual void Release();
private:

  
  void InitRenderVectors(CCamera* _pCamera,
                         vector<CObject3DRenderable*>& _vOpaqueObjects, 
                         vector<CObject3DRenderable*>& _vAlphaObjects, 
                         vector<CObject3DRenderable*>& _vParticleEmiters);

  void ActivateRenderPaths();

  string m_szFileName;

  CCamera* m_pCamera, *m_pActiveCamera;

  map<string,CRenderTarget*> m_mapRenderTargets;
  string m_szDefaultRenderTarget;
  
  map<string,CSceneRendererStep*>     m_mapSceneRendererSteps;
  map<string,CPostSceneRendererStep*> m_mapPostSceneRendererSteps;

  vector<CSceneRendererStep*>     m_vSceneRendererSteps;
  vector<CPostSceneRendererStep*> m_vPostSceneRendererSteps;

  map<string, SRenderPath*> m_mapRenderPaths;
  bool m_bRenderPathsChanged;

  map<EBlendParameters, SBlendObjective> m_BlendValueTime;
};

#endif

