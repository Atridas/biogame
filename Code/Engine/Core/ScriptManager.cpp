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

#include "Core.h"

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
  //luaopen_io(m_pLS);
  luaopen_debug(m_pLS);
  //Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA 
  RegisterLUAFunctions();

  //RunCode( "local a = 2;local b = 5;local c = suma(a, b);log(\"el valor es \" .. c)" );
  //RunFile("Data/Lua/first.lua");
}



//Código de la función Alert que se llamará al generarse algún error de LUA
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
      return luaL_error(State, "`tostring' must return a string to `print'");
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
//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
  if(luaL_dostring(m_pLS,Code.c_str()))
  {
    const char *l_Str=lua_tostring(m_pLS, -1);
    LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
  }
}
//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
  if(luaL_dofile(m_pLS, FileName.c_str()))
  {
    const char *l_Str=lua_tostring(m_pLS, -1);
    LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
  }
}


int Suma(int a, int b)
{
  return a + b;
}


void HelloWorldLua()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "Hola Mundo");
}

void LogTextLua(const string &_c)
{
  LOGGER->AddNewLog(ELL_INFORMATION, _c.c_str());
}



CCore& GetCore()
{
  return CCore::GetSingleton();
}



void CScriptManager::RegisterLUAFunctions()
{
  lua_register(m_pLS,"_ALERT",Alert);
  
  //REGISTER_LUA_FUNCTION(m_pLS, "suma", Suma);
  //REGISTER_LUA_FUNCTION(m_pLS, "hello_world", HelloWorldLua);
  //REGISTER_LUA_FUNCTION(m_pLS, "log", LogTextLua);
  
  module(m_pLS) [
    def("suma", &Suma),
    def("hello_world", &HelloWorldLua),
    def("log", &LogTextLua)
    /*
    ,
    class_<CBaseControl>("CBaseControl")
      .def("is_ok", &CBaseControl::IsOk)
    ,
    class_<CSingleton<CCore>>("CSingletonCore")
    ,
    class_<CCore, bases<CBaseControl,CSingleton<CCore>>>("CCore")
      //.def("get_light_manager", &CCore::GetLightManager)
    */
  ];

  //REGISTER_LUA_FUNCTION(m_pLS, "get_core", GetCore);
}