#ifndef LUA_LIBS_H
#define LUA_LIBS_H

#include "luaState.h"
#include "../tests/funFromC.h"

#include <lua.hpp>

const luaL_Reg eLuaState::iBaseLibs[] = {
    {"_G", luaopen_base},
    {LUA_LOADLIBNAME, luaopen_package},
    // {LUA_COLIBNAME, luaopen_coroutine},
    // {LUA_TABLIBNAME, luaopen_table},
    // {LUA_IOLIBNAME, luaopen_io},
    // {LUA_OSLIBNAME, luaopen_os},
    // {LUA_STRLIBNAME, luaopen_string},
    // {LUA_BITLIBNAME, luaopen_bit32},
    // {LUA_MATHLIBNAME, luaopen_math},
    // {LUA_DBLIBNAME, luaopen_debug},
    {NULL, NULL}
};

const luaL_Reg eLuaState::iGadgetLibs[] = {
    {"eFunFromC", eFunFromC::luaOpen},
    {NULL, NULL}
};

#endif // LUA_LIBS_H
