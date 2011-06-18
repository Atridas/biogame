#pragma once

#ifndef __ENTITIES_TO_LUA_H__
#define __ENTITIES_TO_LUA_H__


struct lua_State;
void RegisterEntitiesToLua(lua_State* _pLS);
void RegisterCore(luabind::class_<CCore>& _Core);

#endif