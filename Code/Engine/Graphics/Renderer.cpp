#include "Renderer.h"
#include "RendererStep.h"
#include "XML/XMLTreeNode.h"
#include "RenderTarget.h"
#include "TextureRenderTarget.h"
#include "BackBufferRenderTarget.h"
#include "MultipleRenderTarget.h"
#include "SceneRendererStep.h"
#include "PostSceneRendererStep.h"
#include "DeferredPostSceneRendererStep.h"
#include "ShadowMapPreRendererStep.h"
#include "Core.h"
#include "RenderManager.h"
#include "Process.h"
#include "EffectManager.h"
#include "Camera.h"
#include "PortalManager.h"
#include "RenderObjectsPostSceneRendererStep.h"

bool CRenderer::Init(const string& _szFileName)
{
  m_szFileName = _szFileName;

  CXMLTreeNode l_treeRenderer;

  if(!l_treeRenderer.LoadFile(_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init Error al carregar el fitxer XML %s",_szFileName.c_str());
    SetOk(false);
  } 
  else 
  {
  
    LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init inicialitzant fitxer XML %s",_szFileName.c_str());

    m_szDefaultRenderTarget = l_treeRenderer.GetPszISOProperty("default", "backbuffer", true);

    CXMLTreeNode l_treeRenderTargets = l_treeRenderer.GetChild("render_targets");
    CXMLTreeNode l_treeRenderers     = l_treeRenderer.GetChild("scene_renderers");
    CXMLTreeNode l_treePostRenderers = l_treeRenderer.GetChild("post_scene_renderers");
    CXMLTreeNode l_treeRenderPaths   = l_treeRenderer.GetChild("render_paths");

    map<string,CTextureRenderTarget*> l_mapTextureRenderTargets;

    if(!l_treeRenderTargets.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init no hi ha RenderTargets");
    }else{

      CXMLTreeNode l_treeBackbufferRenderTargets = l_treeRenderTargets.GetChild("backbuffer_render_target");

      if(l_treeBackbufferRenderTargets.Exists())
      {
        CBackBufferRenderTarget* l_pBackBufferRT = new CBackBufferRenderTarget();
        if(l_pBackBufferRT->Init(l_treeBackbufferRenderTargets.GetPszISOProperty("name", "backbuffer", true)))
        {
          m_mapRenderTargets[ l_pBackBufferRT->GetName() ] = l_pBackBufferRT;
        }
        else
        {
          LOGGER->AddNewLog(ELL_WARNING, "Error a l'init del backbuffer render target");
          delete l_pBackBufferRT;
        }
      }
      else
      {
        LOGGER->AddNewLog(ELL_WARNING, "No hi ha l'element \"backbuffer_render_target\". Inicialitzant amb valors per defecte");
        CBackBufferRenderTarget* l_pBackBufferRT = new CBackBufferRenderTarget();
        if(l_pBackBufferRT->Init("backbuffer"))
        {
          m_mapRenderTargets[ l_pBackBufferRT->GetName() ] = l_pBackBufferRT;
        }
        else
        {
          LOGGER->AddNewLog(ELL_WARNING, "Error a l'init del backbuffer render target");
          delete l_pBackBufferRT;
        }
      }


      CXMLTreeNode l_treeTextureRenderTargets = l_treeRenderTargets.GetChild("texture_render_targets");

      if(!l_treeTextureRenderTargets.Exists())
      {
        LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init no hi ha texture_render_targets");
      }else{

        int l_iNumChildren = l_treeTextureRenderTargets.GetNumChildren();

        for(int i = 0; i < l_iNumChildren;i++)
        {
          CXMLTreeNode l_treeTextureRenderTarget = l_treeTextureRenderTargets(i);

          if(string(l_treeTextureRenderTarget.GetName()) == "texture_render_target")
          {
            CTextureRenderTarget* l_pTextureRenderTarget = new CTextureRenderTarget();

            if(l_pTextureRenderTarget->Init(l_treeTextureRenderTarget))
            {
              m_mapRenderTargets[l_pTextureRenderTarget->GetName()] = l_pTextureRenderTarget;
              l_mapTextureRenderTargets[l_pTextureRenderTarget->GetName()] = l_pTextureRenderTarget;

            }else{
              LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init error al inicialitzar texture_render_target %s",l_pTextureRenderTarget->GetName().c_str());
              CHECKED_DELETE(l_pTextureRenderTarget);
            }

          }else if(!l_treeTextureRenderTarget.IsComment())
          {
            LOGGER->AddNewLog(ELL_WARNING,"CRenderer::Init element no reconegut %s",l_treeTextureRenderTarget.GetName());
          }
        }
      }


      CXMLTreeNode l_treeMultipleRenderTargets = l_treeRenderTargets.GetChild("multiple_render_targets");

      if(!l_treeMultipleRenderTargets.Exists())
      {
        LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init no hi ha multiple_render_targets");
      }else{

        int l_iNumChildren = l_treeMultipleRenderTargets.GetNumChildren();

        for(int i = 0; i < l_iNumChildren;i++)
        {
          CXMLTreeNode l_treeMultipleRenderTarget = l_treeMultipleRenderTargets(i);

          if(string(l_treeMultipleRenderTarget.GetName()) == "multiple_render_target")
          {
            string l_szName = l_treeMultipleRenderTarget.GetPszISOProperty("name","",false);

            if(l_szName != "")
            {
              CMultipleRenderTarget* l_pMultipleRenderTarget = new CMultipleRenderTarget();

              l_pMultipleRenderTarget->Init(l_szName);

              m_mapRenderTargets[l_pMultipleRenderTarget->GetName()] = l_pMultipleRenderTarget;

              int l_iNumChildren = l_treeMultipleRenderTarget.GetNumChildren();

              for(int i = 0; i < l_iNumChildren;i++)
              {
                CXMLTreeNode l_treeTextureRenderTarget = l_treeMultipleRenderTarget(i);

                if(string(l_treeTextureRenderTarget.GetName()) == "texture_render_target")
                {
                  string l_szTRTName = l_treeTextureRenderTarget.GetPszISOProperty("name","",false);
                  int l_iIndex = l_treeTextureRenderTarget.GetIntProperty("index",-1,false);

                  if(l_szTRTName != "")
                  {
                    CTextureRenderTarget* l_pTextureRenderTarget = 0;
                    
                    map<string,CTextureRenderTarget*>::iterator l_itTRT = l_mapTextureRenderTargets.find(l_szTRTName);

                    if(l_itTRT != l_mapTextureRenderTargets.end())
                    {
                      l_pMultipleRenderTarget->AddTextureRenderTarget((*l_itTRT).second,l_iIndex);
                    }else{
                      LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init multiple_render_target: texture_render_target %s no trobat",l_szTRTName.c_str());
                    }

                  }else{
                    LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init multiple_render_target: texture_render_target sense nom");
                  }

                }else if(!l_treeTextureRenderTarget.IsComment())
                {
                  LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init element no reconegut %s",l_treeTextureRenderTarget.GetName());
                }
              }

            }else{
              LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init multiple_render_target sense nom");
            }

          }else if(!l_treeMultipleRenderTargets.IsComment())
          {
            LOGGER->AddNewLog(ELL_WARNING,"CRenderer::Init element no reconegut %s",l_treeMultipleRenderTargets.GetName());
          }
        }
      }
    }

    if(!l_treeRenderers.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init no hi ha SeceneRenderers");
    }else{

      int l_iNumChildren = l_treeRenderers.GetNumChildren();
      string l_szDefaultRenderer = l_treeRenderers.GetPszISOProperty("default_renderer","",false);

      for(int i = 0; i < l_iNumChildren;i++)
      {
        CXMLTreeNode l_treeRenderer = l_treeRenderers(i);

        if(string(l_treeRenderer.GetName()) == "scene_renderer")
        {
          CSceneRendererStep* l_pRenderer = new CSceneRendererStep();

          if(!l_pRenderer->Init(l_treeRenderer, m_szDefaultRenderTarget))
          {
            CHECKED_DELETE(l_pRenderer);
            LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init error inicialitzant SceneRenderer");
          }else{

            m_vSceneRendererSteps.push_back(l_pRenderer);
            m_mapSceneRendererSteps[l_pRenderer->GetName()] = l_pRenderer;
          }
        }
        else if(!l_treeRenderer.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING,"CRenderer::Init element no reconegut %s",l_treeRenderer.GetName());
        }

      }

    }

    if(!l_treePostRenderers.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init no hi ha PostSeceneRenderers");
    }else{

      int l_iNumChildren = l_treePostRenderers.GetNumChildren();

      for(int i = 0; i < l_iNumChildren;i++)
      {
        CXMLTreeNode l_treePostRenderer = l_treePostRenderers(i);

        if(string(l_treePostRenderer.GetName()) == "post_scene_renderer")
        {
          CPostSceneRendererStep* l_pPostRenderer = 0;

          string l_szType = l_treePostRenderer.GetPszISOProperty("type","post_scene_renderer",false);

          if(l_szType == "deferred_post_scene_renderer")
          {
            l_pPostRenderer = new CDeferredPostSceneRendererStep();
          } 
          else if(l_szType == "render_objects_post_scene_renderer")
          {
            l_pPostRenderer = new CRenderObjectsPostSceneRendererStep();
          } 
          else 
          {
            l_pPostRenderer = new CPostSceneRendererStep();
          }

          if(!l_pPostRenderer->Init(l_treePostRenderer, m_szDefaultRenderTarget))
          {
            CHECKED_DELETE(l_pPostRenderer);
            LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init error inicialitzant PostSceneRenderer");
          }else{
            m_vPostSceneRendererSteps.push_back(l_pPostRenderer);
            m_mapPostSceneRendererSteps[l_pPostRenderer->GetName()] = l_pPostRenderer;
          }
        }
        else if(!l_treePostRenderer.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING,"CRenderer::Init element no reconegut %s",l_treePostRenderer.GetName());
        }

      }

    }

    if(l_treeRenderPaths.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "CRenderer::Init Llegint render_paths");
      int l_iNumChilds = l_treeRenderPaths.GetNumChildren();
      for(int i = 0; i < l_iNumChilds; ++i)
      {
        CXMLTreeNode l_treeRenderPath = l_treeRenderPaths(i);
        if(strcmp(l_treeRenderPath.GetName(), "render_path") == 0)
        {
          SRenderPath *l_pRenderPath = new SRenderPath;
          string l_szName = l_treeRenderPath.GetPszISOProperty("name");
          l_pRenderPath->m_bActive = l_treeRenderPath.GetBoolProperty("active", false, false);

          int l_iNumChilds = l_treeRenderPath.GetNumChildren();
          for(int i = 0; i < l_iNumChilds; ++i)
          {
            CXMLTreeNode l_treeRenderer = l_treeRenderPath(i);
            if(strcmp(l_treeRenderer.GetName(), "pre_scene_renderer") == 0)
            {
              l_pRenderPath->m_PreSceneRenderSteps.insert( l_treeRenderer.GetPszISOProperty("name") );
            }
            else if(strcmp(l_treeRenderer.GetName(), "post_scene_renderer") == 0)
            {
              l_pRenderPath->m_PostSceneRenderSteps.insert( l_treeRenderer.GetPszISOProperty("name") );
            }
            else if(strcmp(l_treeRenderer.GetName(), "scene_renderer") == 0)
            {
              l_pRenderPath->m_SceneRenderers.insert( l_treeRenderer.GetPszISOProperty("name") );
            }
            else if(!l_treeRenderer.IsComment())
            {
              LOGGER->AddNewLog(ELL_WARNING, "CRenderer::Init element no reconegut %s", l_treeRenderer.GetName());
            }
          }

          if(m_mapRenderPaths.find(l_szName) != m_mapRenderPaths.end())
          {
            LOGGER->AddNewLog(ELL_WARNING, "CRenderer::Init No repetiu els noms dels renderpaths, collons!!!");
            delete m_mapRenderPaths[l_szName];
          }
          m_mapRenderPaths[l_szName] = l_pRenderPath;
        }
        else if(!l_treeRenderPath.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING, "CRenderer::Init element no reconegut %s", l_treeRenderPath.GetName());
        }
      }
      ActivateRenderPaths();
    }
    
    SetOk(true);
  }

  return IsOk();
}

