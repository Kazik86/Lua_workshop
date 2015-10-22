#include "luaModule.h"

#include "game.h"
#include "luaState.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace
{

int derive(lua_State* aLua)
{
    luaL_checkstring(aLua, 1);

    const sModule& super = eLuaModuleMgr::getMe()->load(aLua, lua_tostring(aLua, 1));
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, super.iRef);

    return 1;
}

}

DEFINE_USERDATA_API(eLuaModuleMgr)
{
    {"derive", ::derive},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eLuaModuleMgr)

sModule::sModule():
    iRef(LUA_NOREF),
    iLua(0),
    iChunkRef(LUA_NOREF)
{}

sModule::~sModule()
{
    luaL_unref(iLua, LUA_REGISTRYINDEX, iRef);
    luaL_unref(iLua, LUA_REGISTRYINDEX, iChunkRef);
}

eLuaModuleMgr* eLuaModuleMgr::iMe = 0;

eLuaModuleMgr::eLuaModuleMgr()
{
    if (iMe)
	throw std::runtime_error("eLuaModuleMgr: multiple instances not allowed.");

    iMe = this;
}

eLuaModuleMgr::~eLuaModuleMgr()
{
    iMe = 0;
}

void eLuaModuleMgr::setClass(lua_State* aLua, sModule& aModule)
{
    // na stosie leżą:
    // -1: tablica główna;
    lua_getfield(aLua, -1, "Class");

    if (! lua_isstring(aLua, -1))
	throw std::runtime_error("'Class' not defined or not a string.");

    /* spr. czy nazwa klasy nie jest użyta jako identyfikator np.
     * Class = "Foo"
     * Foo = 1
     */
    lua_pushvalue(aLua, -1);
    lua_rawget(aLua, -3);
    if (! lua_isnil(aLua, -1))
	throw std::runtime_error("Class name is used as an identifier.");

    lua_pop(aLua, 1);

    aModule.iClass = lua_tostring(aLua, -1);

    lua_pushvalue(aLua, -2);
    lua_settable(aLua, -3);
}

void eLuaModuleMgr::setGlobal(lua_State* aLua)
{
    lua_pushglobaltable(aLua);
    lua_setfield(aLua, -2, "_G");
}

void eLuaModuleMgr::setScript(lua_State* aLua, const std::string& aScript)
{
    lua_pushstring(aLua, aScript.c_str());
    lua_setfield(aLua, -2, "Script");
}

sModule* eLuaModuleMgr::checkClassUniqueness(lua_State* aLua, sModule& aModule)
{
    sModule* result = 0;
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModule.iRef);
    lua_getfield(aLua, -1, "Class");

    // if it is called then there is smth wrong with setClass
    assert(lua_isstring(aLua, -1));

    std::string c = lua_tostring(aLua, -1);

    for (sModule* m : aModule.iInheritanceHierarchy) {
	lua_rawgeti(aLua, LUA_REGISTRYINDEX, m->iRef);
	lua_getfield(aLua, -1, "Class");

	assert(lua_isstring(aLua, -1));

	if (c == lua_tostring(aLua, -1))
	    result = m;

	lua_pop(aLua, 2);

	if (result) break;
    }

    lua_pop(aLua, 2);

    return result;
}

bool eLuaModuleMgr::entryStateExists(lua_State* aLua, sModule& aModule)
{
    bool result = true;

    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModule.iRef);
    lua_getfield(aLua, -1, "EntryState");

    if (lua_isnil(aLua, -1) || ! lua_istable(aLua, -1))
	result = false;

    lua_pop(aLua, 2);
    
    return result;
}

