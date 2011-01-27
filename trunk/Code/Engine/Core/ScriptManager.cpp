#include "ScriptManager.h"

int SumaLua(lua_State *_State);
int HelloWorldLua(lua_State *_State);
int LogTextLua(lua_State *_State);

//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
  m_LS=lua_open();
  luaopen_base(m_LS);
  luaopen_string(m_LS);
  luaopen_table(m_LS);
  luaopen_math(m_LS);
  luaopen_io(m_LS);
  luaopen_debug(m_LS);
  //Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA lua_register(m_LS,"_ALERT",Alert);
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
  lua_close(m_LS);
}
//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
  if(luaL_dostring(m_LS,Code.c_str()))
  {
    const char *l_Str=lua_tostring(m_LS, -1);
    LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
  }
}
//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
  if(luaL_dofile(m_LS, FileName.c_str()))
  {
    const char *l_Str=lua_tostring(m_LS, -1);
    LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
  }
}


int Suma(int a, int b)
{
  return a + b;
}

int SumaLua(lua_State *_State)
{
  int a = lua_tointeger(_State, 0);
  int b = lua_tointeger(_State, 1);

  int c = a + b;

  lua_pushinteger(_State, c);

  return 1;
}


/*
int HelloWorldLua(lua_State *_State)
{
  LOGGER->(ELL_INfo "Hola Mundo");

  return 0;
}

int LogTextLua(lua_State *_State)
{
  char* c = lua_tostring(_State, 0);
  LOGGER->(c);

  return 0;
}
*/