void CRenderer::Update(float _fDeltaTime)
{
  for(uint32 i = 0; i < m_vPostSceneRendererSteps.size(); ++i)
  {
    m_vPostSceneRendererSteps[i]->Update(_fDeltaTime);
  }
  for(uint32 i = 0; i < m_vSceneRendererSteps.size(); ++i)
  {
    m_vSceneRendererSteps[i]->Update(_fDeltaTime);
  }

  CEffectManager* l_pEM = CORE->GetEffectManager();

  set<EBlendParameters> l_ParametersToErase;

  {
    map<EBlendParameters, SBlendObjective>::iterator l_it  = m_BlendValueTime.begin();
    map<EBlendParameters, SBlendObjective>::iterator l_end = m_BlendValueTime.end();
    for(;l_it != l_end; ++l_it)
    {
      float l_fPrevValue, l_fAux;
      switch(l_it->first)
      {
      case EBP_BRIGHT_PASS_THRESHOLD:
        l_fPrevValue = l_pEM->GetBrightPassThreshold();
        break;
      case EBP_EXPOSURE:
        l_fPrevValue = l_pEM->GetExposure();
        break;
      case EBP_GAUSS_MULTIPLIER:
        l_fPrevValue = l_pEM->GetGaussMultiplier();
        break;
      case EBP_BLOOM_FINAL_SCALE:
        l_fPrevValue = l_pEM->GetBloomFinalScale();
        break;
      case EBP_GLOW_TO_BLOOM:
        l_fPrevValue = l_pEM->GetGlowToBloom();
        break;
      case EBP_GLOW_LUMINANCE_SCALE:
        l_fPrevValue = l_pEM->GetGlowLuminanceScale();
        break;
      case EBP_MAX_GLOW_LUMINANCE:
        l_fPrevValue = l_pEM->GetMaxGlowLuminance();
        break;
      case EBP_GLOW_FINAL_SCALE:
        l_fPrevValue = l_pEM->GetGlowFinalScale();
        break;
      case EBP_MAX_LUMINANCE_LIMIT_MIN:
        l_pEM->GetMaxLuminanceLimits(l_fPrevValue, l_fAux);
        break;
      case EBP_MAX_LUMINANCE_LIMIT_MAX:
        l_pEM->GetMaxLuminanceLimits(l_fAux, l_fPrevValue);
        break;
      case EBP_SCENE_LUMINANCE_LIMIT_MIN:
        l_pEM->GetSceneLuminanceLimits(l_fPrevValue, l_fAux);
        break;
      case EBP_SCENE_LUMINANCE_LIMIT_MAX:
        l_pEM->GetSceneLuminanceLimits(l_fAux, l_fPrevValue);
        break;
        
      case EBP_BLUR_RADIUS:
        l_fPrevValue = l_pEM->GetBlurRadius();
        break;
      case EBP_NEAR_FOCAL_PLANE_DEPTH:
        l_fPrevValue = l_pEM->GetNearFocalPlaneDepth();
        break;
      case EBP_FAR_FOCAL_PLANE_DEPTH:
        l_fPrevValue = l_pEM->GetFarFocalPlaneDepth();
        break;
      case EBP_NEAR_BLUR_DEPTH:
        l_fPrevValue = l_pEM->GetNearBlurDepth();
        break;
      case EBP_FAR_BLUR_DEPTH:
        l_fPrevValue = l_pEM->GetFarBlurDepth();
        break;
      }


      float l_fValue;
      if(l_it->second.m_fTime <= _fDeltaTime)
      {
        l_fValue = l_it->second.m_fValue;
        l_ParametersToErase.insert(l_it->first);
      }
      else
      {
        l_fValue = InterpolateNumber(l_fPrevValue, l_it->second.m_fValue, l_it->second.m_fTime, _fDeltaTime);
        l_it->second.m_fTime -= _fDeltaTime;
      }

    
      switch(l_it->first)
      {
      case EBP_BRIGHT_PASS_THRESHOLD:
        l_pEM->SetBrightPassThreshold(l_fValue);
        break;
      case EBP_EXPOSURE:
        l_pEM->SetExposure(l_fValue);
        break;
      case EBP_GAUSS_MULTIPLIER:
        l_pEM->SetGaussMultiplier(l_fValue);
        break;
      case EBP_BLOOM_FINAL_SCALE:
        l_pEM->SetBloomFinalScale(l_fValue);
        break;
      case EBP_GLOW_TO_BLOOM:
        l_pEM->SetGlowToBloom(l_fValue);
        break;
      case EBP_GLOW_LUMINANCE_SCALE:
        l_pEM->SetGlowLuminanceScale(l_fValue);
        break;
      case EBP_MAX_GLOW_LUMINANCE:
        l_pEM->SetMaxGlowLuminance(l_fValue);
        break;
      case EBP_GLOW_FINAL_SCALE:
        l_pEM->SetGlowFinalScale(l_fValue);
        break;
      case EBP_MAX_LUMINANCE_LIMIT_MIN:
        l_pEM->SetMaxLuminanceLimits(l_fValue, l_fAux);
        break;
      case EBP_MAX_LUMINANCE_LIMIT_MAX:
        l_pEM->SetMaxLuminanceLimits(l_fAux, l_fValue);
        break;
      case EBP_SCENE_LUMINANCE_LIMIT_MIN:
        l_pEM->SetSceneLuminanceLimits(l_fValue, l_fAux);
        break;
      case EBP_SCENE_LUMINANCE_LIMIT_MAX:
        l_pEM->SetSceneLuminanceLimits(l_fAux, l_fValue);
        break;
        
      case EBP_BLUR_RADIUS:
        l_pEM->SetBlurRadius(l_fValue);
        break;
      case EBP_NEAR_FOCAL_PLANE_DEPTH:
        l_pEM->SetNearFocalPlaneDepth(l_fValue);
        break;
      case EBP_FAR_FOCAL_PLANE_DEPTH:
        l_pEM->SetFarFocalPlaneDepth(l_fValue);
        break;
      case EBP_NEAR_BLUR_DEPTH:
        l_pEM->SetNearBlurDepth(l_fValue);
        break;
      case EBP_FAR_BLUR_DEPTH:
        l_pEM->SetFarBlurDepth(l_fValue);
        break;
      }
    }
  }
  
  set<EBlendParameters>::iterator l_it  = l_ParametersToErase.begin();
  set<EBlendParameters>::iterator l_end = l_ParametersToErase.end();

  for(;l_it != l_end; ++l_it)
  {
    m_BlendValueTime.erase(*l_it);
  }
}

