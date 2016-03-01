#include "actor.h"

#include "gadget.h"
#include "game.h"
#include "luaState.h"

#include <cassert>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <vector>

#ifdef KPR_REAL_TIME_UPDATE
#include <iostream>
#endif

const int KStateShift = 1;

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

    int enable(lua_State* aLua)
    {
        eActor* me = Script::getUdata<eActor>(aLua);
        me->enable();
        return 0;
    }

    int disable(lua_State* aLua)
    {
        eActor* me = Script::getUdata<eActor>(aLua);
        me->disable();
        return 0;
    }

}

DEFINE_USERDATA_API(eActor)
{
    {"disable", ::disable},
    {"enable", ::enable},
    {"getScript", ::scriptName},
    {"getGadgetsNum", ::gadgetsNum},
    {"shift", ::shift},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eActor)

eActor::eActor(const std::string& aScript, size_t aId, size_t aParentId):
    iId(aId),
    iChildNum(0),
    iParentId(aParentId),
    iFsm(*this),
    iScript(aScript),
    iEnabled(true)
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
    callInit(aLua);
    beginGadget(aLua);
    shiftToEntryState(aLua);
}

void eActor::update(lua_State* aLua, float aDelta)
{
#ifdef KPR_REAL_TIME_UPDATE
    while (1) {
        try {
            const eRealTimeUpdate& rtu = eGame::getMe()->getRtu();
            const sModule* m = rtu.getModule();

            if (m) {
                const std::string& snippetName = rtu.getSnippetName();

                if (snippetName.empty()) {
                    if (m == iModule || eLuaModuleMgr::getMe()->isOnInheritanceList(iModule, m))
                        reenterState(aLua);
                } else {
                    if (m == iModule) {
                        if (! callLuaFuncShallow(aLua, iModule, iMeRef.front(), snippetName.c_str()))
                            throw std::runtime_error(iModule->iScript + ": no function with name '" + snippetName.c_str() + "'");
                    }
                    else if (rtu.isAffectDerived() && eLuaModuleMgr::getMe()->isOnInheritanceList(iModule, m))
                        callLuaFuncDeep(aLua, iModule, iMeRef.front(), snippetName.c_str());
                }
            }
#endif


            if (! iEnabled) return;

            for (eGadget* g : iGadgets) {
                if (g->isEnabled())
                    if (g->update(aLua, aDelta) == KStateShift)
                        return;
            }

            iFsm.update(aLua);
            return;


#ifdef KPR_REAL_TIME_UPDATE
        } catch (const std::exception& aErr) {
            std::cout << "-- RTU session; eActor::update -----------------------------------------\n"
                      << aErr.what() << std::endl;

            int c = '\0';
            while (c != 'a' && c != 'r') {
                std::cout << "[a]bort or [r]etry? ";
                c = std::cin.get();
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (c == 'a') throw;
            eGame::getMe()->getRtu().update();
            lua_settop(aLua, 0);
        }
    }
#endif
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

    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iModule->iRef);
    lua_setfield(aLua, -2, "Env"); // me.Env = module

    lua_pushunsigned(aLua, iId);
    lua_setfield(aLua, -2, "Id"); // me.Id = iId

    lua_pushvalue(aLua, -1);

    // from inheritance
    for (const sModule* m : iModule->iInheritanceHierarchy) {
	lua_newtable(aLua);
	lua_pushvalue(aLua, -1);
	lua_setfield(aLua, -2, m->iClass.c_str()); // me.DerivedClass = DerivedMe

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
    callLuaFuncDeep(aLua, iModule, iMeRef.front(), aFunctionName);
}

/* wersja 'deep' w przeciwieństwie do 'shallow' w poszukiwaniu funkcji
 * przeszukuje całą hierarchię dziedziczenia.
 */
void eActor::callLuaFuncDeep(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModule->iRef);
    lua_getfield(aLua, -1, aFunctionName);

    if (! lua_isfunction(aLua, -1))
	throw std::runtime_error(aModule->iScript + ": no function with name '" + aFunctionName + "'");

    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aMeRef);

    if(eLuaState::pcall(aLua, 1, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(aLua, -1));

    lua_pop(aLua, 1);
}

