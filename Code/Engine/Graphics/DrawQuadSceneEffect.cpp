#include "DrawQuadSceneEffect.h"
#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
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

  m_szEffect = _params.GetPszISOProperty("effect","");
  
  CEffectManager* l_pEffectManager = CORE->GetEffectManager();
  m_pEffect    = l_pEffectManager->GetEffect(m_szEffect);

  if(!m_pEffect)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading Technique, no technique \"%s\" exists.", m_szEffect.c_str());
    SetOk(false);
  } else {
    SetOk(true);
  }

  //m_pEffectMaterial = new CEffectMaterial();
  //m_pEffectMaterial->Init();
  m_iWidth = RENDER_MANAGER->GetScreenWidth(); 
  m_iHeight = RENDER_MANAGER->GetScreenHeight();

  if( IsOk() ) 
  {
    return true;
  } else {
    Release();
    return false;
  }
}

void CDrawQuadSceneEffect::PostRender(CRenderManager *_pRM)
{
  if(IsOk())
  {
    Vect2i posInit(0,0);
    uint32 w = _pRM->GetScreenWidth();
    uint32 h = _pRM->GetScreenHeight();

    CEffectManager* l_pEffectManager = CORE->GetEffectManager();
    l_pEffectManager->SetTextureWidthHeight(m_iWidth,m_iHeight);

    l_pEffectManager->LoadShaderData(m_pEffect);

    ActivateTextures();

    LPD3DXEFFECT l_pD3DEffect = m_pEffect->GetD3DEffect();

    if(l_pD3DEffect!=NULL)
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
}

void CDrawQuadSceneEffect::Release()
{
  CSceneEffect::Release();
  //CHECKED_DELETE(m_pEffectMaterial);
}