void CRenderer::Render(CProcess* _pProcess)
{
  if(m_bRenderPathsChanged)
  {
    ActivateRenderPaths();
  }

  CRenderManager* l_pRM = RENDER_MANAGER;
  CEffectManager* l_pEM = CORE->GetEffectManager();

  if(m_pActiveCamera)
  {
    m_pCamera = m_pActiveCamera;
  } else {
    m_pCamera = _pProcess->GetCamera();
  }

  l_pRM->BeginRendering();

  vector<CObject3DRenderable*> l_vOpaqueObjects, l_vAlphaObjects, l_vParticleEmiters;


  vector<CSceneRendererStep*>::iterator l_itRenderer = m_vSceneRendererSteps.begin();
  vector<CSceneRendererStep*>::iterator l_itRendererEnd = m_vSceneRendererSteps.end();

  bool l_bVectorsInitiated = false;
  for(;l_itRenderer != l_itRendererEnd; ++l_itRenderer)
  {
    if((*l_itRenderer)->IsActive())
    {
      if(!l_bVectorsInitiated)
      {
        InitRenderVectors(m_pCamera, l_vOpaqueObjects, l_vAlphaObjects, l_vParticleEmiters);
        l_bVectorsInitiated = true;
      }

      string l_szRenderTarget = (*l_itRenderer)->GetRenderTarget();
      map<string,CRenderTarget*>::const_iterator l_it = m_mapRenderTargets.find(l_szRenderTarget);
      if(l_it != m_mapRenderTargets.end())
      {
        l_it->second->Activate(l_pRM);
        l_pEM->SetTextureWidthHeight(l_it->second->GetWidth(), l_it->second->GetHeight());
      }
    
    
      (*l_itRenderer)->ClearBuffer(l_pRM);
      (*l_itRenderer)->Render(l_pRM, m_pCamera, l_vOpaqueObjects, l_vAlphaObjects, l_vParticleEmiters);

      if(l_it != m_mapRenderTargets.end())
      {
        l_it->second->Deactivate(l_pRM);
      }
    }
  }
  

  vector<CPostSceneRendererStep*>::iterator l_itPostRenderer = m_vPostSceneRendererSteps.begin();
  vector<CPostSceneRendererStep*>::iterator l_itPostRendererEnd = m_vPostSceneRendererSteps.end();

  for(;l_itPostRenderer != l_itPostRendererEnd; ++l_itPostRenderer)
  {
    CPostSceneRendererStep* l_pPostSceneRenderer = (*l_itPostRenderer);

    if(l_pPostSceneRenderer->IsActive())
    {
      map<string,CRenderTarget*>::const_iterator l_it;

      if(l_pPostSceneRenderer->NeedsToActivateRenderTargets())
      {
        string l_szRenderTarget = l_pPostSceneRenderer->GetRenderTarget();
        l_it = m_mapRenderTargets.find(l_szRenderTarget);
        if(l_it != m_mapRenderTargets.end())
        {
          l_it->second->Activate(l_pRM);
          l_pEM->SetTextureWidthHeight(l_it->second->GetWidth(), l_it->second->GetHeight());
        }
      }
      
      l_pPostSceneRenderer->ClearBuffer(l_pRM);
      l_pPostSceneRenderer->Render(l_pRM, m_pCamera, l_vOpaqueObjects, l_vAlphaObjects, l_vParticleEmiters);
      
      if(l_pPostSceneRenderer->NeedsToActivateRenderTargets())
      {
        if(l_it != m_mapRenderTargets.end())
        {
          l_it->second->Deactivate(l_pRM);
        }
      }
    }
  }

  _pProcess->DebugInformation(l_pRM);

  l_pRM->EndRendering();

  l_pRM->Present();
}

