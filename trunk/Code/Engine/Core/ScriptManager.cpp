#define __DONT_INCLUDE_MEM_LEAKS__
#include "ScriptManager.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include <XML/XMLTreeNode.h>

#include "Utils/MemLeaks.h"

//lua defined class & structs
#include "Core.h"
#include "Engine.h"
#include "Process.h"
#include "RenderableObjectsManager.h"
#include "InstanceMesh.h"
#include "RenderableAnimatedInstanceModel.h"
#include "StaticMeshManager.h"
#include "LightManager.h"
#include "Light.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "SoundManager.h"
#include "PhysicsManager.h"
#include "LevelChanger.h"
#include "Renderer.h"
#include "AnimatedModelManager.h"

#include "Utils/Object3D.h"
#include "Utils/BaseControl.h"

#include "Console.h"

#include "GUIManager.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "EffectManager.h"

#include "EntitiesToLua.h"
#include "IAToLua.h"


using namespace luabind;

#define REGISTER_LUA_FUNCTION(LUA_STATE,FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}


//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
  m_pLS=lua_open();
  luaopen_base(m_pLS);
  luaopen_string(m_pLS);
  luaopen_table(m_pLS);
  luaopen_math(m_pLS);
  //luaopen_io(m_LS);
  luaopen_debug(m_pLS);
  open(m_pLS);

  RegisterLUAFunctions();
}


void CScriptManager::Load(const string& _szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CScriptManager::Load \"%s\"", _szFileName.c_str());

  CXMLTreeNode l_XMLLua;
  if(!l_XMLLua.LoadFile(_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CScriptManager::Load No s'ha trobat el XML \"%s\"", _szFileName.c_str());
    SetOk(false);
    return;
  }

  m_szFileName = _szFileName;

  //Files
  CXMLTreeNode l_TreeFiles = l_XMLLua["files"];
  if(l_TreeFiles.Exists())
  {
    int l_iNumScriptFiles = l_TreeFiles.GetNumChildren();
    for(int i = 0; i < l_iNumScriptFiles; i++)
    {
      CXMLTreeNode l_LuaFile = l_TreeFiles(i);
      if(l_LuaFile.IsComment()) {
        continue;
      }

      string l_szLuaFile = l_LuaFile.GetPszISOProperty("path","");

      if(l_szLuaFile != "")
      {
        m_vLuaScripts.push_back(l_szLuaFile);
        LOGGER->AddNewLog(ELL_INFORMATION,"CScriptManager::Load Carregant el fitxer \"%s\"", l_szLuaFile.c_str());
        RunFile(l_szLuaFile);
      }
      else
        LOGGER->AddNewLog(ELL_WARNING,"CScriptManager::Load Propietat \"file\" no trobada a linia %d", i);
    }
  }

  //Init Scripts
  CXMLTreeNode l_TreeInitScripts = l_XMLLua["initscripts"];
  if(l_TreeInitScripts.Exists())
  {
    int l_iNumScriptCalls = l_TreeInitScripts.GetNumChildren();
    for(int i = 0; i < l_iNumScriptCalls; i++)
    {
      CXMLTreeNode l_LuaScript = l_TreeInitScripts(i);
      if(l_LuaScript.IsComment()) {
        continue;
      }

      string l_szLuaCall = l_LuaScript.GetPszISOProperty("action","");

      if(l_szLuaCall != "")
      {
        LOGGER->AddNewLog(ELL_INFORMATION,"CScriptManager::Load Executant el codi previ \"%s\"", l_szLuaCall.c_str());
        RunCode(l_szLuaCall);
      }else
        LOGGER->AddNewLog(ELL_WARNING,"CScriptManager::Load Propietat \"action\" no trobada a linia %d", i);
    }
  }

  SetOk(true);
}

void CScriptManager::Reload()
{
  if(IsOk())
    Load(m_szFileName);
}

//Código de la función Alert que se llamará al generarse algún error de LUA
//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA 
int Alert(lua_State * State)
{
  std::string l_Text;
  int n = lua_gettop(State);
  int i;
  lua_getglobal(State, "tostring");
  for (i=1; i<=n; i++) {
    const char *s;
    lua_pushvalue(State, -1);
    lua_pushvalue(State, i);
    lua_call(State, 1, 1);
    s = lua_tostring(State, -1);
    if (s == NULL)
      return luaL_error(State, "'tostring' must return a string to 'print'");
    if (i>1) l_Text += '\t';
    l_Text += s;
    lua_pop(State, 1);
  }
  l_Text += '\n';
  LOGGER->AddNewLog(ELL_ERROR, "LUA ERROR: %s", l_Text.c_str());
  return true;
}


