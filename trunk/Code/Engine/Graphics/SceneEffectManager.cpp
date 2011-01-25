#include "SceneEffectManager.h"
#include "Process.h"
#include "RenderManager.h"
#include "SceneEffect.h"
#include "XML/XMLTreeNode.h"

#include "DrawQuadSceneEffect.h"
#include "ShadowMapRenderToTexture.h"
#include "RenderToTextureSceneEffect.h"
#include "RenderTextureSceneEffect.h"

CSceneEffectManager::CSceneEffectManager(void)
{
  SetOk(true);
}


void CSceneEffectManager::Release()
{
  vector<CSceneEffect*>::iterator l_begin, l_end, l_it;

  l_begin = m_vRenderSceneEffects.begin();
  l_end   = m_vRenderSceneEffects.end();
  for(l_it = l_begin; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  m_vRenderSceneEffects.clear();

  l_begin = m_vPreRenderSceneEffects.begin();
  l_end   = m_vPreRenderSceneEffects.end();
  for(l_it = l_begin; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  m_vPreRenderSceneEffects.clear();

  l_begin = m_vCaptureFrameBufferSceneEffects.begin();
  l_end   = m_vCaptureFrameBufferSceneEffects.end();
  for(l_it = l_begin; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  m_vCaptureFrameBufferSceneEffects.clear();

  l_begin = m_vCaptureFrameBufferSceneEffectsAfterPostRender.begin();
  l_end   = m_vCaptureFrameBufferSceneEffectsAfterPostRender.end();
  for(l_it = l_begin; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  m_vCaptureFrameBufferSceneEffectsAfterPostRender.clear();

  l_begin = m_vPostRenderSceneEffects.begin();
  l_end   = m_vPostRenderSceneEffects.end();
  for(l_it = l_begin; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  m_vPostRenderSceneEffects.clear();
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
  for(int i = 0; i < l_iNumChildren; i++)
  {
    CXMLTreeNode l_treeSceneEffect = l_treeSceneEffects(i);
    if(strcmp(l_treeSceneEffect.GetName(),"pre_render") == 0)
    {
      //Pre-render
      string l_szType = l_treeSceneEffect.GetPszISOProperty("type", "");
      CSceneEffect* l_pSceneEffect = 0;
      if(l_szType == "shadow_map_render_to_texture")
      {
        l_pSceneEffect = new CShadowMapRenderToTexture();
      } else if(l_szType == "render_to_texture")
      {
        l_pSceneEffect = new CRenderToTextureSceneEffect();
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
        m_vPreRenderSceneEffects.push_back(l_pSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"render") == 0)
    {
      //Render
      CSceneEffect * l_pSceneEffect = new CSceneEffect();
      if(!l_pSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pSceneEffect;
      } else {
        m_vRenderSceneEffects.push_back(l_pSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"capture_frame_buffer") == 0)
    {
      //capture_frame_buffer
      CRenderTextureSceneEffect * l_pRenderTextureSceneEffect = new CRenderTextureSceneEffect();
      if(!l_pRenderTextureSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pRenderTextureSceneEffect;
      } else {
        m_vCaptureFrameBufferSceneEffects.push_back(l_pRenderTextureSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"capture_frame_buffer_post_fx") == 0)
    {
      //capture_frame_buffer_post_fx
      CRenderTextureSceneEffect * l_pRenderTextureSceneEffect = new CRenderTextureSceneEffect();
      if(!l_pRenderTextureSceneEffect->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pRenderTextureSceneEffect;
      } else {
        m_vCaptureFrameBufferSceneEffectsAfterPostRender.push_back(l_pRenderTextureSceneEffect);
      }
    } else if(strcmp(l_treeSceneEffect.GetName(),"post_render") == 0)
    {
      //post_render
      CDrawQuadSceneEffect * l_pDrawQuad = new CDrawQuadSceneEffect();
      if(!l_pDrawQuad->Init(l_treeSceneEffect))
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load  Scene effect incorrecte \"%s\".", l_treeSceneEffect.GetName());
        delete l_pDrawQuad;
      } else {
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
    m_vPreRenderSceneEffects[i]->PreRender(_pRM,_pProc);
  }
}
  
void CSceneEffectManager::ActivateRenderSceneEffects()
{
  for(size_t i=0; i < m_vRenderSceneEffects.size() ; i++)
  {
    m_vRenderSceneEffects[i]->ActivateTextures();
  }
}
  
void CSceneEffectManager::CaptureFrameBuffers(CRenderManager* _pRM)
{
  for(size_t i=0; i < m_vCaptureFrameBufferSceneEffects.size() ; i++)
  {
    m_vCaptureFrameBufferSceneEffects[i]->CaptureFrameBuffers(_pRM);
  }
  CaptureFrameBuffersAfterPostRender(_pRM);
}
  
void CSceneEffectManager::PostRender(CRenderManager* _pRM)
{
  
  for(size_t i=0; i < m_vPostRenderSceneEffects.size() ; i++)
  {
    m_vPostRenderSceneEffects[i]->PostRender(_pRM);
  }
  
}
  
void CSceneEffectManager::CaptureFrameBuffersAfterPostRender(CRenderManager* _pRM)
{
  for(size_t i=0; i < m_vCaptureFrameBufferSceneEffectsAfterPostRender.size() ; i++)
  {
    m_vCaptureFrameBufferSceneEffectsAfterPostRender[i]->CaptureFrameBuffers(_pRM);
  }
}

