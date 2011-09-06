#include "SceneRendererStep.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Process.h"

bool CSceneRendererStep::Init(CXMLTreeNode& _treeSceneRenderer)
{
  string l_szName = _treeSceneRenderer.GetPszISOProperty("name","",false);

  if(l_szName == "")
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init SceneRenderer sense nom");
    SetOk(false);
  }else{
    
    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneRendererStep::Init iniciant %s",l_szName.c_str());

    CXMLTreeNode l_treeSamplers = _treeSceneRenderer.GetChild("input_samplers");
    CXMLTreeNode l_treeRenderTargets = _treeSceneRenderer.GetChild("render_targets");
    CXMLTreeNode l_treeMaterialEffects = _treeSceneRenderer.GetChild("material_effects");

    if(!InitInputSamplers(l_treeSamplers))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init error inicialitzant input_samplers");
      SetOk(false);
    }else if(!InitRenderTargets(l_treeRenderTargets))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init error inicialitzant render_targets");
      SetOk(false);
    }else if(!InitMaterialEffects(l_treeMaterialEffects))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init error inicialitzant els material_effects");
      SetOk(false);
    }else{

      SetName(l_szName);

      SetOk(true);
    }
  }

  return IsOk();
}

void CSceneRendererStep::Release()
{
  m_mapMaterialEffects.clear();
  m_szStaticMeshEffect = "";
  m_szAnimatedModelEffect = "";
  m_bUseMap = false;

  CRendererStep::Release();
}

bool CSceneRendererStep::InitMaterialEffects(CXMLTreeNode& _treeMaterialEffects)
{
  if(!_treeMaterialEffects.Exists())
    return false;

  if(_treeMaterialEffects.ExistsProperty("static_mesh_effect") && _treeMaterialEffects.ExistsProperty("animated_model_effect"))
  {
    m_szStaticMeshEffect = _treeMaterialEffects.GetPszISOProperty("static_mesh_effect","",false);
    m_szAnimatedModelEffect = _treeMaterialEffects.GetPszISOProperty("animated_model_effect","",false);

    m_bUseMap = false;

  }else{

    m_bUseMap = true;

    int l_iNumRenderTargets = _treeMaterialEffects.GetNumChildren();

    for(int l_iIndex = 0; l_iIndex < l_iNumRenderTargets;l_iIndex++)
    {
      CXMLTreeNode l_pMaterialEffect = _treeMaterialEffects(l_iIndex);

      if(string(l_pMaterialEffect.GetName()) == "material_effect")
      {
        int l_iMaterialType = l_pMaterialEffect.GetIntProperty("material_type",0,false);
        string l_szEffectName = l_pMaterialEffect.GetPszISOProperty("effect","",false);

        if(l_szEffectName != "")
        {
          m_mapMaterialEffects[l_iMaterialType] = l_szEffectName;
        }else{
          LOGGER->AddNewLog(ELL_WARNING,"CSceneRendererStep::InitMaterialEffects material type %d amb effecte null",l_iMaterialType);
        }

      }else if(!l_pMaterialEffect.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING,"CSceneRendererStep::InitMaterialEffects element no reconegut %s",l_pMaterialEffect.GetName());
      }

    }
  }

  return true;
}

void CSceneRendererStep::Render(CProcess* _pProcess)
{
  ActivateInputSamplers();
  ActivateRenderTargets();

  CEffectManager* l_pEM = CORE->GetEffectManager();

  if(!m_bUseMap)
  {  
    CEffect* l_pMeshEffect = l_pEM->GetResource(m_szStaticMeshEffect);
    CEffect* l_pModelEffect = l_pEM->GetResource(m_szAnimatedModelEffect);
    l_pEM->SetForcedStaticMeshEffect(l_pMeshEffect);
    l_pEM->SetForcedAnimatedModelEffect(l_pModelEffect);
  }else{
    l_pEM->SetForcedStaticMeshEffect(0);
    l_pEM->SetForcedAnimatedModelEffect(0);
  }

  CRenderManager* l_pRM = RENDER_MANAGER;

  l_pRM->SetupMatrices(_pProcess->GetCamera());
  _pProcess->RenderScene(l_pRM);
  
  DeactivateRenderTargets();
  DeactivateInputSamplers();
}