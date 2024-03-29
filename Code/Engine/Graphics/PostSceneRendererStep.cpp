#include "PostSceneRendererStep.h"
#include "XML/XMLTreeNode.h"
#include "EffectManager.h"
#include "Core.h"
#include "RenderManager.h"

bool CPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget, bool _bNeedsEffect)
{
  string l_szEffect = _treePostSceneRenderer.GetPszISOProperty("effect","",!_bNeedsEffect);

  if(!CRendererStep::Init(_treePostSceneRenderer, _szDefaultRenderTarget))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init PostSceneRenderer sense nom");
    SetOk(false);
  }else if(l_szEffect == "" && _bNeedsEffect)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init PostSceneRenderer sense effect");
    SetOk(false);
  }else{
    
    LOGGER->AddNewLog(ELL_INFORMATION,"CPostSceneRendererStep::Init iniciant %s",GetName().c_str());

    CRendererStep::Init(_treePostSceneRenderer, _szDefaultRenderTarget);

    CXMLTreeNode l_treeSamplers = _treePostSceneRenderer.GetChild("input_samplers");
    CXMLTreeNode l_treeRenderTargets = _treePostSceneRenderer.GetChild("render_targets");

    if(!InitInputSamplers(l_treeSamplers))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init error inicialitzant input_samplers");
      SetOk(false);
    }else{

      m_szEffect = l_szEffect;

      m_bUseTime = _treePostSceneRenderer.GetBoolProperty("use_time",false,false);

      if(m_bUseTime)
      {
        m_bUseDeltaTime = _treePostSceneRenderer.GetBoolProperty("use_delta_time",false,false);
      }
      else
      {
        m_bUseDeltaTime = false;
      }

      m_bUseCenter = _treePostSceneRenderer.GetBoolProperty("use_center",false,false);

      string l_szAlignment = _treePostSceneRenderer.GetPszISOProperty("alignment","",false);
      if(l_szAlignment == "center")
      {
        m_Alignment = CENTER;
      }

      Vect2f l_fPos = _treePostSceneRenderer.GetVect2fProperty("position",Vect2f(0),false);

      m_iPos.x = (int) (l_fPos.x * RENDER_MANAGER->GetScreenWidth());
      m_iPos.y = (int) (l_fPos.y * RENDER_MANAGER->GetScreenHeight());

      if(_treePostSceneRenderer.ExistsProperty("size"))
      {
        float l_fSizeX = _treePostSceneRenderer.GetFloatProperty("size",1.0,false);

        if(_treePostSceneRenderer.GetBoolProperty("absolute_size",false,false))
        {
          m_iSize.x = (int) l_fSizeX;
          m_iSize.y = m_iSize.x;
        }
        else
        {
          m_iSize.x = (int) (l_fSizeX * RENDER_MANAGER->GetScreenWidth());
          m_iSize.y = m_iSize.x;
        }

        if(_treePostSceneRenderer.ExistsProperty("aspect_ratio"))
        {
          bool l_bAspectRatio = _treePostSceneRenderer.GetBoolProperty("aspect_ratio",true,false);

          if(l_bAspectRatio)
          {
            m_iSize.y = (int)(m_iSize.x * RENDER_MANAGER->GetScreenHeight()/(float)RENDER_MANAGER->GetScreenWidth());
          }
        }
        
      } else if(_treePostSceneRenderer.ExistsProperty("size_x"))
      {
        float l_fSizeX = _treePostSceneRenderer.GetFloatProperty("size_x",1.0,false);
        float l_fSizeY = _treePostSceneRenderer.GetFloatProperty("size_y",1.0,false);

        if(_treePostSceneRenderer.GetBoolProperty("absolute_size",false,false))
        {
          m_iSize.x = (int) l_fSizeX;
          m_iSize.y = (int) l_fSizeY;
        }
        else
        {
          m_iSize.x = (int) (l_fSizeX * RENDER_MANAGER->GetScreenWidth());
          m_iSize.y = (int) (l_fSizeY * RENDER_MANAGER->GetScreenHeight());
        }

      } else
      {
        m_iSize.x = RENDER_MANAGER->GetScreenWidth(); 
        m_iSize.y = RENDER_MANAGER->GetScreenHeight();
      }

      SetOk(true);
    }
  }

  return IsOk();
}

void CPostSceneRendererStep::Render(CRenderManager* _pRM)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();

  l_pEM->SetAlphaFactor(m_fAlphaFactor);
  l_pEM->SetViewport(m_iPos.x, m_iPos.y, m_iSize.x, m_iSize.y);

  if(m_bUseTime)
  {
    l_pEM->SetTime(m_fTime);
  }

  if(m_bUseCenter)
  {
    l_pEM->SetCenterXY(m_vCenter.x,m_vCenter.y);
  }

  CEffect* l_pEffect = l_pEM->GetResource(m_szEffect);

  if(l_pEffect)
  {
    ActivateInputSamplers();

    l_pEM->LoadShaderData(l_pEffect);

    LPD3DXEFFECT l_pD3DEffect = l_pEffect->GetD3DEffect();

    if(l_pD3DEffect!=NULL)
    {
      UINT l_NumPasses;
      l_pD3DEffect->Begin(&l_NumPasses, 0);
      for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
      {
        l_pD3DEffect->BeginPass(iPass);
        _pRM->DrawColoredTexturedQuad2D(m_iPos,m_iSize.x,m_iSize.y,m_Alignment,CColor(Vect4f(0,0,0,0)));
        l_pD3DEffect->EndPass();
      }
      l_pD3DEffect->End();
    }

    DeactivateInputSamplers();
  }
}

void CPostSceneRendererStep::Release()
{
  CRendererStep::Release();
}
