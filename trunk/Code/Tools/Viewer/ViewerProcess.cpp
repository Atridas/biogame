#define __DONT_INCLUDE_MEM_LEAKS__
#include "ViewerProcess.h"
#include "Core.h"
#include "EffectManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "RenderableObjectsManager.h"

#include "LightManager.h"

#include "ScriptManager.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Utils/MemLeaks.h"


bool CViewerProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Init");

  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  m_pViewer = new CViewer();
  
  if(m_pViewer)
  {
    m_pViewer->Init();
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
  return m_pViewer->ExecuteAction(_fDeltaSeconds,_fDelta,_pcAction);
}


void CViewerProcess::RenderINFO(CRenderManager* _pRM)
{
  m_pViewer->ShowInfo();
}


void CViewerProcess::RegisterLuaFunctions()
{
  lua_State* l_LuaState = CORE->GetScriptManager()->GetLuaState();

  luabind::module(l_LuaState)
  [
    //CViewerProcess
    luabind::class_<CViewerProcess,CProcess>("ViewerProcess")
      .def("reset_viewer",  &CViewerProcess::ResetViewer)
      .def("get_viewer",    &CViewerProcess::GetViewer)
      .def("toggle_normals",&CViewerProcess::ToggleNormalRendering)
    ,
    //CViewer
    luabind::class_<CViewer>("Viewer")
      .enum_("MODE")
      [
        luabind::value("FREE",    CViewer::FREE_MODE),
        luabind::value("MESH",    CViewer::MESH_MODE),
        luabind::value("ANIMATED",CViewer::ANIMATED_MODE)
      ]
      //GUI
      .def("get_current_mode",    &CViewer::GetCurrentMode)
      .def("set_mode",            &CViewer::SetMode)
      .def("get_view_mode",       &CViewer::GetViewMode)
      .def("set_view_mode",       &CViewer::SetViewMode)
      .def("toggle_boxes",        &CViewer::ToggleShowBoxes)
      .def("toggle_spheres",      &CViewer::ToggleShowSpheres)
      .def("next_mode",           &CViewer::SetNextMode)
      //MESH
      .def("next_mesh",           &CViewer::SelectNextMesh)
      .def("previous_mesh",       &CViewer::SelectPrevMesh)
      //ANIMATED
      .def("next_model",          &CViewer::SelectNextAnimatedModel)
      .def("previous_model",      &CViewer::SelectPrevAnimatedModel)
      .def("next_animation",      &CViewer::SetNextAnimation)
      .def("previous_animation",  &CViewer::SetPrevAnimation)
    ];

  CORE->GetScriptManager()->RunCode("VIEWER_PROCESS = ENGINE:get_active_process()");
  CORE->GetScriptManager()->RunCode("VIEWER         = VIEWER_PROCESS:get_viewer()");
}

void CViewerProcess::ResetViewer() 
{
  if(m_pViewer)
  {
    m_pViewer->Reset(); 
    m_pCamera = m_pViewer->GetCamera();
  }
}

void CViewerProcess::ToggleNormalRendering()
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
}