void CScriptManager::PrintError(const luabind::error& _error)
{
  const char* s = lua_tostring(_error.state(), -1);
  if(s == NULL)
  {
    LOGGER->AddNewLog(ELL_ERROR, "luabind::error Desconnegut");
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "luabind::error: %s", s);
  }
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
  lua_close(m_pLS);
}

void CScriptManager::Release()
{
  m_vLuaScripts.clear();
}

//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const string& _szCode) const
{
  int l_iError = luaL_dostring(m_pLS,_szCode.c_str());
  if(l_iError != 0)
  {
    const char *l_pcStr = lua_tostring(m_pLS, -1);
    LOGGER->AddNewLog(ELL_ERROR, "CScriptManager::RunCode  %s", l_pcStr);
  }
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const string& _szFileName) const
{
  int l_iError = luaL_dofile(m_pLS, _szFileName.c_str());
  if(l_iError != 0)
  {
    const char *l_pcStr = lua_tostring(m_pLS, -1);
    LOGGER->AddNewLog(ELL_ERROR, "CScriptManager::RunFile  %s", l_pcStr);
  }
}

void LogTextLua(const string &_c)
{
  LOGGER->AddNewLog(ELL_INFORMATION, _c.c_str());
}

CCore* GetCore()
{
  return CORE;
}

void CloseConsole()
{
  CORE->GetConsole()->SetActive(false);
}

void SetNewLevel(const string &_szLevel)
{
  CORE->GetLevelChanger()->SetNewLevel(_szLevel);
}

void SetRenderPath(const string& _szRenderPath)
{
  CORE->GetRenderer()->SetUniqueRenderPath(_szRenderPath);
}

void ReloadAnimationStates()
{
  CORE->GetAnimatedModelManager()->ReloadAnimaionStates();
}

void DeactivateCynematicCamera()
{
  CORE->GetRenderer()->SetActiveCamera(0);
}

void CScriptManager::RegisterGUI() {
  module(m_pLS)
    [
      // registramos la clase CGUIManager
			class_<CGUIManager>("CGUIManager")
			
			// registramos su constructor
      .def(constructor<const Vect2i&>())

      // registramos sus funciones publicas
			//.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)
		
			//.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
			//			"Muestra todas las funciones de esta clase"),
			//			&CScriptRegister::Help)

      .def(	"activeWindows",
						&CGUIManager::ActiveWindows)
			
			.def(	"activeWindowsWithEffect", 
						&CGUIManager::ActiveWindowsWithEffect)
      
			.def(	"pushWindows", 
						&CGUIManager::PushWindows)
      
			.def(	"popWindows", 
						&CGUIManager::PopWindows)
			
			.def(	"setScreenResolution",
						&CGUIManager::SetScreenResolution)
			
			.def(	"loadGuiFiles", 
						&CGUIManager::LoadGuiFiles)
			
			.def(	"reloadGui", 
						&CGUIManager::ReloadGuiFiles)
			
			.def(	"setMessageBox", 
						&CGUIManager::SetMessageBox)
			
			.def(	"isVisibleMessage", 
						&CGUIManager::IsVisibleMessage)
			
			.def(	"setVisiblePointerMouse",
						&CGUIManager::SetVisiblePointerMouse)
			
			//-------Funciones para modificar los GuiElements--------------------------		
			.def(	"setActiveGuiElement", 
						&CGUIManager::SetActiveGuiElement)
			
			.def(	"setVisibleGuiElement", 
						&CGUIManager::SetVisibleGuiElement)
			
			.def(	"getProgressBarValue", 
						&CGUIManager::GetProgressBarValue)
			
			.def(	"setProgressBarValue", 
						&CGUIManager::SetProgressBarValue)
			
			.def(	"nextBlockInRadioBox", 
						&CGUIManager::NextBlockInRadioBox)
			
			.def(	"prevBlockInRadioBox", 
						&CGUIManager::PrevBlockInRadioBox)
			
			.def(	"getButtonCheckInRadioBox", 
						&CGUIManager::GetButtonCheckInRadioBox)
			
			.def(	"setButtonCheckInRadioBox", 
						&CGUIManager::SetButtonCheckInRadioBox)
			
			.def(	"setStateCheckButton", 
						&CGUIManager::SetStateCheckButton)
			
			.def(	"getStateCheckButton", 
						&CGUIManager::GetStateCheckButton)
			
			.def(	"setStateSlider", 
						&CGUIManager::SetStateSlider)
			
			.def(	"getStateSlider", 
						&CGUIManager::GetStateSlider)
			
			.def(	"setLiteralInStaticText", 
						&CGUIManager::SetLiteralInStaticText)
			
			.def(	"setEditableTextBox", 
						&CGUIManager::SetEditableTextBox)
			
			.def(	"getEditableTextBox", 
						&CGUIManager::GetEditableTextBox)
			
			.def(	"setImage", 
						&CGUIManager::SetImage)
			
			.def(	"getImage", 
						&CGUIManager::GetImage)
			
			.def(	"playImage", 
						&CGUIManager::PlayImage)
    ];
}

