#include "SceneRendererStep.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Material.h"
#include "Camera.h"
#include "RenderableObjectsManager.h"
#include "RenderableAnimatedInstanceModel.h"
#include "cal3d/cal3d.h"
#include "PortalManager.h"

bool CSceneRendererStep::Init(CXMLTreeNode& _treeSceneRenderer, const string& _szDefaultRenderTarget)
{
  //string l_szName = _treeSceneRenderer.GetPszISOProperty("name","",false);
  //string l_szRenderTarget = _treeSceneRenderer.GetPszISOProperty("render_target","",false);

  if(!CRendererStep::Init(_treeSceneRenderer,_szDefaultRenderTarget))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init SceneRenderer sense nom");
    SetOk(false);
  }else{
    
    LOGGER->AddNewLog(ELL_INFORMATION,"CSceneRendererStep::Init iniciant %s",GetName().c_str());

    //CRendererStep::Init(l_szRenderTarget);

    CXMLTreeNode l_treeSamplers = _treeSceneRenderer.GetChild("input_samplers");
    CXMLTreeNode l_treeMaterialEffects = _treeSceneRenderer.GetChild("material_effects");

    if(!InitInputSamplers(l_treeSamplers))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init error inicialitzant input_samplers");
      SetOk(false);
    }else if(!InitMaterialEffects(l_treeMaterialEffects))
    {
      LOGGER->AddNewLog(ELL_ERROR,"CSceneRendererStep::Init error inicialitzant els material_effects");
      SetOk(false);
    }else{

      //SetName(l_szName);

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

  m_szParticleEffect = _treeMaterialEffects.GetPszISOProperty("particle_effect","",false);

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

void CSceneRendererStep::Render(CRenderManager* _pRM, CCamera* _pCamera)
{
  //m_pCamera = _pCamera;

  CEffectManager* l_pEM = CORE->GetEffectManager();
  l_pEM->Begin();

  ActivateInputSamplers();
  //ActivateRenderTargets(_pRM);

  SetViewProjectionMatrices(_pRM, _pCamera);
  
  RenderScene(_pRM, _pCamera);
  
  //DeactivateRenderTargets(_pRM);
  DeactivateInputSamplers();
}

void CSceneRendererStep::RenderScene(CRenderManager* _pRM, CCamera* _pCamera)
{
  CPortalManager* l_pPM = CORE->GetPortalManager();

  Vect3f   l_vEye    = _pCamera->GetEye();
  CObject3DOrdering l_Ordering(l_vEye);
  
  CRoom::TBlendQueue l_BlendQueue(l_Ordering);
  CRoom::TBlendQueue l_EmiterQueue(l_Ordering);

  vector<CRenderableObject*> l_vOpaqueObjects;
  l_pPM->GetRenderedObjects(_pCamera, l_vOpaqueObjects, l_BlendQueue, l_EmiterQueue);

  
  CORE->GetEffectManager()->ActivateDefaultRendering();

  
  vector<CRenderableObject*>::iterator l_it  = l_vOpaqueObjects.begin();
  vector<CRenderableObject*>::iterator l_end = l_vOpaqueObjects.end  ();

  for(; l_it != l_end; ++l_it)
  {
    CRenderableObject* l_pRO = *l_it;
    RenderObject3DRenderable(_pRM,l_pRO);
  }
  
  CORE->GetEffectManager()->ActivateAlphaRendering();

  while(!l_BlendQueue.empty())
  {
    CObject3DRenderable* l_pO3DRenderable = l_BlendQueue.top();
    RenderObject3DRenderable(_pRM,l_pO3DRenderable);
    l_BlendQueue.pop();
  }
  
  while(!l_EmiterQueue.empty())
  {
    CObject3DRenderable* l_pEmiter = l_EmiterQueue.top();
    RenderEmiter(_pRM,l_pEmiter);
    l_EmiterQueue.pop();
  }
  
  _pRM->GetDevice()->SetStreamSourceFreq(0, 1);
  _pRM->GetDevice()->SetStreamSourceFreq(1, 1);
}

void CSceneRendererStep::RenderObject3DRenderable(CRenderManager* _pRM, CObject3DRenderable* _pO3DRenderable) const
{
  CEffectManager* l_pEM = CORE->GetEffectManager();

  CEffect* l_pMeshEffect = l_pEM->GetResource(m_szStaticMeshEffect);
  CEffect* l_pModelEffect = l_pEM->GetResource(m_szAnimatedModelEffect);

  vector<CEffect*> l_vEffects;
  const vector<CMaterial*>& l_vMaterials = _pO3DRenderable->GetMaterials();
  vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

  for(;l_itMaterial != l_vMaterials.end(); ++l_itMaterial)
  {
    CEffect* l_pEffect = 0;
    CMaterial* l_pMaterial = *l_itMaterial;
    int l_iMaterialType = l_pMaterial->GetMaterialType();
    bool l_bAnimatedMesh = (l_iMaterialType & ANIMATED_MESH_MATERIAL_MASK) > 0;

    if(!m_bUseMap)
    { 
      if(l_bAnimatedMesh)
      {
        l_pEffect = l_pModelEffect;
      }else{
        l_pEffect = l_pMeshEffect;
      }
      
    }else{

      map<int,string>::const_iterator l_it = m_mapMaterialEffects.find(l_iMaterialType);

      if(l_it != m_mapMaterialEffects.end())
        l_pEffect = l_pEM->GetResource(l_it->second);

      if(!l_pEffect)
        l_pEffect = l_pEM->GetResource("White");
    }


    l_vEffects.push_back(l_pEffect);
  }
  
  l_pEM->SetWorldMatrix(_pO3DRenderable->GetMat44());

  _pO3DRenderable->Render(_pRM,l_vEffects);
}


void CSceneRendererStep::RenderEmiter(CRenderManager* _pRM, CObject3DRenderable* _pO3DRenderable) const
{
  
  if(m_szParticleEffect == "")
  {
    return;
  }
  
  CEffectManager* l_pEM = CORE->GetEffectManager();

  CEffect* l_pParticleEffect = l_pEM->GetResource(m_szParticleEffect);
  if(!l_pParticleEffect)
    l_pParticleEffect = l_pEM->GetResource("White");

  vector<CEffect*> l_vEffects;
  l_vEffects.push_back(l_pParticleEffect);
  
  l_pEM->SetWorldMatrix(_pO3DRenderable->GetMat44());

  _pO3DRenderable->Render(_pRM,l_vEffects);
}
