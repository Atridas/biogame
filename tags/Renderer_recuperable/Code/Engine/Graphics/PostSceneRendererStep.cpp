#include "PostSceneRendererStep.h"
#include "XML/XMLTreeNode.h"
#include "EffectManager.h"
#include "Core.h"
#include "RenderManager.h"

bool CPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer)
{
  string l_szName = _treePostSceneRenderer.GetPszISOProperty("name","",false);
  string l_szEffect = _treePostSceneRenderer.GetPszISOProperty("effect","",false);
  bool l_bActive = _treePostSceneRenderer.GetBoolProperty("active",true,false);

  if(l_szName == "")
  {
    LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init PostSceneRenderer sense nom");
    SetOk(false);
  }else if(l_szEffect == "")
  {
    LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init PostSceneRenderer sense effect");
    SetOk(false);
  }else{
    
    CXMLTreeNode l_treeSamplers = _treePostSceneRenderer.GetChild("input_samplers");
    CXMLTreeNode l_treeRenderTargets = _treePostSceneRenderer.GetChild("render_targets");

    if(!InitInputSamplers(l_treeSamplers))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init error inicialitzant input_samplers");
      SetOk(false);
    }else if(!InitRenderTargets(l_treeRenderTargets))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CPostSceneRendererStep::Init error inicialitzant render_targets");
      SetOk(false);
    }else{

      SetName(l_szName);
      SetActive(l_bActive);

      m_szEffect = l_szEffect;

      m_bUseTime = _treePostSceneRenderer.GetBoolProperty("use_time",false,false);

      string l_szAlignment = _treePostSceneRenderer.GetPszISOProperty("alignment","",false);
      if(l_szAlignment == "center")
      {
        m_Alignment = CENTER;
      }

      Vect2f l_fPos = _treePostSceneRenderer.GetVect2fProperty("position",Vect2f(0),false);

      m_iPos.x = (int) (l_fPos.x * m_iRenderTargetWidth);
      m_iPos.y = (int) (l_fPos.y * m_iRenderTargetHeight);

      if(_treePostSceneRenderer.ExistsProperty("size_x"))
      {
        float l_fSizeX = _treePostSceneRenderer.GetFloatProperty("size_x",1.0,false);

        m_iSize.x = (int) (l_fSizeX * m_iRenderTargetWidth);
        m_iSize.y = m_iSize.x;

        if(_treePostSceneRenderer.ExistsProperty("aspect_ratio"))
        {
          bool l_bAspectRatio = _treePostSceneRenderer.GetBoolProperty("aspect_ratio",true,false);

          if(l_bAspectRatio)
          {
            m_iSize.y = (int)(m_iSize.x * m_iRenderTargetHeight/(float)m_iRenderTargetWidth);
          }
        }

      }else{
        m_iSize.x = m_iRenderTargetWidth; 
        m_iSize.y = m_iRenderTargetHeight;
      }

      SetOk(true);
    }
  }

  return IsOk();
}

void CPostSceneRendererStep::Render(CRenderManager* _pRM)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();
  l_pEM->SetTextureWidthHeight(m_iRenderTargetWidth,m_iRenderTargetHeight);

  l_pEM->SetAlphaFactor(m_fAlphaFactor);

  if(m_bUseTime)
  {
    l_pEM->SetTime(m_fTime);
  }

  CEffect* l_pEffect = l_pEM->GetResource(m_szEffect);

  if(l_pEffect)
  {
    ActivateInputSamplers();
    ActivateRenderTargets(_pRM);

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
  
    DeactivateRenderTargets(_pRM);
    DeactivateInputSamplers();
  }
}

void CPostSceneRendererStep::Release()
{
  CRendererStep::Release();
}
