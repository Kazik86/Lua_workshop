#include "actor.h"

#include "luaModule.h"
#include "luaState.h"

#include <cassert>
#include <stdexcept>
#include <vector>

namespace
{
    int scriptName(lua_State* aLua)
    {
	const eActor* me = Script::getUdata<eActor>(aLua);
	lua_pushstring(aLua, me->getScript().c_str());
	return 1;
    }

    int gadgetsNum(lua_State* aLua)
    {
	const eActor* me = Script::getUdata<eActor>(aLua);
	lua_pushunsigned(aLua, me->getGadgetsNum());
	return 1;
    }
}

DEFINE_USERDATA_API(eActor)
{
    {"getScript", scriptName},
    {"getGadgetsNum", gadgetsNum},
    {0, 0}
};

DEFINE_USERDATA_SUPPORT(eActor)

eActor::eActor(eLuaState& aLua, const std::string& aScript):
    iLua(aLua),
    iFsm(aLua),
    iScript(aScript)
{
}

eActor::~eActor()
{
    lua_State* lua = iLua.getRaw();
    for (int ref : iMeRef)
	luaL_unref(lua, LUA_REGISTRYINDEX, ref);
}

void eActor::doScript()
{
    iModule = &LuaModuleMgr::load(iLua, iScript);
    createMeTables();
    callOnInit();
    createGadgetsContainer();
}

void eActor::update()
{
    iFsm.update();
}

void eActor::shareInternalsWithScript()
{
    std::vector<sActorSharedInternal> shares
    {
	{"eActor", this}
    };

    lua_State* lua = iLua.getRaw();

    for (const sActorSharedInternal& s : shares) {
	lua_pushlightuserdata(lua, s.iAddress);
	lua_setfield(lua, -2, s.iName);
    }
}

void eActor::createMeTables()
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

    shareInternalsWithScript();

    iMeRef.push_front(luaL_ref(lua, LUA_REGISTRYINDEX));
}

void eActor::callLuaFunc(const char* aFunctionName)
{
    callLuaFuncWithEnv(iModule->iRef, iMeRef.front(), aFunctionName);
}

void eActor::callLuaFuncWithEnv(int aModuleRef, int aMeRef, const char* aFunctionName)
{
    lua_State* lua = iLua.getRaw();
    
    lua_rawgeti(lua, LUA_REGISTRYINDEX, aModuleRef);
    lua_getfield(lua, -1, aFunctionName);

    if (! lua_isfunction(lua, -1))
	throw std::runtime_error(iScript + ": no function with name " + aFunctionName);

    lua_rawgeti(lua, LUA_REGISTRYINDEX, aMeRef);

    if(lua_pcall(lua, 1, 0, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(lua, -1));

    lua_pop(lua, 1);
}

void eActor::callLuaFuncThroughInheritanceHierarchyBackward(const char* aFunctionName)
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

void eActor::callOnInit()
{
    callLuaFuncThroughInheritanceHierarchyBackward("OnInit");
}

void eActor::callOnRestart()
{
    callLuaFuncThroughInheritanceHierarchyBackward("OnRestart");
}

void eActor::createGadgetsContainer()
{
    lua_State* lua = iLua.getRaw();
    std::vector<eGadget*>::size_type gadgetsNum = 0;

    // najpierw policzmy ile aktor ma gadżetów
    for (int ref : iMeRef) {
	lua_rawgeti(lua, LUA_REGISTRYINDEX, ref);
	lua_pushnil(lua);

	while (lua_next(lua, -2) != 0) {
	    if (lua_isuserdata(lua, -1) && !lua_islightuserdata(lua, -1))
		++gadgetsNum;

	    lua_pop(lua, 1);
	}

	lua_pop(lua, 1);
    }

    // miejsce na gadżety
    iGadgets.reserve(gadgetsNum);

    // a teraz tworzymy listę gadżetów
    auto itBeg = iMeRef.crbegin();
    auto itEnd = iMeRef.crend();

    for (auto it = itBeg; it != itEnd; ++it) {
	lua_rawgeti(lua, LUA_REGISTRYINDEX, *it);
	lua_pushnil(lua);

	while (lua_next(lua, -2) != 0) {
	    if (lua_isuserdata(lua, -1) && !lua_islightuserdata(lua, -1))
		iGadgets.push_back(static_cast<eGadget*>(lua_touserdata(lua, -1)));

	    lua_pop(lua, 1);
	}

	lua_pop(lua, 1);
    }
}
