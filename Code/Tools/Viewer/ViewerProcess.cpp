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

  RENDER_MANAGER->SetClearColor(colMAGENTA);

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
  m_pViewer->Update(_fElapsedTime);
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
      .def("activate_gui",        &CViewer::ActivateGui)
      .def("is_gui_active",        &CViewer::IsGuiActive)
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
    CEffect* l_pStaticMeshEffect = CORE->GetEffectManager()->GetEffect("ShowNormals");
    CEffect* l_pAnimatedMeshEffect = CORE->GetEffectManager()->GetEffect("AnimatedShowNormals");
    CORE->GetEffectManager()->SetForcedStaticMeshEffect(l_pStaticMeshEffect);
    CORE->GetEffectManager()->SetForcedAnimatedModelEffect(l_pAnimatedMeshEffect);
  }else{
    CORE->GetEffectManager()->SetForcedStaticMeshEffect(0);
    CORE->GetEffectManager()->SetForcedAnimatedModelEffect(0);
  }
}