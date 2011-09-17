#include "DeferredPostSceneRendererStep.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Core.h"
#include "LightManager.h"

bool CDeferredPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget)
{
  return CPostSceneRendererStep::Init(_treePostSceneRenderer, _szDefaultRenderTarget);
}

void CDeferredPostSceneRendererStep::Release()
{
  CPostSceneRendererStep::Release();
}

void CDeferredPostSceneRendererStep::Render(CRenderManager* _pRM)
{
  CLightManager* l_pLightManager = CORE->GetLightManager();
  CEffectManager* l_pEM = CORE->GetEffectManager();
  //l_pEM->SetTextureWidthHeight(m_iRenderTargetWidth,m_iRenderTargetHeight);

  CEffect* l_pEffect = l_pEM->GetResource(m_szEffect);
  if(l_pEffect)
  {
    ActivateInputSamplers();
    //ActivateRenderTargets(_pRM);

    const vector<CLight*>& l_vLights = l_pLightManager->GetLights();

    for(uint32 i = 0; i < l_vLights.size(); i++)
    {
      if(!l_vLights[i]->IsActive())
        continue;

      l_pEM->SetLight(l_vLights[i]);

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
    }

    //DeactivateRenderTargets(_pRM);
    DeactivateInputSamplers();
  }
}