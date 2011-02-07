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
#include "RenderableObjectsManager.h"
#include "InstanceMesh.h"
#include "RenderableAnimatedInstanceModel.h"

#include "Object3D.h"
#include "Utils/BaseControl.h"

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
  CXMLTreeNode l_TreeFiles = l_XMLLua["Files"];
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
      RunFile(l_szLuaFile);
    }
    else
      LOGGER->AddNewLog(ELL_WARNING,"CScriptManager::Load Propietat \"file\" no trobada a linia %d", i);
  }

  //Init Scripts
  CXMLTreeNode l_TreeInitScripts = l_XMLLua["InitScripts"];
  int l_iNumScriptCalls = l_TreeInitScripts.GetNumChildren();
  for(int i = 0; i < l_iNumScriptCalls; i++)
  {
    CXMLTreeNode l_LuaScript = l_TreeInitScripts(i);
    if(l_LuaScript.IsComment()) {
      continue;
    }

    string l_szLuaCall = l_LuaScript.GetPszISOProperty("action","");

    if(l_szLuaCall != "")
      RunCode(l_szLuaCall);
    else
      LOGGER->AddNewLog(ELL_WARNING,"CScriptManager::Load Propietat \"action\" no trobada a linia %d", i);
  }

  SetOk(true);
}

void CScriptManager::Reload()
{
  if(IsOk())
    Load(m_szFileName);
}

void CScriptManager::Execute()
{
  if(IsOk())
  {
    int l_iNumScriptFiles = m_vLuaScripts.size();
    
    for(int i = 0; i < l_iNumScriptFiles; i++)
      RunFile(m_vLuaScripts[i]);
  }
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
      .def("get_renderable_objects_manager", &CCore::GetRenderableObjectsManager)
  ];

  //CRenderableObjectsManager
  module(m_pLS) [
    class_<CRenderableObjectsManager>("RenderableObjectsManager")
      .def("get_resource",   &CRenderableObjectsManager::GetResource)
      .def("add_static",     &CRenderableObjectsManager::AddMeshInstance)
      .def("add_animated",   &CRenderableObjectsManager::AddAnimatedModel)
      .def("add_resource",   &CRenderableObjectsManager::AddResource)
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

    /*
    ,class_<CSingleton<CCore>>("CSingletonCore")
    ,class_<CCore, bases<CBaseControl,CSingleton<CCore>>>("CCore")
      //.def("get_light_manager", &CCore::GetLightManager)
    */
}