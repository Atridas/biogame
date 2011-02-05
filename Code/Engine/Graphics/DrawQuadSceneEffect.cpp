#include "DrawQuadSceneEffect.h"
#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
#include "EffectTechnique.h"
#include "Texture.h"
#include "TextureManager.h"

bool CDrawQuadSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CDrawQuadSceneEffect::Init  Initializing CDrawQuadSceneEffect.");

  if(!CSceneEffect::Init(_params))
    return false;

  Vect4f l_Color = _params.GetVect4fProperty("color",Vect4f(0,0,0,0));
  m_Color.SetRed  (l_Color.x);
  m_Color.SetGreen(l_Color.y);
  m_Color.SetBlue (l_Color.z);
  m_Color.SetAlpha(l_Color.w);

  m_szTechnique = _params.GetPszISOProperty("technique","");
  
  CEffectManager* l_pEffectManager = RENDER_MANAGER->GetEffectManager();
  m_pTechnique    = l_pEffectManager->GetEffectTechnique(m_szTechnique);

  if(!m_pTechnique)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading Technique, no technique \"%s\" exists.", m_szTechnique.c_str());
    Release();
    SetOk(false);
  } else {
    SetOk(true);
  }

  return IsOk();
}

void CDrawQuadSceneEffect::PostRender(CRenderManager *_pRM)
{
  if(IsOk())
  {
    //RECT l_Rect;
    //l_Rect.top=l_Rect.left=0;
    //l_Rect.bottom=_pRM->GetScreenHeight();
    //l_Rect.right=_pRM->GetScreenWidth();
    Vect2i posInit(0,0);
    uint32 w = _pRM->GetScreenWidth();
    uint32 h = _pRM->GetScreenHeight();

    /*if(!m_pTechnique->IsOk())
    {
      CEffectManager* l_pEffectManager = RENDER_MANAGER->GetEffectManager();
      m_pTechnique    = l_pEffectManager->GetEffectTechnique(m_szTechnique);

      if(!m_pTechnique || !m_pTechnique->IsOk())
      {
        Release();
        SetOk(false);
        return;
      }
    }*/

    m_pTechnique->BeginRender(0);
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
        _pRM->DrawTexturedQuad2D (posInit,w,h,UPPER_LEFT,m_Color);
        l_Effect->EndPass();
      }
      l_Effect->End();
    }
  }
}

void CDrawQuadSceneEffect::Release()
{
  CSceneEffect::Release();
}