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

#include "Utils/Object3D.h"
#include "Utils/BaseControl.h"

#include "Console.h"

#include "GUIManager.h"

#include "EntityDefines.h"
#include "ScriptedStateMachine.h"


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
  LOGGER->AddNewLog(ELL_INFORMATION, l_Text.c_str());
  return true;
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


// Entity System functions ----------------------------

luabind::object GetEventInfo(SEvent const& x, lua_State* L)
{
    luabind::object result = luabind::newtable(L);

    for (int i = 0; i < EVENT_INFO_SIZE; ++i)
        result[i + 1] = x.Info[i];

    return result;
}

void SetEventInfo(SEvent& x, luabind::argument const& table)
{
    for (int i = 0; i < EVENT_INFO_SIZE; ++i)
        x.Info[i] = luabind::object_cast<SEventInfo>(table[i + 1]);
} 

// ----------------------------------------------------



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
  ];

  //Vect3f
  module(m_pLS) [
    class_<Vect3f>("Vect3f")
      .def(constructor<float>())
      .def(constructor<float, float, float>())
      .def_readwrite("x",&Vect3f::x)
      .def_readwrite("y",&Vect3f::y)
      .def_readwrite("z",&Vect3f::z)
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
  module(m_pLS) [
    class_<CCore>("Core")
      .def("get_renderable_objects_manager",  &CCore::GetRenderableObjectsManager)
      .def("get_console",                     &CCore::GetConsole)
      .def("get_gui_manager",                 &CCore::GetGUIManager)
      .def("get_light_manager",               &CCore::GetLightManager)
      .def("get_static_mesh_manager",         &CCore::GetStaticMeshManager)
      .def("get_engine",                      &CCore::GetEngine)

  //Process
    ,class_<CProcess,CBaseControl>("Process")

  //Engine
    ,class_<CEngine, CBaseControl>("Engine")
      .def("get_active_process",              &CEngine::GetActiveProcess)
      .def("set_exit",                        &CEngine::SetExit)
      .def("get_exit",                        &CEngine::GetExit)
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
      .def("reload",         ((bool(CRenderableObjectsManager::*)(const string&))&CRenderableObjectsManager::Reload))
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
      .def("load", &CLightManager::Load)
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

  module(m_pLS) [
    class_<CConsole>("Console")
      .def("toggle", &CConsole::Toggle)
  ];
    /*
    ,class_<CSingleton<CCore>>("CSingletonCore")
    ,class_<CCore, bases<CBaseControl,CSingleton<CCore>>>("CCore")
      //.def("get_light_manager", &CCore::GetLightManager)
    */


  module(m_pLS) [
    class_<SEventInfo>("EventInfo")
      .enum_("Type")
      [
        value("sti_int"   ,SEventInfo::STI_INT),
        value("sti_float" ,SEventInfo::STI_FLOAT),
        value("sti_vector",SEventInfo::STI_VECTOR),
        value("sti_string",SEventInfo::STI_STRING)
      ]
      .def_readwrite("i",    &SEventInfo::i)
      .def_readwrite("f",    &SEventInfo::f)
      .def_readwrite("v",    &SEventInfo::v)
      .def_readwrite("str",  &SEventInfo::str)
      
    ,class_<SEvent>("Event")
      //.enum_("EventType")
      //[
      //]
      .def_readwrite("Sender",      &SEvent::Sender)
      .def_readwrite("Receiver",    &SEvent::Receiver)
      .def_readwrite("Msg",         &SEvent::Msg)
      .def_readwrite("DispatchTime",&SEvent::DispatchTime)
      .property("Info", GetEventInfo, SetEventInfo, raw(_2)) 
      //.def_readwrite("Info",        &SEvent::Info)
      
    

    ,class_<CBaseComponent>("BaseComponent")
      .enum_("ComponentType")
      [
          value("phisX_controller", CBaseComponent::ECT_PHISX_CONTROLLER),
          value("object_3d",        CBaseComponent::ECT_OBJECT_3D)
      ]
      .def("get_type",     &CBaseComponent::GetType)
      .def("get_entity",   &CBaseComponent::GetEntity)
      .def("is_type",      &CBaseComponent::IsType)
      .def("receive_event",&CBaseComponent::ReceiveEvent)

    ,class_<CGameEntity>("GameEntity")
      .def("init",          &CGameEntity::Init)
      .def("get_guid",      &CGameEntity::GetGUID)
      .def("receive_event", &CGameEntity::ReceiveEvent)
   
    ,class_<CScriptedStateMachine>("ScriptedStateMachine")
      .def("set_current_state",    &CScriptedStateMachine::SetCurrentState)
      .def("change_state",         &CScriptedStateMachine::ChangeState)
      .def("current_state",        &CScriptedStateMachine::CurrentState)
      .def("receive_event",        &CScriptedStateMachine::ReceiveEvent)
  ];

  RegisterGUI();
}