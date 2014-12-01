#include "fsm.h"

#include "luaModule.h"
#include "luaState.h"

#include <cassert>
#include <cstring>
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
    iMeRef.push_front(luaL_ref(lua, LUA_REGISTRYINDEX));

    // from inheritance
    const std::list<sModule*>& ih = iModule->iInheritanceHierarchy;

    for (decltype(ih.size()) i = 0; i < ih.size(); ++i) {
	lua_newtable(lua);
	iMeRef.push_back(luaL_ref(lua, LUA_REGISTRYINDEX));
    }
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

    /* nie każda funkcja musi posiadać jako upvalue swoje _ENV. Z tego co widzę
     * z eksperymentów, _ENV dołączane jest tylko do funkcji mających 'free
     * variables'. W dodatku nawet jeśli mamy _ENV to niekoniecznie jako
     * upvalue na pozycji 1.
     */
    for (int i = 1; ; ++i) {
	const char* upvalueName = lua_getupvalue(lua, -1, i);
	if (upvalueName == 0)
	    break;
	else if(strcmp(upvalueName, "_ENV") == 0) {
	    lua_pushvalue(lua, -3);
	    lua_setupvalue(lua, -3, i);
	    lua_pop(lua, 1);
	    break;
	}

	lua_pop(lua, 1);
    }

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

    assert(envIt == iMeRef.crend());
}

void eFsm::callOnInit()
{
    callLuaFuncThroughInheritanceHierarchyBackward("OnInit");
}

void eFsm::callOnRestart()
{
    callLuaFuncThroughInheritanceHierarchyBackward("OnRestart");
}
