#include "DrawQuadToTextureSceneEffect.h"

#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Texture.h"
#include "Process.h"


bool CDrawQuadToTextureSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CDrawQuadToTextureSceneEffect::Init  Initializing CDrawQuadToTextureSceneEffect.");

  if(!CRenderTextureSceneEffect::Init(_params))
    return false;
  
  Vect4f l_Color = _params.GetVect4fProperty("color",Vect4f(0,0,0,0));
  m_Color.SetRed  (l_Color.x);
  m_Color.SetGreen(l_Color.y);
  m_Color.SetBlue (l_Color.z);
  m_Color.SetAlpha(l_Color.w);
  
  string l_szEffect = _params.GetPszISOProperty("effect","");
  
  CEffectManager* l_pEffectManager = CORE->GetEffectManager();
  m_pEffect = l_pEffectManager->GetEffect(l_szEffect);

  if(!m_pEffect)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadToTextureSceneEffect::Init  No effect \"%s\".", l_szEffect.c_str());
    
    SetOk(false);
  }

  if( IsOk() ) 
  {
    return true;
  } else {
    Release();
    return false;
  }
}

void CDrawQuadToTextureSceneEffect::Release()
{
  CRenderTextureSceneEffect::Release();
}


//Copia de RenderToTexture i DrawQuad

void CDrawQuadToTextureSceneEffect::PreRender(CRenderManager* _pRM, CProcess* _pProc)
{
  m_pTexture->SetAsRenderTarget();

  _pRM->BeginRendering();

  //fins aqui codi render to texture
  {
    CEffectManager* l_pEM = CORE->GetEffectManager();

    //aqui dintre, codi drawquad
    Vect2i posInit(0,0);
    uint32 w = m_pTexture->GetWidth();
    uint32 h = m_pTexture->GetHeight();

    l_pEM->SetTextureWidthHeight(w,h);

    l_pEM->LoadShaderData(m_pEffect);
    
    ActivateTextures();

    LPD3DXEFFECT l_pD3DEffect = m_pEffect->GetD3DEffect();

    if(l_pD3DEffect != 0)
    {
      UINT l_NumPasses;
      l_pD3DEffect->Begin(&l_NumPasses, 0);
      for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
      {
        l_pD3DEffect->BeginPass(iPass);
        _pRM->DrawColoredTexturedQuad2D (posInit,w,h,UPPER_LEFT,m_Color);
        l_pD3DEffect->EndPass();
      }
      l_pD3DEffect->End();
    }
  }
  //a partir d'aqui codi render to texture

  _pRM->EndRendering();

  m_pTexture->UnsetAsRenderTarget();
}