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

  string l_szName = _params.GetPszISOProperty("name","");
  SetName(l_szName);
  LOGGER->AddNewLog(ELL_INFORMATION, "CDrawQuadSceneEffect::Init  \"%s\".", l_szName.c_str());
  string l_szTechnique = _params.GetPszISOProperty("technique","");
  
  CEffectManager* l_pEffectManager = RENDER_MANAGER->GetEffectManager();
  m_pTechnique    = l_pEffectManager->GetEffectTechnique(l_szTechnique);

  int l_iNumChildren = _params.GetNumChildren();

  for(int i = 0; i < l_iNumChildren; i++)
  {
    CXMLTreeNode l_treeTextures = _params(i);
    if(strcmp(l_treeTextures.GetName(), "texture") == 0)
    {
      int l_iStage = l_treeTextures.GetIntProperty("stage_id", 0);
      string l_szTextureName = l_treeTextures.GetPszISOProperty("texture", "");
      if(l_szTextureName == "")
      {
        LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading Texture, no texture name.");
        Release();
        SetOk(false);
        return false;
      }
      CTexture* l_pTexture = RENDER_MANAGER->GetTextureManager()->GetResource(l_szTextureName);
      if(!l_pTexture)
      {
        LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading Texture, no texture \"%s\" exists.", l_szTextureName.c_str());
        Release();
        SetOk(false);
        return false;
      }

      m_vTextures.push_back(QuadTextures(l_iStage,l_pTexture));
    } else if(!l_treeTextures.IsComment())
    {
      LOGGER->AddNewLog(ELL_WARNING, "CDrawQuadSceneEffect::Init  Tag \"%s\" not recognized.", l_treeTextures.GetName());
    }
  }

  if(!m_pTechnique)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading Technique, no technique \"%s\" exists.", l_szTechnique.c_str());
    SetOk(false);
  } else {
    SetOk(true);
  }

  return IsOk();
}

void CDrawQuadSceneEffect::PostRender(CRenderManager *RM)
{}

void CDrawQuadSceneEffect::Release()
{
  m_vTextures.clear();
}