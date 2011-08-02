#pragma once

#ifndef __IA_TO_LUA_H__
#define __IA_TO_LUA_H__


struct lua_State;
void RegisterIAToLua(lua_State* _pLS);
void RegisterCore_IA(luabind::class_<CCore>& _Core);

#endif