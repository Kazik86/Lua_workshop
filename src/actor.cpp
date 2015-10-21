#include "actor.h"

#include "gadget.h"
#include "game.h"
#include "luaState.h"

#include <cassert>
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

    int getY(lua_State* aLua)
    {
	const eActor* me = Script::getUdata<eActor>(aLua);
	lua_pushnumber(aLua, me->getPos().y);
	return 1;
    }
}

DEFINE_USERDATA_API(eActor)
{
    {"getScript", ::scriptName},
    {"getGadgetsNum", ::gadgetsNum},
    {"getY", ::getY},
    {"shift", ::shift},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eActor)

eActor::eActor(const std::string& aScript):
    iFsm(*this),
    iScript(aScript),
    iPos(0, 0),
    iRot(0)
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
    beginGadget();
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
                    if (m == iModule)
                        callLuaFuncShallow(aLua, iModule, iMeRef.front(), snippetName.c_str(), true);
                    else if (rtu.isAffectDerived() && eLuaModuleMgr::getMe()->isOnInheritanceList(iModule, m))
                        callLuaFuncDeep(aLua, iModule, iMeRef.front(), snippetName.c_str());
                }
            }
#endif


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

    if(lua_pcall(aLua, 1, 0, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(aLua, -1));

    lua_pop(aLua, 1);
}

bool eActor::funcExistsInModule(lua_State* aLua, int aModuleRef, const char* aFunctionName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);

    /* Do odnalezienia funkcji nie mogę posłużyć się 'lua_getfield', bo ta
     * funkcja przeszukuje całą hierarchię dziedziczenia, a mnie interesuje
     * tylko moduł na samym dole hierarchii.
     */
    
    lua_pushstring(aLua, aFunctionName);
    lua_rawget(aLua, -2);

    bool isFun = lua_isfunction(aLua, -1);
    lua_pop(aLua, 2);

    return isFun;
}

/* 'shallow' oznacza, że interesuje nas tylko i wyłącznie funkcja zdefiniowana
 * w module na samym dole hierarchii dziedziczenia. Jeśli jej tam nie ma
 * zgłaszam błąd bez szukania jej w klasach bazowych.
 */
void eActor::callLuaFuncShallow(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName, bool aThrow)
{
    if (funcExistsInModule(aLua, aModule->iRef, aFunctionName))
	/* funkcja istnieje w module głównym, wywołam ją za pomocą wersji
	 * 'deep' tzn. za pośrednictwem metatablicy aby ustawić jej odpowiednie
	 * _ENV
	 */
	callLuaFuncDeep(aLua, aModule, aMeRef, aFunctionName);
    else { 
	if (aThrow)
	    throw std::runtime_error(aModule->iScript + ": no function with name '" + aFunctionName + "'");
    }
}

void eActor::callLuaFuncThroughInheritanceHierarchyBackward(lua_State* aLua, const char* aFunctionName)
{
    const std::list<sModule*>& ih = iModule->iInheritanceHierarchy;

    auto moduleItBeg = ih.crbegin();
    auto moduleItEnd = ih.crend();

    auto envIt = iMeRef.crbegin();

    for (auto it = moduleItBeg; it != moduleItEnd; ++it, ++envIt) {
	sModule* m = *it;
	callLuaFuncShallow(aLua, m, *envIt, aFunctionName, false);
    }

    callLuaFuncShallow(aLua, iModule, *envIt, aFunctionName, false);

    assert(++envIt == iMeRef.crend());
}

void eActor::callInit(lua_State* aLua)
{
    callLuaFuncThroughInheritanceHierarchyBackward(aLua, "Init");
}

void eActor::callRestart(lua_State* aLua)
{
    callLuaFuncThroughInheritanceHierarchyBackward(aLua, "Restart");
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

    if(lua_pcall(aLua, 2, 1, 0) != LUA_OK)
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