void CRenderer::Release()
{
  map<string,CRenderTarget*>::iterator l_itMap = m_mapRenderTargets.begin();
  map<string,CRenderTarget*>::iterator l_itMapEnd = m_mapRenderTargets.end();

  for(;l_itMap != l_itMapEnd; ++l_itMap)
  {
    CHECKED_DELETE((*l_itMap).second);
  }

  m_mapRenderTargets.clear();

  vector<CSceneRendererStep*>::iterator l_itRenderer = m_vSceneRendererSteps.begin();
  vector<CSceneRendererStep*>::iterator l_itRendererEnd = m_vSceneRendererSteps.end();

  for(;l_itRenderer != l_itRendererEnd; ++l_itRenderer)
  {
    CHECKED_DELETE(*l_itRenderer);
  }

  m_vSceneRendererSteps.clear();
  m_mapSceneRendererSteps.clear();

  vector<CPostSceneRendererStep*>::iterator l_itPostRenderer = m_vPostSceneRendererSteps.begin();
  vector<CPostSceneRendererStep*>::iterator l_itPostRendererEnd = m_vPostSceneRendererSteps.end();

  for(;l_itPostRenderer != l_itPostRendererEnd; ++l_itPostRenderer)
  {
    CHECKED_DELETE(*l_itPostRenderer);
  }

  m_vPostSceneRendererSteps.clear();
  m_mapPostSceneRendererSteps.clear();

  map<string, SRenderPath*>::iterator l_itRendererPaths    = m_mapRenderPaths.begin();
  map<string, SRenderPath*>::iterator l_itRendererPathsEnd = m_mapRenderPaths.end();

  for(;l_itRendererPaths != l_itRendererPathsEnd; ++l_itRendererPaths)
  {
    CHECKED_DELETE(l_itRendererPaths->second);
  }

  m_mapRenderPaths.clear();
}