/* 'shallow' oznacza, że interesuje nas tylko i wyłącznie funkcja zdefiniowana
 * w module na samym dole hierarchii dziedziczenia. Jeśli jej tam nie ma
 * zgłaszam błąd bez szukania jej w klasach bazowych.
 */
bool eActor::callLuaFuncShallow(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModule->iRef);
    lua_pushstring(aLua, aFunctionName);
    lua_rawget(aLua, -2);

    bool funExists = false;

    if (lua_isfunction(aLua, -1)) {
        funExists = true;
        lua_rawgeti(aLua, LUA_REGISTRYINDEX, aMeRef);

        if(eLuaState::pcall(aLua, 1, 0) != LUA_OK)
            throw std::runtime_error(lua_tostring(aLua, -1));
    }

    lua_pop(aLua, 1);

    return funExists;
}

bool eActor::callLuaFuncShallow_2(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModule->iRef);
    lua_pushstring(aLua, aFunctionName);
    lua_rawget(aLua, -2);

    bool funExists = false;

    if (lua_isfunction(aLua, -1)) {
        funExists = true;
        lua_rawgeti(aLua, LUA_REGISTRYINDEX, aMeRef);
        lua_pushvalue(aLua, 3);

        if(eLuaState::pcall(aLua, 2, 0) != LUA_OK)
            throw std::runtime_error(lua_tostring(aLua, -1));
    }

    lua_pop(aLua, 1);

    return funExists;
}

void eActor::callLuaFuncThroughInheritanceHierarchyBackward(lua_State* aLua, const char* aFunctionName, TLuaCaller aFun)
{
    const std::list<sModule*>& ih = iModule->iInheritanceHierarchy;

    auto moduleItBeg = ih.crbegin();
    auto moduleItEnd = ih.crend();

    auto envIt = iMeRef.crbegin();

    for (auto it = moduleItBeg; it != moduleItEnd; ++it, ++envIt) {
	sModule* m = *it;
	(this->*aFun)(aLua, m, *envIt, aFunctionName);
    }

    (this->*aFun)(aLua, iModule, *envIt, aFunctionName);

    assert(++envIt == iMeRef.crend());
}

void eActor::callInit(lua_State* aLua)
{
    callLuaFuncThroughInheritanceHierarchyBackward(aLua, "Init", &eActor::callLuaFuncShallow_2);
}

void eActor::callRestart(lua_State* aLua)
{
    callLuaFuncThroughInheritanceHierarchyBackward(aLua, "Restart", &eActor::callLuaFuncShallow);
}


void eActor::addGadget(eGadget* aGadget)
{
    iGadgets.push_back(aGadget);
}

void eActor::beginGadget(lua_State* aLua)
{
    for (eGadget* g : iGadgets)
	g->begin(aLua);
}

void eActor::shiftToEntryState(lua_State* aLua)
{
    shiftToState(aLua, "EntryState");
}

void eActor::shiftToState(lua_State* aLua, const char* aName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iModule->iRef);
    lua_getfield(aLua, -1, "Shift");
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iMeRef.front());
    lua_getfield(aLua, -3, aName);

    if(eLuaState::pcall(aLua, 2, 1) != LUA_OK)
	throw std::runtime_error(iScript + ": while entering into '" + aName + "' - " + lua_tostring(aLua, -1));
    
    lua_pop(aLua, 2);
}

void eActor::reenterState(lua_State* aLua)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iFsm.getStateRef());
    lua_getfield(aLua, -1, "FullName");

    shiftToState(aLua, lua_tostring(aLua, -1));

    lua_pop(aLua, 2);
}

eGadget* eActor::findGadgetByClass(const char* aClass) const
{
    for (eGadget* g : iGadgets) {
        if (::strcmp(g->getClassName(), aClass) == 0)
            return g;
    }

    return 0;
}
