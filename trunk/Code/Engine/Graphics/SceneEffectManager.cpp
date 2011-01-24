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
    int l_iNumChildren = l_treeSceneEffects.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneEffectManager::Load() Loading %d pre_render.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {

      CXMLTreeNode l_treeSceneEffect = l_treeSceneEffects(i);
      if(l_treeSceneEffect.IsComment())
        continue;
      
      string l_szTypePreRender = l_treePreRender.GetPszISOProperty("type","");
      string l_szNamePreRender = l_treePreRender.GetPszISOProperty("name","");
      string l_szTexturePreRender = l_treePreRender.GetPszISOProperty("texture","");
      int l_widthPreRender = l_treePreRender.GetIntProperty("width");
      int l_iheightPreRender = l_treePreRender.GetIntProperty("height");
      string l_szStaticMeshTechniquePreRender = l_treePreRender.GetPszISOProperty("static_mesh_technique","");
      string l_szAnimatedModelTechniquePreRender = l_treePreRender.GetPszISOProperty("animated_model_technique","");
      string l_szLightShadowCastPreRender = l_treePreRender.GetPszISOProperty("light_shadow_cast","");
      bool l_bActivePreRender = l_treePreRender.GetBoolProperty("active");
      string l_szformatTypePreRender = l_treePreRender.GetPszISOProperty("format_type","");

      
      // O es creo un objecta, i es posen les dades llegides dins, no se com
      CSceneEffect* l_pSceneEffect = new CSceneEffect();
    
      //O inserta a sacp la info en el vector
    /*  m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szTypePreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szNamePreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szTexturePreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_widthPreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_iheightPreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szStaticMeshTechniquePreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szAnimatedModelTechniquePreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szLightShadowCastPreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_bActivePreRender);
      m_vPreRenderSceneEffects.insert(m_vPreRenderSceneEffects.end(),l_szformatTypePreRender);*/
    }

    //--------Render-------------
    CXMLTreeNode l_treeRender = l_treeSceneEffects["render"];
    int l_iNumChildrenRender = l_treeRender.GetNumChildren();

    CXMLTreeNode l_treeTextureRender = l_treeRender["texture"];
    int l_iNumChildrenTextureRender = l_treeTextureRender.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneEffectManager::Load() Loading %d render.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildrenRender; i++)
    {
      CXMLTreeNode l_treeRender = l_treeRender(i);
      if(l_treeRender.IsComment())
        continue;
      
      string l_szNameRender = l_treeRender.GetPszISOProperty("name","");
      bool l_bActiveRender = l_treeRender.GetBoolProperty("active");
      
      for(int i = 0; i < l_iNumChildrenTextureRender; i++)
      {
        CXMLTreeNode l_treeTextureRender = l_treeTextureRender(i);
        if(l_treeTextureRender.IsComment())
          continue;
      
        int l_szStageIdRender = l_treeTextureRender.GetIntProperty("stage_id");
        string l_szFileRender = l_treeTextureRender.GetPszISOProperty("file","");
        
      
      }
      
    /*  m_vRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_szNameRender);
      m_vRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_bActiveRender);
     // m_vRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_szStageIdRender);
     // m_vRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_szFileRender);*/
    }
    //----------Texture------------------------------

    //----------Capture Buffer-----------------------
    
    CXMLTreeNode l_treeCaptureBuffer = l_treeSceneEffects["capture_frame_buffer"];
    int l_iNumChildrenCaptureBuffer = l_treeCaptureBuffer.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneEffectManager::Load() Loading %d capture_frame_buffer.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildrenCaptureBuffer; i++)
    {
      CXMLTreeNode l_treeCaptureBuffer = l_treeCaptureBuffer(i);
      if(l_treeCaptureBuffer.IsComment())
        continue;
      
      string l_szNameCaptureBuffer = l_treeCaptureBuffer.GetPszISOProperty("name","");
      string l_szTextureCaptureBuffer = l_treeCaptureBuffer.GetPszISOProperty("texture","");
      string l_szTextureFrameCaptureBuffer = l_treeCaptureBuffer.GetPszISOProperty("texture_width_as_frame_buffer","");
      bool l_bActiveRender = l_treeCaptureBuffer.GetBoolProperty("active");
      string l_szFormatTypeCaptureBuffer = l_treeCaptureBuffer.GetPszISOProperty("format_type","");
      
     /* m_vCaptureFrameBufferSceneEffects.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szNameCaptureBuffer);
      m_vCaptureFrameBufferSceneEffects.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szTextureCaptureBuffer);
      m_vCaptureFrameBufferSceneEffects.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szTextureFrameCaptureBuffer);
      m_vCaptureFrameBufferSceneEffects.insert(m_vCaptureFrameBufferSceneEffects.end(),l_bActiveRender);
      m_vCaptureFrameBufferSceneEffects.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szFormatTypeCaptureBuffer);*/
      
  }

  //----------Capture Buffer PostRender-----------------------
    
    CXMLTreeNode l_treeCaptureBufferPostRender = l_treeSceneEffects["capture_frame_buffer_post_fx"];
    int l_iNumChildrenCaptureBufferPostRender = l_treeCaptureBufferPostRender.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneEffectManager::Load() Loading %d capture_frame_buffer_post_fx.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildrenCaptureBufferPostRender; i++)
    {
      CXMLTreeNode l_treeCaptureBufferPostRender = l_treeCaptureBufferPostRender(i);
      if(l_treeCaptureBufferPostRender.IsComment())
        continue;
      
      string l_szNameCaptureBufferPostRender = l_treeCaptureBufferPostRender.GetPszISOProperty("name","");
      string l_szTextureCaptureBufferPostRender = l_treeCaptureBufferPostRender.GetPszISOProperty("texture","");
      string l_szTextureFrameCaptureBufferPostRender = l_treeCaptureBufferPostRender.GetPszISOProperty("texture_width_as_frame_buffer","");
      bool l_bActiveFrameCaptureBufferPostRender = l_treeCaptureBufferPostRender.GetBoolProperty("active");
      string l_szFormatTypeCaptureBufferPostRender = l_treeCaptureBufferPostRender.GetPszISOProperty("format_type","");
      
    /*  m_vCaptureFrameBufferSceneEffectsAfterPostRender.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szNameCaptureBufferPostRender);
      m_vCaptureFrameBufferSceneEffectsAfterPostRender.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szTextureCaptureBufferPostRender);
      m_vCaptureFrameBufferSceneEffectsAfterPostRender.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szTextureFrameCaptureBufferPostRender);
      m_vCaptureFrameBufferSceneEffectsAfterPostRender.insert(m_vCaptureFrameBufferSceneEffects.end(),l_bActiveFrameCaptureBufferPostRender);
      m_vCaptureFrameBufferSceneEffectsAfterPostRender.insert(m_vCaptureFrameBufferSceneEffects.end(),l_szFormatTypeCaptureBufferPostRender);*/
      
  }

  //--------Post Render-------------
    CXMLTreeNode l_treePostRender = l_treeSceneEffects["post_render"];
    int l_iNumChildrenPostRender = l_treePostRender.GetNumChildren();

    CXMLTreeNode l_treeTexturePostRender = l_treePostRender["texture"];
    int l_iNumChildrenTexturePostRender = l_treeTexturePostRender.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneEffectManager::Load() Loading %d post render.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildrenPostRender; i++)
    {
      CXMLTreeNode l_treePostRender = l_treePostRender(i);
      if(l_treePostRender.IsComment())
        continue;
      
      string l_szTypePostRender = l_treePostRender.GetPszISOProperty("type","");
      string l_szNamePostRender = l_treePostRender.GetPszISOProperty("name","");
      string l_szTechniquePostRender = l_treePostRender.GetPszISOProperty("technique","");
      bool l_bActivePostRender = l_treePostRender.GetBoolProperty("active");
      
      for(int i = 0; i < l_iNumChildrenTexturePostRender; i++)
      {
        CXMLTreeNode l_treeTexturePostRender = l_treeTexturePostRender(i);
        if(l_treeTexturePostRender.IsComment())
          continue;
      
        int l_szStageIdTexturePostRender = l_treeTexturePostRender.GetIntProperty("stage_id");
        string l_szFileTexturePostRender = l_treeTexturePostRender.GetPszISOProperty("file","");
        bool l_bActiveTexturePostRender = l_treeTexturePostRender.GetBoolProperty("active");
      
      }
      
    /*  m_vPostRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_szTypePostRender);
      m_vPostRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_szNamePostRender);
      m_vPostRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_szTechniquePostRender);
      m_vPostRenderSceneEffects.insert(m_vRenderSceneEffects.end(),l_bActivePostRender);*/
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

