#include "actor.h"

#include "gadget.h"
#include "game.h"
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

    int shift(lua_State* aLua)
    {
	eActor* me = Script::getUdata<eActor>(aLua);
	me->shift(aLua);
	return 0;
    }
}

DEFINE_USERDATA_API(eActor)
{
    {"getScript", ::scriptName},
    {"getGadgetsNum", ::gadgetsNum},
    {"shift", ::shift},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eActor)

eActor::eActor(const std::string& aScript):
    iFsm(*this),
    iScript(aScript),
    iPos(0, 0)
{
}

eActor::~eActor()
{
    lua_State* lua = eGame::getMe()->getLua()->getRaw();
    for (int ref : iMeRef)
	luaL_unref(lua, LUA_REGISTRYINDEX, ref);
}

void eActor::doScript(lua_State* aLua)
{
    iModule = &eLuaModuleMgr::getMe()->load(aLua, iScript);
    createMeTables(aLua);
    callOnInit(aLua);

    if (! iFsm.isEntryStateSet())
	throw std::runtime_error(iScript + ": no entry state.");

    beginGadget();
}

void eActor::update(lua_State* aLua, float aDelta)
{
    for (eGadget* g : iGadgets) {
	if (g->isEnabled())
	    g->update(aLua, aDelta);
    }

    iFsm.update(aLua);
}

void eActor::shareInternalsWithScript(lua_State* aLua, int aRef)
{
    std::vector<sActorSharedInternal> shares
    {
	{"eActor", this},
	{"eFsm", &iFsm}
    };

    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aRef);

    for (const sActorSharedInternal& s : shares) {
	lua_pushlightuserdata(aLua, s.iAddress);
	lua_setfield(aLua, -2, s.iName);
    }

    lua_pop(aLua, 1);
}

void eActor::createMeTables(lua_State* aLua)
{
    // me
    lua_newtable(aLua);
    lua_pushvalue(aLua, -1);
    lua_setfield(aLua, -1, iModule->iClass.c_str()); // me.MyClass = me
    lua_pushvalue(aLua, -1);

    // from inheritance
    for (const sModule* m : iModule->iInheritanceHierarchy) {
	lua_newtable(aLua);
	lua_pushvalue(aLua, -1);
	lua_setfield(aLua, 1, m->iClass.c_str()); // me.DerivedClass = DerivedMe

	lua_newtable(aLua);
	lua_pushvalue(aLua, -2);
	lua_setfield(aLua, -2, "__index");
	lua_setmetatable(aLua, -3);
	lua_remove(aLua, -2);
	
	lua_pushvalue(aLua, -1);

	iMeRef.push_back(luaL_ref(aLua, LUA_REGISTRYINDEX));
    }

    lua_pop(aLua, 1);
    iMeRef.push_front(luaL_ref(aLua, LUA_REGISTRYINDEX));

    shareInternalsWithScript(aLua, iMeRef.back());
}

void eActor::callLuaFunc(lua_State* aLua, const char* aFunctionName)
{
    callLuaFuncWithEnv(aLua, iModule->iRef, iMeRef.front(), aFunctionName);
}

void eActor::callLuaFuncWithEnv(lua_State* aLua, int aModuleRef, int aMeRef, const char* aFunctionName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);
    lua_getfield(aLua, -1, aFunctionName);

    if (! lua_isfunction(aLua, -1))
	throw std::runtime_error(iScript + ": no function with name " + aFunctionName);

    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aMeRef);

    if(lua_pcall(aLua, 1, 0, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(aLua, -1));

    lua_pop(aLua, 1);
}

void eActor::callLuaFuncThroughInheritanceHierarchyBackward(lua_State* aLua, const char* aFunctionName)
{
    const std::list<sModule*>& ih = iModule->iInheritanceHierarchy;

    auto moduleItBeg = ih.crbegin();
    auto moduleItEnd = ih.crend();

    auto envIt = iMeRef.crbegin();

    for (auto it = moduleItBeg; it != moduleItEnd; ++it, ++envIt) {
	sModule* m = *it;
	callLuaFuncWithEnv(aLua, m->iRef, *envIt, aFunctionName);
    }

    callLuaFuncWithEnv(aLua, iModule->iRef, *envIt, aFunctionName);

    assert(++envIt == iMeRef.crend());
}

void eActor::callOnInit(lua_State* aLua)
{
    callLuaFuncThroughInheritanceHierarchyBackward(aLua, "OnInit");
}

void eActor::callOnRestart(lua_State* aLua)
{
    callLuaFuncThroughInheritanceHierarchyBackward(aLua, "OnRestart");
}


void eActor::addGadget(eGadget* aGadget)
{
    iGadgets.push_back(aGadget);
}

void eActor::beginGadget()
{
    for (eGadget* g : iGadgets)
	g->begin();
}
