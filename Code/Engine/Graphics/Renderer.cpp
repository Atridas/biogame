#include "Renderer.h"
#include "RendererStep.h"
#include "XML/XMLTreeNode.h"
#include "PreSceneRendererStep.h"
#include "SceneRendererStep.h"
#include "PostSceneRendererStep.h"
#include "Core.h"
#include "RenderManager.h"
#include "Process.h"
#include "EffectManager.h"
//#include "Console.h"
//#include "GUIManager.h"

bool CRenderer::Init(const string& _szFileName)
{
  m_szFileName = _szFileName;

  CXMLTreeNode l_treeRenderer;

  if(!l_treeRenderer.LoadFile(_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init Error al carregar el fitxer XML %s",_szFileName.c_str());
    SetOk(false);
  }else{
  
    LOGGER->AddNewLog(ELL_INFORMATION,"CRenderer::Init inicialitzant fitxer XML %s",_szFileName.c_str());

    CXMLTreeNode l_treePreRenderers = l_treeRenderer.GetChild("pre_scene_renderers");
    CXMLTreeNode l_treeRenderers = l_treeRenderer.GetChild("scene_renderers");
    CXMLTreeNode l_treePostRenderers = l_treeRenderer.GetChild("post_scene_renderers");

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
          CPreSceneRendererStep* l_pPreRenderer = new CPreSceneRendererStep();

          if(!l_pPreRenderer->Init(l_treePreRenderer))
          {
            CHECKED_DELETE(l_pPreRenderer);
            LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init error inicialitzant PreRenderer");
          }else{
            m_vPreSceneRendererSteps.push_back(l_pPreRenderer);
            m_mapPreSceneRendererSteps[l_pPreRenderer->GetName()] = l_pPreRenderer;
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

          if(!l_pRenderer->Init(l_treeRenderer))
          {
            CHECKED_DELETE(l_pRenderer);
            LOGGER->AddNewLog(ELL_ERROR,"CRenderer::Init error inicialitzant SceneRenderer");
          }else{

            m_vSceneRendererSteps.push_back(l_pRenderer);
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
          CPostSceneRendererStep* l_pPostRenderer = new CPostSceneRendererStep();

          if(!l_pPostRenderer->Init(l_treePostRenderer))
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
    
    SetOk(true);
  }

  return IsOk();
}

void CRenderer::Render(CProcess* _pProcess)
{
  
  CEffectManager* l_pEM = CORE->GetEffectManager();
  l_pEM->Begin();

  RENDER_MANAGER->BeginRendering();

  vector<CPreSceneRendererStep*>::iterator l_itPreRenderer = m_vPreSceneRendererSteps.begin();
  vector<CPreSceneRendererStep*>::iterator l_itPreRendererEnd = m_vPreSceneRendererSteps.end();


  for(;l_itPreRenderer != l_itPreRendererEnd; ++l_itPreRenderer)
  {
    CPreSceneRendererStep* l_pPreSceneRenderer = (*l_itPreRenderer);

    if(l_pPreSceneRenderer->IsActive())
    {
      l_pPreSceneRenderer->Render(_pProcess);
    }
  }



  if(m_pCurrentSceneRenderer)
  {
    m_pCurrentSceneRenderer->Render(_pProcess);
  }
  

  vector<CPostSceneRendererStep*>::iterator l_itPostRenderer = m_vPostSceneRendererSteps.begin();
  vector<CPostSceneRendererStep*>::iterator l_itPostRendererEnd = m_vPostSceneRendererSteps.end();

  for(;l_itPostRenderer != l_itPostRendererEnd; ++l_itPostRenderer)
  {
    CPostSceneRendererStep* l_pPostSceneRenderer = (*l_itPostRenderer);

    if(l_pPostSceneRenderer->IsActive())
    {
      l_pPostSceneRenderer->Render(_pProcess);
    }
  }

  _pProcess->DebugInformation();

  //RenderSystems(RENDER_MANAGER);

  RENDER_MANAGER->EndRendering();

  RENDER_MANAGER->Present();
}

/*void CRenderer::RenderSystems(CRenderManager* _pRM)
{
  CFontManager* l_pFontManager = CORE->GetFontManager();
  CLogRender* l_pLR = CORE->GetLogRender();

  _pRM->EnableAlphaBlend();

  if(l_pLR)
    l_pLR->Render(_pRM,l_pFontManager);

  CConsole* l_pC = CORE->GetConsole();
  if(l_pC)
    l_pC->Render(_pRM,l_pFontManager);

  CGUIManager* l_pGUI = CORE->GetGUIManager();
  if(l_pGUI)
  {
    l_pGUI->Render(_pRM,l_pFontManager);
    l_pGUI->RenderPointerMouse(_pRM,l_pFontManager);
  }

  _pRM->DisableAlphaBlend();
}*/

void CRenderer::Release()
{
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

  vector<CPostSceneRendererStep*>::iterator l_itPostRenderer = m_vPostSceneRendererSteps.begin();
  vector<CPostSceneRendererStep*>::iterator l_itPostRendererEnd = m_vPostSceneRendererSteps.end();

  for(;l_itPostRenderer != l_itPostRendererEnd; ++l_itPostRenderer)
  {
    CHECKED_DELETE(*l_itPostRenderer);
  }

  m_vPostSceneRendererSteps.clear();
  m_mapPostSceneRendererSteps.clear();

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

  vector<CSceneRendererStep*>::iterator l_itRenderer = m_vSceneRendererSteps.begin();
  vector<CSceneRendererStep*>::iterator l_itRendererEnd = m_vSceneRendererSteps.end();

  if(l_itRenderer != l_itRendererEnd)
  {
    m_pCurrentSceneRenderer = *l_itRenderer;
  }

  for(;l_itRenderer != l_itRendererEnd; ++l_itRenderer)
  {
    CSceneRendererStep* l_pSceneRenderer = *l_itRenderer;

    if(l_pSceneRenderer->GetName() == _szRendererName)
    {
      m_pCurrentSceneRenderer = l_pSceneRenderer;
      break;
    }
  }
}
