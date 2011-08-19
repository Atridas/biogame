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

  m_bUseTime = _params.GetBoolProperty("use_time",false,false);

  string l_szAlignment = _params.GetPszISOProperty("alignment","",false);

  if(l_szAlignment.compare("center") == 0)
  {
    m_Alignment = CENTER;
  }

  Vect2f l_fPos = _params.GetVect2fProperty("position",Vect2f(0),false);

  m_iPos.x = (int) (l_fPos.x * RENDER_MANAGER->GetScreenWidth());
  m_iPos.y = (int) (l_fPos.y * RENDER_MANAGER->GetScreenHeight());

  if(_params.ExistsProperty("size_x"))
  {
    float l_fSizeX = _params.GetFloatProperty("size_x",1.0,false);

    m_iSize.x = (int) (l_fSizeX * RENDER_MANAGER->GetScreenWidth());
    m_iSize.y = m_iSize.x;
  }else{
    m_iSize.x = RENDER_MANAGER->GetScreenWidth(); 
    m_iSize.y = RENDER_MANAGER->GetScreenHeight();
  }

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
    LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading effect, no effect \"%s\" exists.", m_szEffect.c_str());
    SetOk(false);
  } else {
    SetOk(true);
  }

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
    CEffectManager* l_pEffectManager = CORE->GetEffectManager();
    l_pEffectManager->SetTextureWidthHeight(m_iSize.x,m_iSize.y);
    l_pEffectManager->SetAlphaFactor(m_fAlphaFactor);

    if(m_bUseTime)
    {
      l_pEffectManager->SetTime(m_fTime);
    }

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
        _pRM->DrawColoredTexturedQuad2D(m_iPos,m_iSize.x,m_iSize.y,m_Alignment,m_Color);
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