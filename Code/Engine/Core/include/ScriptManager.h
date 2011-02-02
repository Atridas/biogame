#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>


struct lua_State;

#include "base.h"
#include "Core.h"

class CScriptManager
{
public:
  CScriptManager() : m_pLS(0) {};
  ~CScriptManager() {Destroy();};
  void Initialize();
  void Destroy();
  void RunCode(const std::string &Code) const;
  void RunFile(const std::string &FileName) const;
  void Load(const std::string &XMLFile);
  lua_State * GetLuaState() const {return m_pLS;}
  void RegisterLUAFunctions();
private:
  lua_State *m_pLS;
};
#endif