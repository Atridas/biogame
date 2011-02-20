#include "ViewerProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "StaticMesh.h"
#include "StaticMeshManager.h"
#include "RenderableObjectsManager.h"
#include "AnimatedModelManager.h"
#include "RenderableObject.h"
#include "AnimatedInstanceModel.h"
#include "RenderableAnimatedInstanceModel.h"

#include <IndexedVertexs.h>
#include "VertexsStructs.h"

#include <LightManager.h>
#include <sstream>
#include "SpotLight.h"

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
}

bool CViewerProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  return m_pViewer->ExecuteAction(_fDeltaSeconds,_fDelta,_pcAction);

  return false;
}


void CViewerProcess::RenderINFO(CRenderManager* _pRM)
{
  m_pViewer->ShowInfo();
}
