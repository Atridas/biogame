#define __DONT_INCLUDE_MEM_LEAKS__
#include "ViewerProcess.h"
#include "Core.h"
#include "EffectManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "RenderableObjectsManager.h"
#include "LightManager.h"

#include "ScriptManager.h"
#include "Viewer.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Utils/MemLeaks.h"

using namespace luabind;

CViewer* GetViewer(void)
{
  return CViewer::GetSingletonPtr();
}

bool CViewerProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Init");

  RENDER_MANAGER->SetClearColor(colMAGENTA);

  //m_pSceneEffectManager = CORE->GetSceneEffectManager();

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

  m_pCamera = m_pViewer->GetCamera();
}

//void CViewerProcess::RenderScene(CRenderManager* _pRM)
//{
//  CORE->GetRenderableObjectsManager()->Render(_pRM);
//
//  if(m_pViewer->GetRenderLights())
//    CORE->GetLightManager()->Render(_pRM);
//
//  m_pViewer->Debug(_pRM);
//}

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

  module(l_LuaState)
  [
    //CViewerProcess
    //luabind::class_<CViewerProcess,CProcess>("ViewerProcess")
    //  .def("reset_viewer"  ,&CViewerProcess::ResetViewer)
    //  .def("get_viewer"    ,&CViewerProcess::GetViewer)
    //  .def("toggle_normals",&CViewerProcess::ToggleNormalRendering)
    //,

    //CViewer
    def("get_viewer", &GetViewer)
    ,
    class_<CViewer>("Viewer")
      .enum_("MODE")
      [
        value("FREE",    CViewer::FREE_MODE),
        value("MESH",    CViewer::MESH_MODE),
        value("ANIMATED",CViewer::ANIMATED_MODE)
      ]
      .enum_("MATERIAL")
      [
        value("SPECULAR",     CViewer::SPECULAR),
        value("GLOSSINESS",   CViewer::GLOSSINESS),
        value("GLOW",         CViewer::GLOW)
      ]
      //ACTIONS
      .def("get_current_mode",    &CViewer::GetCurrentMode)
      //.def("set_mode",            &CViewer::SetMode)
      //.def("activate_gui",        &CViewer::ActivateGui)
      //.def("is_gui_active",       &CViewer::IsGuiActive)
      //.def("toggle_boxes",        &CViewer::ToggleShowBoxes)
      //.def("toggle_spheres",      &CViewer::ToggleShowSpheres)
      .def("next_mode",           &CViewer::SetNextMode)
      .def("next_material",       &CViewer::NextMaterialProperty)
      .def("get_material",        &CViewer::GetCurrentMaterialProperty)
      .def("toggle_info",         &CViewer::ToggleInfo)
      .def("info_active",         &CViewer::GetInfoActive)
      .def("glow_inc_mesh",       &CViewer::IncrementGlowMesh)
      .def("glow_dec_mesh",       &CViewer::DecrementGlowMesh)
      .def("glow_inc_anim",       &CViewer::IncrementGlowAnimated)
      .def("glow_dec_anim",       &CViewer::DecrementGlowAnimated)
      .def("spec_inc_mesh",       &CViewer::IncrementSpecMesh)
      .def("spec_dec_mesh",       &CViewer::DecrementSpecMesh)
      .def("spec_inc_anim",       &CViewer::IncrementSpecAnimated)
      .def("spec_dec_anim",       &CViewer::DecrementSpecAnimated)
      .def("gloss_inc_mesh",      &CViewer::IncrementGlossMesh)
      .def("gloss_dec_mesh",      &CViewer::DecrementGlossMesh)
      .def("gloss_inc_anim",      &CViewer::IncrementGlossAnimated)
      .def("gloss_dec_anim",      &CViewer::DecrementGlossAnimated)
      //.def("bump_inc_mesh",       &CViewer::IncrementBumpMesh)
      //.def("bump_dec_mesh",       &CViewer::DecrementBumpMesh)
      //.def("bump_inc_anim",       &CViewer::IncrementBumpAnimated)
      //.def("bump_dec_anim",       &CViewer::DecrementBumpAnimated)
      //MESH
      .def("next_mesh",           &CViewer::SelectNextMesh)
      .def("previous_mesh",       &CViewer::SelectPrevMesh)
      //ANIMATED
      .def("next_model",          &CViewer::SelectNextAnimatedModel)
      .def("previous_model",      &CViewer::SelectPrevAnimatedModel)
      .def("next_animation",      &CViewer::SetNextAnimation)
      .def("previous_animation",  &CViewer::SetPrevAnimation)
    ];

  //CORE->GetScriptManager()->RunCode("VIEWER_PROCESS = ENGINE:get_active_process()");
  //CORE->GetScriptManager()->RunCode("VIEWER         = VIEWER_PROCESS:get_viewer()");
}

void CViewerProcess::ResetViewer() 
{
  if(m_pViewer)
  {
    m_pViewer->Reset(); 
    m_pCamera = m_pViewer->GetCamera();
  }
}