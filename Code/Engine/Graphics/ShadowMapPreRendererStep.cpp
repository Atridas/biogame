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

void CShadowMapPreRendererStep::SetViewProjectionMatrices(CRenderManager* _pRM)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();

  const Vect3f &l_Position = m_pLightShadowCast->GetPosition();
  CDirectionalLight* l_pDirLight = ((CDirectionalLight *)m_pLightShadowCast); 
  l_pDirLight->SetFPSCamera(m_FPSCamera);
  m_pCamera = &m_FPSCamera;
  //Vect3f l_vDirection = l_pDirLight->GetDirection();

  //Mat44f l_LightViewMatrix = l_pDirLight->GetLightViewMatrix();
  //Mat44f l_LightProjectionMatrix = l_pDirLight->GetLightProjectionMatrix();
  //Vect3f l_LightPosition = l_pDirLight->GetPosition();
  //Vect3f l_LightRight = Vect3f(0,1,0) ^ (l_pDirLight->GetDirection()).GetNormalized();
  //
  //if(l_LightRight.SquaredLength() < 0.05f)
  //{
  //  l_LightRight = (l_pDirLight->GetDirection() ^ Vect3f(0,0,1)).GetNormalized();
  //}else{
  //  l_LightRight.Normalize();
  //}
  //
  //Vect3f l_LightUp = (l_pDirLight->GetDirection() ^ l_LightRight).GetNormalized();
  //
  ////_pRM->SetViewMatrix(l_LightViewMatrix);
  ////_pRM->SetProjectionMatrix(l_LightProjectionMatrix);
  //
  //l_pEM->ActivateCamera(l_LightViewMatrix,l_LightProjectionMatrix,l_LightPosition,l_LightUp,l_LightRight);
  //l_pEM->SetShadowProjectionMatrix(l_LightProjectionMatrix);
  //l_pEM->SetLightViewMatrix(l_LightViewMatrix);
  
  Mat44f l_matView;
  Mat44f l_matProjection;
  
  Vect3f l_vEye;
  Vect3f l_vUp;
  Vect3f l_vRight;
  Vect3f l_vLookat;

  l_vEye = m_pCamera->GetEye();
  l_vUp  = m_pCamera->GetVecUp().GetNormalized();
	l_vLookat = m_pCamera->GetLookAt();
  l_vRight = (l_vUp ^ (l_vLookat - l_vEye)).GetNormalized();

  l_matView = _pRM->GetLookAtMatrix(l_vEye,l_vLookat,l_vUp);

  l_matProjection = _pRM->GetPerspectiveFOVMatrix(m_pCamera->GetFov(),m_pCamera->GetAspectRatio(),m_pCamera->GetZn(),m_pCamera->GetZf());
  l_pEM->ActivateCamera(l_matView,l_matProjection,l_vEye,l_vUp,l_vRight);
  l_pEM->SetShadowProjectionMatrix(l_matProjection);
  l_pEM->SetLightViewMatrix(l_matView);
}

void CShadowMapPreRendererStep::Render(CRenderManager* _pRM, CCamera* _pCamera)
{
  if(m_pLightShadowCast != 0)
  {
    if(m_pLightShadowCast->GetRenderShadows())
    {
      CSceneRendererStep::Render(_pRM,_pCamera);
    }
  }
}
