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

#include "Utils\MemLeaks.h"

#include "Core.h"
#include <XML/XMLTreeNode.h>

using namespace luabind;

#define REGISTER_LUA_FUNCTION(LUA_STATE,FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

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

  int l_iNumScriptFiles = l_XMLLua.GetNumChildren();
  for(int i = 0; i < l_iNumScriptFiles; i++)
  {
    CXMLTreeNode l_LuaFile = l_XMLLua(i);
    if(l_LuaFile.IsComment()) {
      continue;
    }

    string l_szLuaFile = l_LuaFile.GetPszISOProperty("file","");

    if(l_szLuaFile != "")
      m_vLuaScripts.push_back(l_szLuaFile);
    else
      LOGGER->AddNewLog(ELL_WARNING,"CScriptManager::Load Propietat \"file\" no trobada a linia %d", i);
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

//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
  m_pLS=lua_open();
  luaL_openlibs(m_pLS);

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
  if(luaL_dostring(m_pLS,_szCode.c_str()))
  {
    const char *l_pcStr=lua_tostring(m_pLS, -1);
    LOGGER->AddNewLog(ELL_INFORMATION, "CScriptManager::RunCode  %s", l_pcStr);
  }
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const string& _szFileName) const
{
  if(luaL_dofile(m_pLS, _szFileName.c_str()))
  {
    const char *l_pcStr=lua_tostring(m_pLS, -1);
    LOGGER->AddNewLog(ELL_INFORMATION, "CScriptManager::RunFile  %s", l_pcStr);
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



CCore* GetCore()
{
  return CCore::GetSingletonPtr();
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
    def("log", &LogTextLua),
    def("get_core", &GetCore)
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