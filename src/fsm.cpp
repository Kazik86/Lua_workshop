#include "fsm.h"

#include "luaModule.h"
#include "luaState.h"

#include <cassert>
#include <stdexcept>

eFsm::eFsm(eLuaState& aLua, const std::string& aScript):
    iLua(aLua),
    iScript(aScript),
    iModule(0)
{
}

eFsm::~eFsm()
{
    lua_State* lua = iLua.getRaw();
    for (int ref : iMeRef)
	luaL_unref(lua, LUA_REGISTRYINDEX, ref);
}

void eFsm::doScript()
{
    iModule = &LuaModuleMgr::load(iLua, iScript);
    createMeTables();
}

void eFsm::update()
{

}

void eFsm::createMeTables()
{
    lua_State* lua = iLua.getRaw();

    // me
    lua_newtable(lua);
    lua_pushvalue(lua, -1);
    lua_setfield(lua, -1, iModule->iClass.c_str()); // me.MyClass = me
    lua_pushvalue(lua, -1);

    // from inheritance
    for (const sModule* m : iModule->iInheritanceHierarchy) {
	lua_newtable(lua);
	lua_pushvalue(lua, -1);
	lua_setfield(lua, 1, m->iClass.c_str()); // me.DerivedClass = DerivedMe

	lua_newtable(lua);
	lua_pushvalue(lua, -2);
	lua_setfield(lua, -2, "__index");
	lua_setmetatable(lua, -3);
	lua_remove(lua, -2);
	
	lua_pushvalue(lua, -1);

	iMeRef.push_back(luaL_ref(lua, LUA_REGISTRYINDEX));
    }

    lua_pop(lua, 1);
    iMeRef.push_front(luaL_ref(lua, LUA_REGISTRYINDEX));
}

void eFsm::callLuaFunc(const char* aFunctionName)
{
    callLuaFuncWithEnv(iModule->iRef, iMeRef.front(), aFunctionName);
}

void eFsm::callLuaFuncWithEnv(int aModuleRef, int aMeRef, const char* aFunctionName)
{
    lua_State* lua = iLua.getRaw();
    
    lua_rawgeti(lua, LUA_REGISTRYINDEX, aModuleRef);
    lua_getfield(lua, -1, aFunctionName);

    lua_rawgeti(lua, LUA_REGISTRYINDEX, aMeRef);

    if(lua_pcall(lua, 1, 0, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(lua, -1));

    lua_pop(lua, 1);
}

void eFsm::callLuaFuncThroughInheritanceHierarchyBackward(const char* aFunctionName)
{
    const std::list<sModule*>& ih = iModule->iInheritanceHierarchy;

    auto moduleItBeg = ih.crbegin();
    auto moduleItEnd = ih.crend();

    auto envIt = iMeRef.crbegin();

    for (auto it = moduleItBeg; it != moduleItEnd; ++it, ++envIt) {
	sModule* m = *it;
	callLuaFuncWithEnv(m->iRef, *envIt, aFunctionName);
    }

    callLuaFuncWithEnv(iModule->iRef, *envIt, aFunctionName);

    assert(++envIt == iMeRef.crend());
}

void eFsm::callOnInit()
{
    callLuaFuncThroughInheritanceHierarchyBackward("OnInit");
}

void eFsm::callOnRestart()
{
    callLuaFuncThroughInheritanceHierarchyBackward("OnRestart");
}