CPostSceneRendererStep* CRenderer::GetPostSceneRendererStep(string _szName)
{
  map<string,CPostSceneRendererStep*>::const_iterator l_It = m_mapPostSceneRendererSteps.find(_szName);
  if(l_It == m_mapPostSceneRendererSteps.end()) 
  {
    return 0;
  }
  return l_It->second;
}

void CRenderer::ActivateRenderPath  (const string& _szRenderPath)
{
  map<string, SRenderPath*>::iterator l_it = m_mapRenderPaths.find(_szRenderPath);
  if(l_it != m_mapRenderPaths.end() && !l_it->second->m_bActive )
  {
    l_it->second->m_bActive = true;
    m_bRenderPathsChanged  = true;
  }
}

void CRenderer::DeactivateRenderPath(const string& _szRenderPath)
{
  map<string, SRenderPath*>::iterator l_it = m_mapRenderPaths.find(_szRenderPath);
  if(l_it != m_mapRenderPaths.end() && l_it->second->m_bActive )
  {
    l_it->second->m_bActive = false;
    m_bRenderPathsChanged  = true;
  }
}

void CRenderer::SetUniqueRenderPath(const string& _szRenderPath)
{
  map<string, SRenderPath*>::iterator l_it = m_mapRenderPaths.find(_szRenderPath);
  map<string, SRenderPath*>::iterator l_end   = m_mapRenderPaths.end();
  if( l_it != l_end )
  {
    map<string, SRenderPath*>::iterator l_begin = m_mapRenderPaths.begin();
    for(; l_begin != l_end; ++l_begin)
    {
      if(l_begin == l_it)
      {
        l_begin->second->m_bActive = true;
      }
      else
      {
        l_begin->second->m_bActive = false;
      }
    }
    m_bRenderPathsChanged  = true;
  }
}

