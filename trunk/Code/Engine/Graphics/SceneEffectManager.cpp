#include "SceneEffectManager.h"
#include "Process.h"
#include "RenderManager.h"
#include "SceneEffect.h"
#include "XML/XMLTreeNode.h"

#include "DrawQuadSceneEffect.h"
#include "ShadowMapRenderToTexture.h"
#include "RenderToTextureSceneEffect.h"
#include "RenderTextureSceneEffect.h"
#include "DrawQuadToTextureSceneEffect.h"

CSceneEffectManager::CSceneEffectManager(void) : m_pShadowMapEffect(0)
{
  SetOk(true);
}


void CSceneEffectManager::Release()
{

  m_pShadowMapEffect = 0;

  m_vRenderSceneEffects.clear();
  m_vPreRenderSceneEffects.clear();
  m_vCaptureFrameBufferSceneEffects.clear();
  m_vCaptureFrameBufferSceneEffectsAfterPostRender.clear();
  m_vPostRenderSceneEffects.clear();

  CMapManager::Release();

}
  
bool CSceneEffectManager::Load(const string &_szFileName)
{
  m_szFileName = _szFileName;
  return Load();
}

bool CSceneEffectManager::Load()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CSceneEffectManager::Load() Carregant el fitxer \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_treeSceneEffects;
  if(!l_treeSceneEffects.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load() No s'ha trobat el XML.");
    SetOk(false);
    Release();
    return IsOk();
  }

  int l_iNumChildren = l_treeSceneEffects.GetNumChildren();

  string l_szName;
  CSceneEffect* l_pSceneEffect = 0;

  for(int i = 0; i < l_iNumChildren; i++)
  {
    CXMLTreeNode l_treeSceneEffect = l_treeSceneEffects(i);

    if(!l_treeSceneEffect.ExistsProperty("name"))
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load Skipping SceneEffect with no name property.");
      continue;
    }

    l_szName = l_treeSceneEffect.GetPszISOProperty("name","",false);

    if(l_szName.compare("") == 0)
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load Skipping SceneEffect with no name property.");
      continue;
    }

    if(GetResource(l_szName) != 0)
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load SceneEffect \"%s\" already exsists.",l_szName.c_str());
      continue;
    }

    if(string(l_treeSceneEffect.GetName()).compare("pre_render") == 0)
    {
      //Pre-render
      string l_szType = l_treeSceneEffect.GetPszISOProperty("type", "");
        
      if(l_szType == "shadow_map_render_to_texture")
      {
        l_pSceneEffect = new CShadowMapRenderToTexture();
        m_pShadowMapEffect = (CShadowMapRenderToTexture*)l_pSceneEffect;
      } else if(l_szType == "render_to_texture")
      {
        l_pSceneEffect = new CRenderToTextureSceneEffect();
      } else if(l_szType == "draw_quad_to_texture")
      {
        l_pSceneEffect = new CDrawQuadToTextureSceneEffect();
      } else
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  type de pre-render incorrecte o no trobat \"%s\".", l_szType.c_str());
        continue;
      }

      if(!l_pSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pSceneEffect;
      } else {
        AddResource(l_szName,l_pSceneEffect);
        m_vPreRenderSceneEffects.push_back(l_pSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"render") == 0)
    {
      //Render
      l_pSceneEffect = new CSceneEffect();

      if(!l_pSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pSceneEffect;
      } else {
        AddResource(l_szName,l_pSceneEffect);
        m_vRenderSceneEffects.push_back(l_pSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"capture_frame_buffer") == 0)
    {
      //capture_frame_buffer
      CRenderTextureSceneEffect* l_pRenderTextureSceneEffect = new CRenderTextureSceneEffect();

      if(!l_pRenderTextureSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pRenderTextureSceneEffect;
      } else {
        AddResource(l_szName,l_pRenderTextureSceneEffect);
        m_vCaptureFrameBufferSceneEffects.push_back(l_pRenderTextureSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"capture_frame_buffer_post_fx") == 0)
    {
      //capture_frame_buffer_post_fx
      CRenderTextureSceneEffect* l_pRenderTextureSceneEffect = new CRenderTextureSceneEffect();

      if(!l_pRenderTextureSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pRenderTextureSceneEffect;
      } else {
        AddResource(l_szName,l_pRenderTextureSceneEffect);
        m_vCaptureFrameBufferSceneEffectsAfterPostRender.push_back(l_pRenderTextureSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"post_render") == 0)
    {
      //post_render
      CDrawQuadSceneEffect* l_pDrawQuad = new CDrawQuadSceneEffect();

      if(!l_pDrawQuad->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pDrawQuad;
      } else {
        AddResource(l_szName,l_pDrawQuad);
        m_vPostRenderSceneEffects.push_back(l_pDrawQuad);
      }
    } else if(!l_treeSceneEffect.IsComment())
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Element no reconegut a l'xml.", l_treeSceneEffect.GetName());
    }

  }
  return IsOk();
}
  
void CSceneEffectManager::PreRender(CRenderManager* _pRM, CProcess* _pProc)
{
  for(size_t i=0; i < m_vPreRenderSceneEffects.size() ; i++)
  {
    if(m_vPreRenderSceneEffects[i]->IsActive())
      m_vPreRenderSceneEffects[i]->PreRender(_pRM,_pProc);
  }
}
  
void CSceneEffectManager::ActivateRenderSceneEffects()
{
  for(size_t i=0; i < m_vRenderSceneEffects.size() ; i++)
  {
    if(m_vRenderSceneEffects[i]->IsActive())
      m_vRenderSceneEffects[i]->ActivateTextures();
  }
}
  
void CSceneEffectManager::CaptureFrameBuffers(CRenderManager* _pRM)
{
  for(size_t i=0; i < m_vCaptureFrameBufferSceneEffects.size() ; i++)
  {
    if(m_vCaptureFrameBufferSceneEffects[i]->IsActive())
      m_vCaptureFrameBufferSceneEffects[i]->CaptureFrameBuffers(_pRM);
  }
  //CaptureFrameBuffersAfterPostRender(_pRM);
}
  
void CSceneEffectManager::PostRender(CRenderManager* _pRM)
{
  
  for(size_t i=0; i < m_vPostRenderSceneEffects.size() ; i++)
  {
    if(m_vPostRenderSceneEffects[i]->IsActive())
      m_vPostRenderSceneEffects[i]->PostRender(_pRM);
  }
  
}
  
void CSceneEffectManager::CaptureFrameBuffersAfterPostRender(CRenderManager* _pRM)
{
  for(size_t i=0; i < m_vCaptureFrameBufferSceneEffectsAfterPostRender.size() ; i++)
  {
    if(m_vCaptureFrameBufferSceneEffectsAfterPostRender[i]->IsActive())
      m_vCaptureFrameBufferSceneEffectsAfterPostRender[i]->CaptureFrameBuffers(_pRM);
  }
}

void CSceneEffectManager::SetShadowMapLightCast(CLight* _pLight)
{
  if(m_pShadowMapEffect)
    m_pShadowMapEffect->SetShadowMapLightCast(_pLight);
};

