#include "ShadowMapPreRendererStep.h"
#include "Process.h"
#include "Core.h"
#include "EffectManager.h"
#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "RenderManager.h"

bool CShadowMapPreRendererStep::Init(CXMLTreeNode& _treePreSceneRenderer)
{
  SetOk(CPreSceneRendererStep::Init(_treePreSceneRenderer));
  return IsOk();
}

void CShadowMapPreRendererStep::Release()
{
  CSceneRendererStep::Release();
  m_pLightShadowCast = 0;
}

void CShadowMapPreRendererStep::Render(CProcess* _pProcess)
{
  //Render ShadowMap
  if(m_pLightShadowCast != 0)
  {
    if(m_pLightShadowCast->GetRenderShadows())
    {
      //Nos debemos guardar las matrices de View y de Proyección actualmente activas en el EffectManager
      const Vect3f &l_Position=m_pLightShadowCast->GetPosition();
      CDirectionalLight* l_pDirLight = ((CDirectionalLight *)m_pLightShadowCast); 
      Vect3f l_VDir= l_pDirLight->GetDirection();

      CEffectManager* l_pEM = CORE->GetEffectManager();
      Mat44f l_LightViewMatrix = l_pDirLight->GetLightViewMatrix();
      Mat44f l_LightProjectionMatrix = l_pDirLight->GetLightProjectionMatrix();
      Vect3f l_LightPosition = l_pDirLight->GetPosition();
      Vect3f l_LightRight       = Vect3f(0,1,0) ^ (l_pDirLight->GetDirection()).GetNormalized();
      if(l_LightRight.SquaredLength() < 0.05f)
      {
        l_LightRight       = (l_pDirLight->GetDirection() ^ Vect3f(0,0,1)).GetNormalized();
      }
      else
      {
        l_LightRight.Normalize();
      }
      Vect3f l_LightUp = (l_pDirLight->GetDirection() ^ l_LightRight).GetNormalized();

      
      l_pEM->ActivateCamera(l_LightViewMatrix, l_LightProjectionMatrix, l_LightPosition,l_LightUp,l_LightRight);
      l_pEM->SetShadowProjectionMatrix(l_LightProjectionMatrix);
      l_pEM->SetLightViewMatrix(l_LightViewMatrix);

      ActivateInputSamplers();
      ActivateRenderTargets();

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
  }
}
