#ifndef LUA_MODULE_H
#define LUA_MODULE_H

#include <lua.hpp>
#include <string>

class eLuaState;

namespace LuaModuleMgr
{
    int load(eLuaState& aLua, const std::string& aName);
    int getModule(const std::string& aName);
    void clear();
}

#endif // LUA_MODULE_H