void CRenderer::ActivateRenderPaths()
{
  //desactivar els steps
  for(uint32 i = 0; i < m_vPostSceneRendererSteps.size(); ++i)
  {
    m_vPostSceneRendererSteps[i]->SetActive(false);
  }
  for(uint32 i = 0; i < m_vSceneRendererSteps.size(); ++i)
  {
    m_vSceneRendererSteps[i]->SetActive(false);
  }

  map<string, SRenderPath*>::iterator l_it  = m_mapRenderPaths.begin();
  map<string, SRenderPath*>::iterator l_end = m_mapRenderPaths.end();
  for(; l_it != l_end; ++l_it)
  {
    if(l_it->second->m_bActive)
    {
      set<string>::iterator l_it2  = l_it->second->m_PostSceneRenderSteps.begin();
      set<string>::iterator l_end2 = l_it->second->m_PostSceneRenderSteps.end  ();
      for(; l_it2 != l_end2; ++l_it2)
      {
        map<string,CPostSceneRendererStep*>::iterator l_it = m_mapPostSceneRendererSteps.find(*l_it2);
        if(l_it != m_mapPostSceneRendererSteps.end())
        {
          l_it->second->SetActive(true);
        }
      }

      l_it2  = l_it->second->m_SceneRenderers.begin();
      l_end2 = l_it->second->m_SceneRenderers.end  ();
      for(; l_it2 != l_end2; ++l_it2)
      {
        map<string,CSceneRendererStep*>::iterator l_it = m_mapSceneRendererSteps.find(*l_it2);
        if(l_it != m_mapSceneRendererSteps.end())
        {
          l_it->second->SetActive(true);
        }
      }
    }
  }

  m_bRenderPathsChanged = false;
}


