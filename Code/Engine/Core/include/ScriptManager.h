#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>


struct lua_State;

#include "base.h"
#include "Utils/BaseControl.h"
#include "Core.h"

class CScriptManager:
  public CBaseControl
{
public:
  CScriptManager() : m_pLS(0) {};
  ~CScriptManager() {Destroy(); Done();};
  void Initialize();
  void Destroy();
  void RunCode(const string& _szCode) const;
  void RunFile(const string& _szFileName) const;
  void Load(const string& _szFileName);
  void Reload();
  void Execute();
  lua_State * GetLuaState() const {return m_pLS;}
  void RegisterLUAFunctions();

protected:
  void Release();

private:
  lua_State*      m_pLS;
  string          m_szFileName;
  vector<string>  m_vLuaScripts;
};

#endif