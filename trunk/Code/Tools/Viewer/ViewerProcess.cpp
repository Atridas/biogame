#include "ViewerProcess.h"
#include "Core.h"
#include "EffectManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "RenderableObjectsManager.h"

#include "LightManager.h"


bool CViewerProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Init");

  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  m_pViewer = new CViewer();
  
  if(m_pViewer)
  {
    m_pCamera = m_pViewer->GetCamera();
    SetOk(true);
  }else{
    SetOk(false);
  }

  return IsOk();
}

void CViewerProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Release");

  CHECKED_DELETE(m_pViewer)
}

void CViewerProcess::Update(float _fElapsedTime)
{
  Vect3i l_vMouseDelta = INPUT_MANAGER->GetMouseDelta();
  m_pViewer->Update(_fElapsedTime,l_vMouseDelta);
}

void CViewerProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);

  if(m_pViewer->GetRenderLights())
    CORE->GetLightManager()->Render(_pRM);

  //m_pViewer->Debug(_pRM);
}

bool CViewerProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "ToggleNormalRendering") == 0)
  {
    m_pViewer->ToggleNormalRendering();

    if(m_pViewer->GetNormalRendering())
    {
      m_pStaticMeshTechnique = CORE->GetEffectManager()->GetEffectTechnique("ShowNormalsTechnique");
      m_pAnimatedTechnique = CORE->GetEffectManager()->GetEffectTechnique("Cal3dShowNormalsTechnique");
    }else{
      m_pStaticMeshTechnique = 0;
      m_pAnimatedTechnique = 0;
    }

    return true;
  }

  return m_pViewer->ExecuteAction(_fDeltaSeconds,_fDelta,_pcAction);

  return false;
}


void CViewerProcess::RenderINFO(CRenderManager* _pRM)
{
  m_pViewer->ShowInfo();
}
