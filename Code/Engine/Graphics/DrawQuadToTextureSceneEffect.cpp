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
  
  string l_szTechnique = _params.GetPszISOProperty("technique","");
  
  CEffectManager* l_pEffectManager = CORE->GetEffectManager();
  m_pTechnique    = l_pEffectManager->GetEffectTechnique(l_szTechnique);

  if(!m_pTechnique)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadToTextureSceneEffect::Init  No technique \"%s\".", l_szTechnique.c_str());
    
    SetOk(false);
  }

  return IsOk();
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
    //aqui dintre, codi drawquad
    Vect2i posInit(0,0);
    uint32 w = m_pTexture->GetWidth();
    uint32 h = m_pTexture->GetHeight();

    m_pTechnique->BeginRender(m_pEffectMaterial);
    ActivateTextures();
    LPD3DXEFFECT l_Effect=m_pTechnique->GetEffect()->GetD3DEffect();
    if(l_Effect!=NULL)
    {
      l_Effect->SetTechnique(m_pTechnique->GetD3DTechnique());
      UINT l_NumPasses;
      l_Effect->Begin(&l_NumPasses, 0);
      for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
      {
        l_Effect->BeginPass(iPass);
        _pRM->DrawColoredTexturedQuad2D (posInit,w,h,UPPER_LEFT,m_Color);
        l_Effect->EndPass();
      }
      l_Effect->End();
    }
  }
  //a partir d'aqui codi render to texture

  _pRM->EndRendering();

  m_pTexture->UnsetAsRenderTarget();
}