void CRenderer::GetActiveRenderPaths(set<string>& _RenderPaths) const
{
  _RenderPaths.clear();

  map<string, SRenderPath*>::const_iterator l_it  = m_mapRenderPaths.begin();
  map<string, SRenderPath*>::const_iterator l_end = m_mapRenderPaths.end();
  for(; l_it != l_end; ++l_it)
  {
    if(l_it->second->m_bActive)
    {
      _RenderPaths.insert(l_it->first);
    }
  }
}

void CRenderer::InitRenderVectors(CCamera* _pCamera,
                                  vector<CObject3DRenderable*>& _vOpaqueObjects, 
                                  vector<CObject3DRenderable*>& _vAlphaObjects, 
                                  vector<CObject3DRenderable*>& _vParticleEmiters)
{
  CPortalManager* l_pPM = CORE->GetPortalManager();

  Vect3f   l_vEye    = _pCamera->GetEye();
  CObject3DOrdering l_Ordering(l_vEye);
  
  CRoom::TBlendQueue l_BlendQueue(l_Ordering);
  CRoom::TBlendQueue l_EmiterQueue(l_Ordering);

  vector<CRenderableObject*> l_vOpaqueObjects;
  l_pPM->GetRenderedObjects(_pCamera, l_vOpaqueObjects, l_BlendQueue, l_EmiterQueue);


  vector<CRenderableObject*>::iterator l_it  = l_vOpaqueObjects.begin();
  vector<CRenderableObject*>::iterator l_end = l_vOpaqueObjects.end  ();

  for(; l_it != l_end; ++l_it)
  {
    CRenderableObject* l_pRO = *l_it;
    _vOpaqueObjects.push_back(l_pRO);
  }
  
  CORE->GetEffectManager()->ActivateAlphaRendering();

  while(!l_BlendQueue.empty())
  {
    CObject3DRenderable* l_pO3DRenderable = l_BlendQueue.top();
    _vAlphaObjects.push_back(l_pO3DRenderable);
    l_BlendQueue.pop();
  }
  
  while(!l_EmiterQueue.empty())
  {
    CObject3DRenderable* l_pEmiter = l_EmiterQueue.top();
    _vParticleEmiters.push_back(l_pEmiter);
    l_EmiterQueue.pop();
  }
}