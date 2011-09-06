#include "RendererStep.h"
#include "RenderTarget.h"
#include "TextureRenderTarget.h"
#include "BackBufferRenderTarget.h"
#include "InputSampler.h"
#include "TextureInputSampler.h"
#include "FrameBufferInputSampler.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"

bool CRendererStep::InitRenderTargets(CXMLTreeNode& _treeRenderTargets)
{
  if(_treeRenderTargets.Exists())
  {
    string l_szFormatType = _treeRenderTargets.GetPszISOProperty("format_type","A8R8G8B8");

    float l_fScale = _treeRenderTargets.GetFloatProperty("scale_size",1.0f,false);
    int l_iWidth = _treeRenderTargets.GetIntProperty("width",(int)(RENDER_MANAGER->GetScreenWidth()*l_fScale),false);
    int l_iHeight = _treeRenderTargets.GetIntProperty("height",(int)(RENDER_MANAGER->GetScreenHeight()*l_fScale),false);
    
    m_iRenderTargetWidth = l_iWidth;
    m_iRenderTargetHeight = l_iHeight;

    int l_iNumRenderTargets = _treeRenderTargets.GetNumChildren();

    for(int l_iIndex = 0; l_iIndex < l_iNumRenderTargets;l_iIndex++)
    {
      CXMLTreeNode l_pRenderTargetNode = _treeRenderTargets(l_iIndex);

      if(string(l_pRenderTargetNode.GetName()) == "render_target")
      {
        CTextureRenderTarget* l_pRenderTarget = 0;

        l_pRenderTarget = new CTextureRenderTarget();

        if(l_pRenderTarget->Init(l_pRenderTargetNode,l_iWidth,l_iHeight,l_szFormatType))
        {
          m_vRenderTargets.push_back(l_pRenderTarget);
        }else{
          CHECKED_DELETE(l_pRenderTarget);
          LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitRenderTargets init render_target ha fallat.");
        }
      }
      else if(!l_pRenderTargetNode.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitRenderTargets element no reconegut %s",l_pRenderTargetNode.GetName());
      }

    }

    SetOk(true);

  }else{

    m_iRenderTargetWidth = RENDER_MANAGER->GetScreenWidth();
    m_iRenderTargetHeight = RENDER_MANAGER->GetScreenHeight();

    CBackBufferRenderTarget* l_pRenderTarget = 0;

    l_pRenderTarget = new CBackBufferRenderTarget();

    if(l_pRenderTarget->Init())
    {
      m_vRenderTargets.push_back(l_pRenderTarget);
      SetOk(true);
    }else{
      CHECKED_DELETE(l_pRenderTarget);
      LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitRenderTargets init render_target backbuffer ha fallat.");
      SetOk(false);
    }
  }

  return IsOk();
}

bool CRendererStep::InitInputSamplers(CXMLTreeNode& _treeInputSamplers)
{
  if(!_treeInputSamplers.Exists())
  {
    LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers no hi ha input samplers.");
    
  }else{

    int l_iNumChildren = _treeInputSamplers.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; ++i)
    {
      CXMLTreeNode l_treeInputSampler = _treeInputSamplers(i);
      if(string(l_treeInputSampler.GetName()) == "input_sampler")
      {
        string l_szType = l_treeInputSampler.GetPszISOProperty("type", "frame_buffer", true);
        int l_iIndex = l_treeInputSampler.GetIntProperty("index", 0, true);
        CInputSampler *l_pInputSampler = 0;
        if(l_szType == "frame_buffer")
        {
          CFrameBufferInputSampler *l_pFBInputSampler = new CFrameBufferInputSampler();
          if(l_pFBInputSampler->Init(l_iIndex))
          {
            l_pInputSampler = l_pFBInputSampler;
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers init input_sampler frame_buffer ha fallat.");
            delete l_pFBInputSampler;
          }
        }
        else if(l_szType == "texture")
        {
          CTextureInputSampler *l_pTInputSampler = new CTextureInputSampler();
          string l_szTexturename = l_treeInputSampler.GetPszISOProperty("texture", "", true);
          if(l_pTInputSampler->Init(l_iIndex, l_szTexturename))
          {
            l_pInputSampler = l_pTInputSampler;
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers init input_sampler texture ha fallat.");
            delete l_pTInputSampler;
          }
        }
        else
        {
          LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers tipus input_sampler desconegut: %s",l_szType.c_str());
        }

        if(l_pInputSampler)
        {
          m_vInputSamplers.push_back(l_pInputSampler);
        }

      }
      else
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitRenderTargets element no reconegut %s",l_treeInputSampler.GetName());
      }
    }

    
  }

  return true;
}

void CRendererStep::ActivateInputSamplers()
{
  vector<CInputSampler*>::iterator l_itSampler = m_vInputSamplers.begin();
  vector<CInputSampler*>::iterator l_itSamplerEnd = m_vInputSamplers.end();

  for(;l_itSampler != l_itSamplerEnd;++l_itSampler)
  {
    CInputSampler* l_pInputSampler = (*l_itSampler);
    l_pInputSampler->Activate();
  }
}

void CRendererStep::ActivateRenderTargets()
{
  vector<CRenderTarget*>::iterator l_itRendertarget = m_vRenderTargets.begin();
  vector<CRenderTarget*>::iterator l_itRendertargetEnd = m_vRenderTargets.end();

  for(;l_itRendertarget != l_itRendertargetEnd;++l_itRendertarget)
  {
    CRenderTarget* l_pRenderTarget = (*l_itRendertarget);
    l_pRenderTarget->Activate();
  }
  RENDER_MANAGER->GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );
}

void CRendererStep::DeactivateRenderTargets()
{
  vector<CRenderTarget*>::iterator l_itRendertarget = m_vRenderTargets.begin();
  vector<CRenderTarget*>::iterator l_itRendertargetEnd = m_vRenderTargets.end();

  for(;l_itRendertarget != l_itRendertargetEnd;++l_itRendertarget)
  {
    CRenderTarget* l_pRenderTarget = (*l_itRendertarget);
    l_pRenderTarget->Deactivate();
  }
}

void CRendererStep::Release()
{
  vector<CRenderTarget*>::iterator l_itRendertarget = m_vRenderTargets.begin();
  vector<CRenderTarget*>::iterator l_itRendertargetEnd = m_vRenderTargets.end();

  for(;l_itRendertarget != l_itRendertargetEnd;++l_itRendertarget)
  {
    CHECKED_DELETE(*l_itRendertarget);
  }
  m_vRenderTargets.clear();

  vector<CInputSampler*>::iterator l_itSampler = m_vInputSamplers.begin();
  vector<CInputSampler*>::iterator l_itSamplerEnd = m_vInputSamplers.end();

  for(;l_itSampler != l_itSamplerEnd;++l_itSampler)
  {
    CHECKED_DELETE(*l_itSampler);
  }
  m_vInputSamplers.clear();
}
