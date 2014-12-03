#ifndef LUA_MODULE_H
#define LUA_MODULE_H

#include <list>
#include <lua.hpp>
#include <string>

class eLuaState;

struct sModule
{
    sModule();
    ~sModule();

    int iRef;
    lua_State* iLua;
    std::string iScript;
    std::string iClass;
    std::list<sModule*> iInheritanceHierarchy;
};

namespace LuaModuleMgr
{
    const sModule& load(eLuaState& aLua, const std::string& aName);
    int getModule(const std::string& aName);
    void clear();
}

#endif // LUA_MODULE_H
