#include "luaModule.h"

#include "luaState.h"

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace
{

int replaceEnv(lua_State* aLua)
{
    if (lua_getmetatable(aLua, 1)) {
	lua_getfield(aLua, -1, "Data");
	if (! lua_isnil(aLua, -1)) {
	    lua_pushvalue(aLua, 2);
	    lua_gettable(aLua, -2);

	    if (lua_isfunction(aLua, -1)) {
		/* nie każda funkcja musi posiadać jako upvalue swoje _ENV. Z tego co widzę
		 * z eksperymentów, _ENV nie mają np. funkcje, które operują tylko na
		 * przesłanych jej argumentach (brak odwołań do jakichkolwiek danych spoza
		 * zakresu funkcji). W dodatku nawet jeśli mamy _ENV to niekoniecznie jako
		 * upvalue na pozycji 1.
		 */
		for (int i = 1; ; ++i) {
		    const char* upvalueName = lua_getupvalue(aLua, -1, i);
		    if (upvalueName == 0)
			break;
		    else if(strcmp(upvalueName, "_ENV") == 0) {
			lua_pushvalue(aLua, 1);
			lua_setupvalue(aLua, -3, i);
			lua_pop(aLua, 1);
			break;
		    }

		    lua_pop(aLua, 1);
		}
	    }
	}
    } else
	lua_pushnil(aLua);

    return 1;
}

}

sModule::sModule():
    iRef(LUA_NOREF),
    iLua(0)
{}

sModule::~sModule()
{
    luaL_unref(iLua, LUA_REGISTRYINDEX, iRef);
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
    lua_getfield(aLua, -1, "Data");
    lua_getfield(aLua, -1, "Class");

    if (! lua_isstring(aLua, -1))
	throw std::runtime_error("Non-existent, empty or wrong type in field 'Class'.");

    /* spr. czy nazwa klasy nie jest użyta jako identyfikator np.
     * Class = "Foo"
     * Foo = 1
     */
    lua_pushvalue(aLua, -1);
    lua_gettable(aLua, -3);
    if (! lua_isnil(aLua, -1))
	throw std::runtime_error("Class name is used as an identifier.");

    lua_pop(aLua, 1);

    aModule.iClass = lua_tostring(aLua, -1);

    lua_pushvalue(aLua, -3);
    lua_settable(aLua, -4);
    lua_pop(aLua, 1);
}

void eLuaModuleMgr::setGlobal(lua_State* aLua)
{
    lua_pushglobaltable(aLua);
    lua_setfield(aLua, -2, "_G");
}

std::list<sModule*> eLuaModuleMgr::derive(lua_State* aLua)
{
    std::list<sModule*> inheritanceHierarchy;
    lua_getfield(aLua, -1, "Derives");

    if (lua_isstring(aLua, -1)) {
	lua_newtable(aLua);
	sModule& super = add(aLua, lua_tostring(aLua, -2));
	lua_rawgeti(aLua, LUA_REGISTRYINDEX, super.iRef);
	lua_setfield(aLua, -2, "Data");
	lua_pushcfunction(aLua, replaceEnv);
	lua_setfield(aLua, -2, "__index");
	lua_setmetatable(aLua, -3);

	inheritanceHierarchy = super.iInheritanceHierarchy;
	inheritanceHierarchy.push_front(&super);

    } else if (! lua_isnil(aLua, -1))
	throw std::runtime_error("'Derives' field must be a table.");

    lua_pop(aLua, 1);

    return inheritanceHierarchy;
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

	    m.iLua = aLua;

	    // główna tablica Aktora
	    lua_newtable(aLua);
	    // główna tablica będzie jednocześnie swoją metatablicą więc
	    // kopiujemy
	    lua_pushvalue(aLua, -1);

	    // do tablicy 'Data' załadowane zostaną wszystkie dane ze skryptu
	    lua_newtable(aLua);
	    lua_setfield(aLua, -2, "Data");
	    lua_pushcfunction(aLua, replaceEnv);
	    lua_setfield(aLua, -2, "__index");
	    lua_setmetatable(aLua, -2);

	    lua_getfield(aLua, -1, "Data");

	    if (luaL_loadfile(aLua, aName.c_str()) != LUA_OK)
		throw std::runtime_error(lua_tostring(aLua, -1));

	    lua_pushvalue(aLua, -2);
	    lua_setupvalue(aLua, -2, 1);

	    if (lua_pcall(aLua, 0, 0, 0) != LUA_OK)
		throw std::runtime_error(lua_tostring(aLua, -1));

	    m.iInheritanceHierarchy = derive(aLua);
	    lua_pop(aLua, 1);
	    setClass(aLua, m);
	    setGlobal(aLua);

	    m.iRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
	    m.iScript = aName;

	    if (sModule* redef = checkClassUniqueness(aLua, m))
		throw std::runtime_error("Class defined in " + m.iScript + " redefinied in " + redef->iScript);
	}

	return m;

    } catch (const std::exception& aErr) {
	throw std::runtime_error("While loading module '" + aName + "': " + aErr.what());
    }
}

