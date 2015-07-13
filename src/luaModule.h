#ifndef LUA_MODULE_H
#define LUA_MODULE_H

#include "userdata.h"

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
    int iChunkRef;
};

class eLuaModuleMgr
{
    DECLARE_USERDATA_CLASS()

public:
    eLuaModuleMgr();
    ~eLuaModuleMgr();

    static eLuaModuleMgr* getMe() { return iMe; }

    const sModule& load(lua_State* aLua, const std::string& aName) { return add(aLua, aName); }
    int getModule(const std::string& aName) { return iModules[aName].iRef; }
    const sModule* reloadModule(lua_State* aLua, const std::string& aModule);
    const sModule* callSnippet(lua_State* aLua, const std::string& aModule, const std::string& aFile);
    void removeSnippet(lua_State* aLua, int aModuleRef, const char* aSnippetName);
    bool isOnInheritanceList(const sModule* aModule, const sModule* aFind);

private:
    eLuaModuleMgr(const eLuaModuleMgr& aOther);
    eLuaModuleMgr& operator=(const eLuaModuleMgr& aOther);

    void setClass(lua_State* aLua, sModule& aModule);
    void setGlobal(lua_State* aLua);
    void setScript(lua_State* aLua, const std::string& aScript);

    sModule* checkClassUniqueness(lua_State* aLua, sModule& aModule);
    bool entryStateExists(lua_State* aLua, sModule& aModule);
    sModule& add(lua_State* aLua, const std::string& aName);

    void saveChunk(lua_State* aLua, sModule& aModule);
    void callChunk(lua_State* aLua, int aModuleRef);

private:
    static eLuaModuleMgr* iMe;
    std::unordered_map<std::string, sModule> iModules;
};

#endif // LUA_MODULE_H
