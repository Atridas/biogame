#include "RendererStep.h"
//#include "RenderTarget.h"
//#include "TextureRenderTarget.h"
//#include "BackBufferRenderTarget.h"
#include "InputSampler.h"
#include "TextureInputSampler.h"
#include "FrameBufferInputSampler.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"

bool CRendererStep::Init(CXMLTreeNode& _treeSceneRenderer, const string& _szDefaultRenderTarget) 
{
  string l_szName = _treeSceneRenderer.GetPszISOProperty("name","",false);
  m_szRenderTarget = _treeSceneRenderer.GetPszISOProperty("render_target",_szDefaultRenderTarget.c_str(),false);
  bool l_bActive = _treeSceneRenderer.GetBoolProperty("active",true,false);

  m_bClearColor = _treeSceneRenderer.GetBoolProperty("clear_color",false,false);
  if(m_bClearColor)
  {
    CColor l_DefaultColor = RENDER_MANAGER->GetClearColor();

    Vect4f l_Color = _treeSceneRenderer.GetVect4fProperty("clear_color_value", 
                                                          Vect4f(l_DefaultColor.GetRed(),  
                                                                 l_DefaultColor.GetGreen(),
                                                                 l_DefaultColor.GetBlue(), 
                                                                 l_DefaultColor.GetAlpha()),
                                                          false);
	  uint32 red		= (uint32) (l_Color.x * 255);
	  uint32 green	= (uint32) (l_Color.y * 255);
	  uint32 blue		= (uint32) (l_Color.z * 255);
	  uint32 alpha	= (uint32) (l_Color.w * 255);

    m_Color = D3DCOLOR_ARGB(alpha, red, green, blue);
  }

  m_bClearDepth = _treeSceneRenderer.ExistsProperty("clear_depth");
  if(m_bClearDepth)
  {
    m_fDepth = _treeSceneRenderer.GetFloatProperty("clear_depth");
  }

  m_bClearStencil = _treeSceneRenderer.ExistsProperty("clear_stencil");
  if(m_bClearStencil)
  {
    m_iStencil = _treeSceneRenderer.GetIntProperty("clear_stencil");
  }

  SetName(l_szName);
  SetActive(l_bActive);

  SetOk(l_szName != ""); 
  return IsOk();
};

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


void CRendererStep::ClearBuffer(CRenderManager* l_pRM) const
{
  uint32 flags = 0;
  flags |= (m_bClearColor)  ? D3DCLEAR_TARGET  : 0;
  flags |= (m_bClearDepth)  ? D3DCLEAR_ZBUFFER : 0;
  flags |= (m_bClearStencil)? D3DCLEAR_STENCIL : 0;

  if(flags == 0)
    return;

  l_pRM->GetDevice()->Clear(0, NULL, flags, m_Color, m_fDepth, m_iStencil);
}

void CRendererStep::Release()
{

  vector<CInputSampler*>::iterator l_itSampler = m_vInputSamplers.begin();
  vector<CInputSampler*>::iterator l_itSamplerEnd = m_vInputSamplers.end();

  for(;l_itSampler != l_itSamplerEnd;++l_itSampler)
  {
    CHECKED_DELETE(*l_itSampler);
  }
  m_vInputSamplers.clear();
}
