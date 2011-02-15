#include "SceneEffect.h"
#include "Core.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "Process.h"
#include "Texture.h"
#include "XML\XMLTreeNode.h"

bool CSceneEffect::Init(const CXMLTreeNode& _params)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CSceneEffect::Init  Initializing CSceneEffect.");
  
  string l_szName = _params.GetPszISOProperty("name","");
  SetName(l_szName);
  LOGGER->AddNewLog(ELL_INFORMATION, "CSceneEffect::Init  \"%s\".", l_szName.c_str());

  SetActive( _params.GetBoolProperty("active", true) );

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
      CTexture* l_pTexture = CORE->GetTextureManager()->GetResource(l_szTextureName);
      if(!l_pTexture)
      {
        LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadSceneEffect::Init  Error loading Texture, no texture \"%s\" exists.", l_szTextureName.c_str());
        Release();
        SetOk(false);
        return false;
      }

      AddStageTexture(l_iStage, l_pTexture);
    } else if(!l_treeTextures.IsComment())
    {
      LOGGER->AddNewLog(ELL_WARNING, "CDrawQuadSceneEffect::Init  Tag \"%s\" not recognized.", l_treeTextures.GetName());
    }
  }
  SetOk(true);
  return IsOk();
}

void CSceneEffect::PreRender(CRenderManager* _pRM, CProcess* _pProc)
{

}
void CSceneEffect::PostRender(CRenderManager* _pRM)
{

}
void CSceneEffect::CaptureFrameBuffers(CRenderManager* _pRM)
{

}

void CSceneEffect::CStageTexture::Activate() const
{
  m_pTexture->Activate(m_iStageId);
}

void CSceneEffect::ActivateTextures()
{
  vector<CStageTexture>::const_iterator l_end = m_vStageTextures.cend();
  for(vector<CStageTexture>::const_iterator l_it = m_vStageTextures.cbegin(); l_it != l_end; ++l_it)
  {
    l_it->Activate();
  }
}

void CSceneEffect::AddStageTexture(int _iStageId, CTexture* _pTexture)
{
  m_vStageTextures.push_back(CStageTexture(_iStageId,_pTexture));
}

void CSceneEffect::Release()
{
  m_vStageTextures.clear();
}