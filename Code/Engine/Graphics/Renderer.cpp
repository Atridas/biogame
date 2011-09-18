#include "Renderer.h"
#include "RendererStep.h"
#include "XML/XMLTreeNode.h"
#include "RenderTarget.h"
#include "TextureRenderTarget.h"
#include "BackBufferRenderTarget.h"
#include "MultipleRenderTarget.h"
#include "PreSceneRendererStep.h"
#include "SceneRendererStep.h"
#include "PostSceneRendererStep.h"
#include "DeferredPostSceneRendererStep.h"
#include "ShadowMapPreRendererStep.h"
#include "Core.h"
#include "RenderManager.h"
#include "Process.h"
#include "EffectManager.h"
#include "Camera.h"

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
    CXMLTreeNode l_treePreRenderers  = l_treeRenderer.GetChild("pre_scene_renderers");
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

    if(!l_treePreRenderers.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init no hi ha PreSeceneRenderers");
    }else{

      int l_iNumChildren = l_treePreRenderers.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init inicialitzant %d PreSceneRenderers",l_iNumChildren);

      for(int i = 0; i < l_iNumChildren;i++)
      {
        CXMLTreeNode l_treePreRenderer = l_treePreRenderers(i);

        if(string(l_treePreRenderer.GetName()) == "pre_scene_renderer")
        {
          CPreSceneRendererStep* l_pPreRenderer = 0;

          if(string(l_treePreRenderer.GetPszProperty("type","",false)) == "shadow_map_scene_renderer")
          {
            l_pPreRenderer = new CShadowMapPreRendererStep();
          }else if(string(l_treePreRenderer.GetPszProperty("type","",false)) == "pre_scene_renderer")
          {
            l_pPreRenderer = new CPreSceneRendererStep();
          }else{
            LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init PreRenderer type no reconegut");
          }

          if(l_pPreRenderer)
          {
            if(!l_pPreRenderer->Init(l_treePreRenderer, m_szDefaultRenderTarget))
            {
              CHECKED_DELETE(l_pPreRenderer);
              LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init error inicialitzant PreRenderer");
            }else{
              m_vPreSceneRendererSteps.push_back(l_pPreRenderer);
              m_mapPreSceneRendererSteps[l_pPreRenderer->GetName()] = l_pPreRenderer;
            }
          }
        }
        else if(!l_treePreRenderer.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING,"CRenderer::Init element no reconegut %s",l_treePreRenderer.GetName());
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

        SetSceneRenderer(l_szDefaultRenderer);

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
          }else{
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
      LOGGER->AddNewLog(ELL_WARNING, "CRenderer::Init Llegint render_paths");
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
              l_pRenderPath->m_szSceneRenderer = l_treeRenderer.GetPszISOProperty("name");
            }
            else
            {
              LOGGER->AddNewLog(ELL_WARNING, "CRenderer::Init element no reconegut %s", l_treeRenderer.GetName());
            }
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

void CRenderer::Render(CProcess* _pProcess)
{
  if(m_bRenderPathsChanged)
  {
    ActivateRenderPaths();
  }

  CRenderManager* l_pRM = RENDER_MANAGER;
  CEffectManager* l_pEM = CORE->GetEffectManager();

  m_pCamera = _pProcess->GetCamera();

  l_pRM->BeginRendering();

  vector<CPreSceneRendererStep*>::iterator l_itPreRenderer = m_vPreSceneRendererSteps.begin();
  vector<CPreSceneRendererStep*>::iterator l_itPreRendererEnd = m_vPreSceneRendererSteps.end();


  for(;l_itPreRenderer != l_itPreRendererEnd; ++l_itPreRenderer)
  {
    CPreSceneRendererStep* l_pPreSceneRenderer = (*l_itPreRenderer);

    if(l_pPreSceneRenderer->IsActive())
    {
      string l_szRenderTarget = l_pPreSceneRenderer->GetRenderTarget();
      map<string,CRenderTarget*>::const_iterator l_it = m_mapRenderTargets.find(l_szRenderTarget);
      if(l_it != m_mapRenderTargets.end())
      {
        l_it->second->Activate(l_pRM);
      }

      l_pEM->SetTextureWidthHeight(l_it->second->GetWidth(), l_it->second->GetHeight());

      l_pPreSceneRenderer->Render(l_pRM,m_pCamera);
    }
  }



  if(m_pCurrentSceneRenderer)
  {
    string l_szRenderTarget = m_pCurrentSceneRenderer->GetRenderTarget();
    map<string,CRenderTarget*>::const_iterator l_it = m_mapRenderTargets.find(l_szRenderTarget);
    if(l_it != m_mapRenderTargets.end())
    {
      l_it->second->Activate(l_pRM);
    }
    
    l_pEM->SetTextureWidthHeight(l_it->second->GetWidth(), l_it->second->GetHeight());

    m_pCurrentSceneRenderer->Render(l_pRM,m_pCamera);
  }
  

  vector<CPostSceneRendererStep*>::iterator l_itPostRenderer = m_vPostSceneRendererSteps.begin();
  vector<CPostSceneRendererStep*>::iterator l_itPostRendererEnd = m_vPostSceneRendererSteps.end();

  for(;l_itPostRenderer != l_itPostRendererEnd; ++l_itPostRenderer)
  {
    CPostSceneRendererStep* l_pPostSceneRenderer = (*l_itPostRenderer);

    if(l_pPostSceneRenderer->IsActive())
    {
      string l_szRenderTarget = l_pPostSceneRenderer->GetRenderTarget();
      map<string,CRenderTarget*>::const_iterator l_it = m_mapRenderTargets.find(l_szRenderTarget);
      if(l_it != m_mapRenderTargets.end())
      {
        l_it->second->Activate(l_pRM);
      }
      
      l_pEM->SetTextureWidthHeight(l_it->second->GetWidth(), l_it->second->GetHeight());

      l_pPostSceneRenderer->Render(l_pRM,m_pCamera);
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

  m_pCurrentSceneRenderer = 0;

  vector<CPreSceneRendererStep*>::iterator l_itPreRenderer = m_vPreSceneRendererSteps.begin();
  vector<CPreSceneRendererStep*>::iterator l_itPreRendererEnd = m_vPreSceneRendererSteps.end();

  for(;l_itPreRenderer != l_itPreRendererEnd; ++l_itPreRenderer)
  {
    CHECKED_DELETE(*l_itPreRenderer);
  }

  m_vPreSceneRendererSteps.clear();
  m_mapPreSceneRendererSteps.clear();

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

CPreSceneRendererStep* CRenderer::GetPreSceneRendererStep(string _szName)
{
  map<string,CPreSceneRendererStep*>::const_iterator l_It = m_mapPreSceneRendererSteps.find(_szName);
  if(l_It == m_mapPreSceneRendererSteps.end()) 
  {
    return 0;
  }
  return l_It->second;
}

void CRenderer::SetSceneRenderer(const string& _szRendererName)
{
  m_pCurrentSceneRenderer = 0;

  map<string,CSceneRendererStep*>::iterator l_it = m_mapSceneRendererSteps.find(_szRendererName);
  if(l_it != m_mapSceneRendererSteps.end())
  {
    m_pCurrentSceneRenderer = l_it->second;
  }
  else
  {
    m_pCurrentSceneRenderer = 0;
  }
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

void CRenderer::ActivateRenderPaths()
{
  //desactivar els steps
  for(uint32 i = 0; i < m_vPreSceneRendererSteps.size(); ++i)
  {
    m_vPreSceneRendererSteps[i]->SetActive(false);
  }
  for(uint32 i = 0; i < m_vPostSceneRendererSteps.size(); ++i)
  {
    m_vPostSceneRendererSteps[i]->SetActive(false);
  }
  m_pCurrentSceneRenderer = 0;

  map<string, SRenderPath*>::iterator l_it  = m_mapRenderPaths.begin();
  map<string, SRenderPath*>::iterator l_end = m_mapRenderPaths.end();
  for(; l_it != l_end; ++l_it)
  {
    if(l_it->second->m_bActive)
    {
      set<string>::iterator l_it2  = l_it->second->m_PreSceneRenderSteps.begin();
      set<string>::iterator l_end2 = l_it->second->m_PreSceneRenderSteps.end  ();
      for(; l_it2 != l_end2; ++l_it2)
      {
        map<string,CPreSceneRendererStep*>::iterator l_it = m_mapPreSceneRendererSteps.find(*l_it2);
        if(l_it != m_mapPreSceneRendererSteps.end())
        {
          l_it->second->SetActive(true);
        }
      }

      l_it2  = l_it->second->m_PostSceneRenderSteps.begin();
      l_end2 = l_it->second->m_PostSceneRenderSteps.end  ();
      for(; l_it2 != l_end2; ++l_it2)
      {
        map<string,CPostSceneRendererStep*>::iterator l_it = m_mapPostSceneRendererSteps.find(*l_it2);
        if(l_it != m_mapPostSceneRendererSteps.end())
        {
          l_it->second->SetActive(true);
        }
      }

      if(l_it->second->m_szSceneRenderer != "")
      {
        SetSceneRenderer(l_it->second->m_szSceneRenderer);
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