sModule& eLuaModuleMgr::add(lua_State* aLua, const std::string& aName)
{
    try {
	sModule& m = iModules[aName];

	if (m.iRef == LUA_NOREF) {
	    if (m.iLua != 0) {
		// m.iLua pełni rolę strażnika zapętlonego dziedziczenia
		// tzn. gdy skrypt A dziedziczy po skrypcie, który znów
		// dziedziczy po A
		throw std::runtime_error("Circular inheritance detected!");
	    }

            // needed for rtu after one of the following exceptions thrown
            auto resetGuardFn = [&](sModule*) { auto it = iModules.find(aName); iModules.erase(it); };
            std::unique_ptr<sModule, decltype(resetGuardFn)> resetGuard(&m, resetGuardFn);

	    m.iLua = aLua;

	    /* główna tablica Aktora, zawiera pola:
	     *     __index - bo tablica główna będzie jednocześnie swoją metatablicą;
	     *     Script - ścieżka skryptu;
	     *     'NazwaKlasy' - ponownie adres samej siebie
	     *     - wszystko to co zdefiniowano z poziomu skryptu;
	     *     - namespace'y używane w skryptach np.: _G
	     *     - wskaźnik na samą siebie 'This';
	     */
	    lua_newtable(aLua);
	    setScript(aLua, aName);

	    lua_pushvalue(aLua, -1);
	    lua_setfield(aLua, -2, "This");

	    setGlobal(aLua);

	    // główna tablica jest jednocześnie swoją metatablicą
	    lua_pushvalue(aLua, -1);
	    lua_setmetatable(aLua, -2);

	    if (eGame::getMe()->getLua()->loadFile(aLua, aName) != LUA_OK)
		throw std::runtime_error(lua_tostring(aLua, -1));

#ifdef KPR_REAL_TIME_UPDATE
            saveChunk(aLua, m);
#endif

	    lua_pushvalue(aLua, -2);
	    lua_setupvalue(aLua, -2, 1);

	    if (lua_pcall(aLua, 0, 0, 0) != LUA_OK)
		throw std::runtime_error(lua_tostring(aLua, -1));

	    // hierarchia dziedziczenia
	    lua_getfield(aLua, -1, "Super");

	    if (lua_istable(aLua, -1)) {
		// Env[__index] = Super
		lua_pushvalue(aLua, -1);
		lua_setfield(aLua, -3, "__index");

		lua_getfield(aLua, -1, "Script");
		sModule& super = iModules[std::string(lua_tostring(aLua, -1))];
		lua_pop(aLua, 1);

		m.iInheritanceHierarchy = super.iInheritanceHierarchy;
		m.iInheritanceHierarchy.push_front(&super);
	    } else if (! lua_isnil(aLua, -1))
		throw std::runtime_error("'Super' must be a table.");

	    lua_pop(aLua, 1);

	    setClass(aLua, m);
            registerInGlobalEnv(aLua); // must be called after 'setClass' because relies on data validated in 'setClass'

	    m.iRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
	    m.iScript = aName;

	    // various tests
	    if (sModule* redef = checkClassUniqueness(aLua, m))
		throw std::runtime_error("Class defined in " + m.iScript + " redefinied in " + redef->iScript);

	    if (! entryStateExists(aLua, m))
		throw std::runtime_error(m.iScript + ": 'EntryState' not defined or not a table (or you just forgot to derive from Actor.lua).");

            resetGuard.release();
	}

	return m;

    } catch (const std::exception& aErr) {
	throw std::runtime_error("While loading module '" + aName + "': " + aErr.what());
    }
}

const sModule* eLuaModuleMgr::reloadModule(lua_State* aLua, const std::string& aModule)
{
    auto mod = iModules.find(aModule);

    if (mod == iModules.end())
        throw std::runtime_error("module not found");

    if (eGame::getMe()->getLua()->loadFile(aLua, aModule) != LUA_OK)
        throw std::runtime_error(lua_tostring(aLua, -1));

    saveChunk(aLua, mod->second);
    callChunk(aLua, mod->second.iRef);

    for (const auto& m : iModules) {
        if (isOnInheritanceList(&m.second, &mod->second)) {
            lua_rawgeti(aLua, LUA_REGISTRYINDEX, m.second.iChunkRef);
            callChunk(aLua, m.second.iRef);
        }
    }

    return &(mod->second);
}

const sModule* eLuaModuleMgr::callSnippet(lua_State* aLua, const std::string& aModule, const std::string& aFile)
{
    auto mod = iModules.find(aModule);

    if (mod == iModules.end())
        throw std::runtime_error("module not found");

    if (eGame::getMe()->getLua()->loadFile(aLua, aFile) != LUA_OK)
        throw std::runtime_error(lua_tostring(aLua, -1));

    callChunk(aLua, mod->second.iRef);

    return &(mod->second);
}

bool eLuaModuleMgr::isOnInheritanceList(const sModule* aModule, const sModule* aFind)
{
    for (const sModule* m : aModule->iInheritanceHierarchy) {
        if (m == aFind)
            return true;
    }

    return false;
}

void eLuaModuleMgr::saveChunk(lua_State* aLua, sModule& aModule)
{
    lua_pushvalue(aLua, -1);
    aModule.iChunkRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
}

void eLuaModuleMgr::callChunk(lua_State* aLua, int aModuleRef)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);
    lua_setupvalue(aLua, -2, 1);

    if (lua_pcall(aLua, 0, 0, 0) != LUA_OK)
        throw std::runtime_error(lua_tostring(aLua, -1));
}

void eLuaModuleMgr::removeSnippet(lua_State* aLua, int aModuleRef, const char* aSnippetName)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);
    lua_pushnil(aLua);
    lua_setfield(aLua, -2, aSnippetName);
    lua_pop(aLua, 1);
}

void eLuaModuleMgr::registerInGlobalEnv(lua_State* aLua)
{
    // na stosie leżą:
    // -1: tablica główna;
    lua_pushglobaltable(aLua);
    lua_getfield(aLua, -2, "Class");
    lua_pushvalue(aLua, -3);
    lua_settable(aLua, -3); // _G.MyClass = tablica główna
    lua_pop(aLua, 1);
}