void CScriptManager::RegisterLUAFunctions()
{
  lua_register(m_pLS,"_ALERT",Alert);
  
  //REGISTER_LUA_FUNCTION(m_pLS, "suma", Suma);
  //REGISTER_LUA_FUNCTION(m_pLS, "hello_world", HelloWorldLua);
  //REGISTER_LUA_FUNCTION(m_pLS, "log", LogTextLua);
  
  //general access functions
  module(m_pLS) [
    //logger
    def("log", &LogTextLua)
    //core macro
    ,def("get_core", &GetCore)
    //open/close console
    ,def("exit", &CloseConsole)
    //collision group
    ,def("get_collision_group", &GetCollisionGroup)
    //canviar de nivell
    ,def("set_new_level", &SetNewLevel)
    //activar un renderpath, desactivant la resta
    ,def("set_render_path", &SetRenderPath)
    //recarregar els animation states
    ,def("reload_animation_states", &ReloadAnimationStates)

    ,def("deactivate_cynematic_camera",&DeactivateCynematicCamera)
  ];

  //Vect3f
  module(m_pLS) [
    class_<Vect2f>("Vect2f")
      .def(constructor<>())
      .def(constructor<const Vect2f&>())
      .def(constructor<const float>())
      .def(constructor<const float, const float>())
      .def_readwrite("x",&Vect2f::x)
      .def_readwrite("y",&Vect2f::y)
      .def( const_self    + const_self )
      .def( const_self    - const_self )
      .def( const_self    * const_self )
      .def( const_self    * float() )
      .def( float() * const_self )
      .def( const_self    / float() )
      .def("length",    &Vect2f::Length)
      .def("length_sq", &Vect2f::SquaredLength)
      .def("normalize", &Vect2f::Normalize)
  ];

  //Vect3f
  module(m_pLS) [
    class_<Vect3f>("Vect3f")
      .def(constructor<>())
      .def(constructor<const Vect3f&>())
      .def(constructor<const float>())
      .def(constructor<const float, const float, const float>())
      .def_readwrite("x",&Vect3f::x)
      .def_readwrite("y",&Vect3f::y)
      .def_readwrite("z",&Vect3f::z)
      .def( const_self    + const_self )
      .def( const_self    - const_self )
      .def( const_self    * const_self )
      .def( const_self    ^ const_self )
      .def( const_self    * float() )
      .def( float() * const_self )
      .def( const_self    / float() )
      .def("length",       &Vect3f::Length)
      .def("length_sq",    &Vect3f::SquaredLength)
      .def("normalize",    &Vect3f::Normalize)
      .def("get_proj_zx",  &Vect3f::GetProjZX)
      .def("get_angle_y",  &Vect3f::GetAngleY)
  ];

  //Vect3f
  module(m_pLS) [
    class_<Vect2i>("Vect2i")
      .def(constructor<>())
      .def(constructor<const Vect2i&>())
      .def(constructor<const int>())
      .def(constructor<const int, const int>())
      .def_readwrite("x",&Vect2i::x)
      .def_readwrite("y",&Vect2i::y)
      .def( const_self    + const_self )
      .def( const_self    - const_self )
      .def( const_self    * const_self )
      .def( const_self    * int() )
      .def( int()   * const_self )
      .def( const_self    / int() )
      .def("length",    &Vect2i::Length)
      .def("length_sq", &Vect2i::SquaredLength)
  ];

  //Vect3f
  module(m_pLS) [
    class_<Vect3i>("Vect3i")
      .def(constructor<>())
      .def(constructor<const Vect3i&>())
      .def(constructor<const int>())
      .def(constructor<const int, const int, const int>())
      .def_readwrite("x",&Vect3i::x)
      .def_readwrite("y",&Vect3i::y)
      .def_readwrite("z",&Vect3i::z)
      .def( const_self    + const_self )
      .def( const_self    - const_self )
      .def( const_self    * const_self )
      .def( const_self    ^ const_self )
      .def( const_self    * int() )
      .def( int()   * const_self )
      .def( const_self    / int() )
      .def("length",    &Vect3i::Length)
      .def("length_sq", &Vect3i::SquaredLength)
  ];

  module(m_pLS) [
    class_<vector<Vect3f>>("Vect3fVector")
      .def("size",              &vector<Vect3f>::size)
      .def("at",                (vector<Vect3f>::reference (vector<Vect3f>::*) (vector<Vect3f>::size_type))&vector<Vect3f>::at)
      .def("const_at",          (vector<Vect3f>::const_reference (vector<Vect3f>::*) (vector<Vect3f>::size_type) const)&vector<Vect3f>::at)
  ];

  module(m_pLS) [
    class_<CColor>("Color")
      .def(constructor<>())
      .def(constructor<float, float, float, float>())
      .def("get_vect4f",    &CColor::GetArgb)
      //TODO
  ];
  //Object3D
  module(m_pLS) [
    class_<CObject3D>("Object3D")
      .def(constructor<>())
      .def(constructor<Vect3f,float,float,float>())
      .def("get_yaw",       &CObject3D::GetYaw)
      .def("get_roll",      &CObject3D::GetRoll)
      .def("get_pitch",     &CObject3D::GetPitch)
      .def("get_position",  &CObject3D::GetPosition)
      .def("get_visible",   &CObject3D::GetVisible)
      .def("set_yaw",       &CObject3D::SetYaw)
      .def("set_roll",      &CObject3D::SetRoll)
      .def("set_pitch",     &CObject3D::SetPitch)
      .def("set_position",  &CObject3D::SetPosition)
      .def("set_visible",   &CObject3D::SetVisible)
  ];

  //BaseControl
  module(m_pLS) [
    class_<CBaseControl>("BaseControl")
      .def("is_ok",    &CBaseControl::IsOk)
      .def("Done",     &CBaseControl::Done)
  ];



  //Core
  class_<CCore> l_core ("Core");

  l_core.def("get_renderable_objects_manager",  &CCore::GetRenderableObjectsManager)
        .def("get_console",                     &CCore::GetConsole)
        .def("get_gui_manager",                 &CCore::GetGUIManager)
        .def("get_light_manager",               &CCore::GetLightManager)
        .def("get_static_mesh_manager",         &CCore::GetStaticMeshManager)
        //.def("get_engine",                      &CCore::GetEngine)
        .def("get_script_manager",              &CCore::GetScriptManager)
        .def("get_input_manager",               &CCore::GetInputManager)
        .def("get_action_manager",              &CCore::GetActionManager)
        .def("get_sound_manager",               &CCore::GetSoundManager)
        .def("get_level_changer",               &CCore::GetLevelChanger)
        .def("get_renderer",                    &CCore::GetRenderer)
        .def("get_effect_manager",              &CCore::GetEffectManager)
        .def("set_exit",                        &CCore::SetExit)
        .def("set_pause",                       &CCore::SetPause);
  
  RegisterCore_Entities(l_core);
  RegisterCore_IA(l_core);

  module(m_pLS) [
    l_core

  //Process
    ,class_<CProcess,CBaseControl>("Process")

  //Engine
    ,class_<CEngine, CBaseControl>("Engine")
      .def("get_active_process",              &CEngine::GetActiveProcess)
      .def("set_exit",                        &CEngine::SetExit)
      .def("get_exit",                        &CEngine::GetExit)

  //Level Changer
    ,class_<CLevelChanger>("LevelChanger")
      .def("set_new_level", &CLevelChanger::SetNewLevel)
      .def("reset_level", &CLevelChanger::ResetLevel)
  ];

  //CRenderableObjectsManager
  module(m_pLS) [
    class_<CMapManager<CRenderableObject>>("RenderableObjectMapManager"),
    class_<CRenderableObjectsManager, CMapManager<CRenderableObject>>("RenderableObjectsManager")
      .def("get_resource",   &CRenderableObjectsManager::GetResource)
      .def("add_static",     &CRenderableObjectsManager::AddMeshInstance)
      .def("add_animated",   &CRenderableObjectsManager::AddAnimatedModel)
      .def("add_resource",   &CRenderableObjectsManager::AddResource)
      .def("set_instanced",  &CRenderableObjectsManager::SetInstanced)
      .def("reload",         ((bool(CRenderableObjectsManager::*)(void))&CRenderableObjectsManager::Reload))
      .def("reload_file",    ((bool(CRenderableObjectsManager::*)(const string&))&CRenderableObjectsManager::Reload))
  ];

  //StaticMeshManager
  module(m_pLS) [
    class_<CMapManager<CStaticMesh>>("StaticMeshMapManager"),
    class_<CStaticMeshManager, CMapManager<CStaticMesh>>("StaticMeshManager")
      .def("add_file", &CStaticMeshManager::AddXMLFile)
      .def("reload",   &CStaticMeshManager::Reload)
  ];

  //LightManager
  module(m_pLS) [
    class_<CMapManager<CLight>>("LightMapManager"),
    class_<CLightManager, CMapManager<CLight>>("LightManager")
      .def("load",           &CLightManager::Load)
      .def("get_resource",   &CLightManager::GetResource)
  ];

  //Light
  module(m_pLS) [
    class_<CLight>("Light")
      .def("set_active",     &CLight::SetActive)
      .def("set_color",      &CLight::SetColor)
      .def("get_color",      &CLight::GetColor)
      .def("set_start_att",  &CLight::SetStartRangeAttenuation)
      .def("get_start_att",  &CLight::GetStartRangeAttenuation)
      .def("set_end_att",    &CLight::SetEndRangeAttenuation)
      .def("get_end_att",    &CLight::GetEndRangeAttenuation)
  ];

  //Omni
  module(m_pLS) [
    class_<COmniLight, CLight>("OmniLight")
  ];

  //Directional
  module(m_pLS) [
    class_<CDirectionalLight, CLight>("DirectionalLight")
  ];

  //Spot
  module(m_pLS) [
    class_<CSpotLight, CDirectionalLight>("SpotLight")
  ];

  //ScriptManager
  module(m_pLS) [
    class_<CScriptManager>("ScriptManager")
      .def("reload", &CScriptManager::Reload)
  ];

  //SoundManager
  module(m_pLS) [
    class_<CSoundManager>("SoundManager")
      .def("play_sample", &CSoundManager::PlaySample)
      .def("stop_sample", &CSoundManager::StopSample)
      .def("play_sample_3D", &CSoundManager::PlaySample3D)
      .def("change_music", &CSoundManager::ChangeMusic)
      .def("play_music", &CSoundManager::PlayMusic)
      .def("set_music_position_3D", &CSoundManager::SetMusic3DPosition)
      .def("stop_all", &CSoundManager::StopAll)
      .def("stop_musics", &CSoundManager::StopMusics)
      .def("stop_sounds", &CSoundManager::StopSounds)
      .def("pause_samples", &CSoundManager::PauseSamples)
      .def("resume_samples", &CSoundManager::ResumeSamples)
      .def("set_master_volume", &CSoundManager::SetMasterVolume)
      .def("pause_music", &CSoundManager::Pause)
      .def("resume_music", &CSoundManager::Resume)
      .def("fade_music_volume", &CSoundManager::FadeMusicVolume)
  ];

  //Input
  module(m_pLS) [
    class_<CInputManager>("InputManager")
      .def("get_mouse_delta",  &CInputManager::GetMouseDelta)
    ,
    class_<CActionManager>("ActionManager")
      .def("is_action_active", &CActionManager::IsActionActive)
  ];

  //RenderableObject
  module(m_pLS) [
    class_<CRenderableObject, bases<CObject3D,CBaseControl>>("RenderableObject")
  ];

  //InstanceMesh
  module(m_pLS) [
    class_<CInstanceMesh, bases<CRenderableObject>>("InstanceMesh")
  ];

  //RenderableAnimatedInstanceModel
  module(m_pLS) [
    class_<CRenderableAnimatedInstanceModel, bases<CRenderableObject>>("AnimatedInstance")
  ];

  //Renderer
  module(m_pLS) [
    class_<CRenderer>("Renderer")
      .def("activate_render_path"  , &CRenderer::ActivateRenderPath  )
      .def("deactivate_render_path", &CRenderer::DeactivateRenderPath)
      .def("set_unique_render_path", &CRenderer::SetUniqueRenderPath)
      .def("get_active_render_path", &CRenderer::GetActiveRenderPaths)
      .enum_("BlendParameters")
      [
        value("bright_pass_threshold"     ,CRenderer::EBP_BRIGHT_PASS_THRESHOLD    ),
        value("exposure"                  ,CRenderer::EBP_EXPOSURE                 ),
        value("gauss_multiplier"          ,CRenderer::EBP_GAUSS_MULTIPLIER         ),
        value("max_luminance_limit_min"   ,CRenderer::EBP_MAX_LUMINANCE_LIMIT_MIN  ),
        value("max_luminance_limit_max"   ,CRenderer::EBP_MAX_LUMINANCE_LIMIT_MAX  ),
        value("scene_luminance_limit_min" ,CRenderer::EBP_SCENE_LUMINANCE_LIMIT_MIN),
        value("scene_luminance_limit_max" ,CRenderer::EBP_SCENE_LUMINANCE_LIMIT_MAX),
        value("bloom_final_scale"         ,CRenderer::EBP_BLOOM_FINAL_SCALE        ),
        value("glow_to_bloom"             ,CRenderer::EBP_GLOW_TO_BLOOM            ),
        value("glow_luminance_scale"      ,CRenderer::EBP_GLOW_LUMINANCE_SCALE     ),
        value("max_glow_luminance"        ,CRenderer::EBP_MAX_GLOW_LUMINANCE       ),
        value("glow_final_scale"          ,CRenderer::EBP_GLOW_FINAL_SCALE         ),
        
        value("blur_radius"               ,CRenderer::EBP_BLUR_RADIUS              ),
        value("near_focal_plane_depth"    ,CRenderer::EBP_NEAR_FOCAL_PLANE_DEPTH   ),
        value("far_focal_plane_depth"     ,CRenderer::EBP_FAR_FOCAL_PLANE_DEPTH    ),
        value("near_blur_depth"           ,CRenderer::EBP_NEAR_BLUR_DEPTH          ),
        value("far_blur_depth"            ,CRenderer::EBP_FAR_BLUR_DEPTH           )
      ]
      .def("blend_parameter", &CRenderer::BlendParameter)
  ];

  //EffectManager
  module(m_pLS) [
    class_<CEffectManager>("EffectManager")
      .def("set_bright_pass_threshold"  , &CEffectManager::SetBrightPassThreshold )
      .def("set_exposure"               , &CEffectManager::SetExposure            )
      .def("set_gauss_multiplier"       , &CEffectManager::SetGaussMultiplier     )
      .def("set_max_luminance_limits"   , &CEffectManager::SetMaxLuminanceLimits  )
      .def("set_scene_luminance_limits" , &CEffectManager::SetSceneLuminanceLimits)
      .def("set_bloom_final_scale"      , &CEffectManager::SetBloomFinalScale     )
      .def("set_glow_to_bloom"          , &CEffectManager::SetGlowToBloom         )
      .def("set_glow_luminance_scale"   , &CEffectManager::SetGlowLuminanceScale  )
      .def("set_max_glow_luminance"     , &CEffectManager::SetMaxGlowLuminance    )
      .def("set_glow_final_scale"       , &CEffectManager::SetGlowFinalScale      )
      .def("print_hdr"                  , &CEffectManager::PrintHDRParams  )

      .def("set_blur_radius"            , &CEffectManager::SetBlurRadius          )
      .def("set_near_blur_depth"        , &CEffectManager::SetNearBlurDepth       )
      .def("set_far_blur_depth"         , &CEffectManager::SetFarBlurDepth        )
      .def("set_near_focal_plane_depth" , &CEffectManager::SetNearFocalPlaneDepth )
      .def("set_far_focal_plane_depth"  , &CEffectManager::SetFarFocalPlaneDepth  )
  ];


  module(m_pLS) [                                       
    class_<CConsole>("Console")
      .def("toggle", &CConsole::Toggle)
  ];
    /*
    ,class_<CSingleton<CCore>>("CSingletonCore")
    ,class_<CCore, bases<CBaseControl,CSingleton<CCore>>>("CCore")
      //.def("get_light_manager", &CCore::GetLightManager)
    */

  RegisterGUI();
  RegisterEntitiesToLua(m_pLS);
  RegisterIAToLua(m_pLS);
}
