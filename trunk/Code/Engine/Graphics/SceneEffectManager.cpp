#include "SceneEffectManager.h"
#include "Process.h"
#include "RenderManager.h"
#include "SceneEffect.h"
#include "XML/XMLTreeNode.h"

CSceneEffectManager::CSceneEffectManager(void)
{
  SetOk(true);
}


void CSceneEffectManager::Release()
{
  m_vRenderSceneEffects.clear();
  m_vPreRenderSceneEffects.clear();
  m_vCaptureFrameBufferSceneEffects.clear();
  m_vCaptureFrameBufferSceneEffectsAfterPostRender.clear();
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
    LOGGER->AddNewLog(ELL_WARNING,"CSceneEffectManager::Load() No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    return false;
  }

  if(strcmp(l_treeSceneEffects.GetName(),"scene_effects") == 0)
  {

    //--------Pre_Render-------------
    CXMLTreeNode l_treePreRender = l_treeSceneEffects["pre_render"];
    int l_iNumChildren = l_treePreRender.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneEffectManager::Load() Loading %d effects.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treePreRender = l_treePreRender(i);
      if(l_treePreRender.IsComment())
        continue;
      
      //int l_iVertexType = l_treePreRender.GetIntProperty("vertex_type");
      //string l_szTechniqueName = l_treePreRender.GetPszISOProperty("technique","");
      string l_szType = l_treePreRender.GetPszISOProperty("type","");
      string l_szName = l_treePreRender.GetPszISOProperty("name","");
      string l_szTexture = l_treePreRender.GetPszISOProperty("texture","");
      int l_width = l_treePreRender.GetIntProperty("width");
      int l_iheight = l_treePreRender.GetIntProperty("height");
      string l_szStatic_mesh_technique = l_treePreRender.GetPszISOProperty("static_mesh_technique","");
      string l_szAnimated_model_technique = l_treePreRender.GetPszISOProperty("animated_model_technique","");
      string l_szLight_shadow_cast = l_treePreRender.GetPszISOProperty("light_shadow_cast","");
      bool l_bActive = l_treePreRender.GetBoolProperty("active");
      string l_szformat_type = l_treePreRender.GetPszISOProperty("format_type","");

    }
  }
  return true;
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

