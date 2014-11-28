#include "luaModule.h"

#include "luaState.h"

#include <cassert>
#include <list>
#include <stdexcept>
#include <unordered_map>

namespace
{
    struct sModule
    {
	sModule():
	    iRef(LUA_NOREF),
	    iLua(0)
	{}

	~sModule()
	{
	    luaL_unref(iLua, LUA_REGISTRYINDEX, iRef);
	}

	int iRef;
	lua_State* iLua;
	std::string iScript;
	std::list<sModule*> iInheritanceHierarchy;
    };

    std::unordered_map<std::string, sModule> iModules;

    // functions //////////////////////////////////////////////////////

    std::list<sModule*> derive(lua_State* aLua);
    sModule& add(lua_State* aLua, const std::string& aName);

    // [-0, +0]
    void setClass(lua_State* aLua)
    {
	lua_getfield(aLua, -1, "Class");

	if (! lua_isstring(aLua, -1))
	    throw std::runtime_error("Non-existent, empty or wrong type in field 'Class'.");

	lua_pushvalue(aLua, -2);
	lua_settable(aLua, -3);
    }

    // [-0, +1]
    bool require(lua_State* aLua, const char* aPath)
    {
	lua_getfield(aLua, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(aLua, -1, aPath);
	bool alreadyLoaded = lua_toboolean(aLua, -1);

	lua_pop(aLua, 2);

	lua_getglobal(aLua, "require");
	lua_pushstring(aLua, aPath);

	if (lua_pcall(aLua, 1, 1, 0) != LUA_OK)
	    throw std::runtime_error(lua_tostring(aLua, -1));

	if(! lua_istable(aLua, -1))
	    throw std::runtime_error("Actor's script must return a table.");

	return alreadyLoaded;
    }

    // [-0, +0]
    std::list<sModule*> derive(lua_State* aLua)
    {
	std::list<sModule*> inheritanceHierarchy;
	lua_getfield(aLua, -1, "Derives");

	if (lua_isstring(aLua, -1)) {
	    lua_newtable(aLua);
	    sModule& super = add(aLua, lua_tostring(aLua, -2));
	    lua_rawgeti(aLua, LUA_REGISTRYINDEX, super.iRef);
	    lua_setfield(aLua, -2, "__index");
	    lua_setmetatable(aLua, -3);

	    inheritanceHierarchy = super.iInheritanceHierarchy;
	    inheritanceHierarchy.push_front(&super);

	} else if (! lua_isnil(aLua, -1))
	    throw std::runtime_error("'Derives' field must be a table.");

	lua_pop(aLua, 1);

	return inheritanceHierarchy;
    }

    sModule* checkClassUniqueness(lua_State* aLua, sModule& aModule)
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

    // [-0, +0]
    sModule& add(lua_State* aLua, const std::string& aName)
    {
	try {
	    sModule& m = iModules[aName];

	    if (m.iRef == LUA_NOREF) {
		if (require(aLua, aName.c_str()))
		    // gdy skrypt A dziedziczy po skrypcie, który znów dziedziczy po A
		    throw std::runtime_error("Circular inheritance detected!");

		setClass(aLua);
		m.iInheritanceHierarchy = derive(aLua);
		m.iRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
		m.iLua = aLua;
		m.iScript = aName;

		if (sModule* redef = checkClassUniqueness(aLua, m))
		    throw std::runtime_error("Class defined in " + m.iScript + " redefinied in " + redef->iScript);
	    }

	    return m;

	} catch (const std::exception& aErr) {
	    throw std::runtime_error("While loading module '" + aName + "': " + aErr.what());
	}
    }
}

int LuaModuleMgr::load(eLuaState& aLua, const std::string& aName)
{
    return add(aLua.getRaw(), aName).iRef;
}

int LuaModuleMgr::getModule(const std::string& aName)
{
    return iModules[aName].iRef;
}

void LuaModuleMgr::clear()
{
    iModules.clear();
}
