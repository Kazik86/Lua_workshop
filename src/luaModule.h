#ifndef LUA_MODULE_H
#define LUA_MODULE_H

#include <list>
#include <lua.hpp>
#include <string>
#include <unordered_map>

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
    const sModule& load(lua_State* aLua, const std::string& aName);
    int getModule(const std::string& aName);
    void clear();
}

class eLuaModuleMgr
{
public:
    eLuaModuleMgr();
    ~eLuaModuleMgr();

    static eLuaModuleMgr* getMe() { return iMe; }

    const sModule& load(lua_State* aLua, const std::string& aName) { return add(aLua, aName); }
    int getModule(const std::string& aName) { return iModules[aName].iRef; }

private:
    eLuaModuleMgr(const eLuaModuleMgr& aOther);
    eLuaModuleMgr& operator=(const eLuaModuleMgr& aOther);

    void setClass(lua_State* aLua, sModule& aModule);
    void setGlobal(lua_State* aLua);
    void setScript(lua_State* aLua, const std::string& aScript);

    std::list<sModule*> derive(lua_State* aLua);
    sModule* checkClassUniqueness(lua_State* aLua, sModule& aModule);
    sModule& add(lua_State* aLua, const std::string& aName);

private:
    static eLuaModuleMgr* iMe;
    std::unordered_map<std::string, sModule> iModules;
};

#endif // LUA_MODULE_H
