#include "fsm.h"

#include "luaState.h"

#include <stdexcept>

eFsm::eFsm(eLuaState& aLuaState, const std::string& aScript):
    iLuaState(aLuaState.getRaw()),
    iScript(aScript),
    iEnv(LUA_NOREF),
    iModule(LUA_NOREF)
{
}

eFsm::~eFsm()
{
    luaL_unref(iLuaState, LUA_REGISTRYINDEX, iEnv);
    luaL_unref(iLuaState, LUA_REGISTRYINDEX, iModule);
}

// [-0, +0]
void eFsm::setClass(const char* aPath)
{
    lua_getfield(iLuaState, -1, "Class");

    if (! lua_isstring(iLuaState, -1))
	throw std::runtime_error(std::string(aPath) + ": non-existent, empty or wrong type in field 'Class'.");

    lua_pushvalue(iLuaState, -2);
    lua_settable(iLuaState, -3);
}

// [-0, +1]
void eFsm::loadModule(const char* aPath)
{
    lua_getglobal(iLuaState, "require");
    lua_pushstring(iLuaState, aPath);

    if (lua_pcall(iLuaState, 1, 1, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(iLuaState, -1));

    if(! lua_istable(iLuaState, -1)) {
	std::string msg = std::string("module '") + aPath + "' must return a table.";
	throw std::runtime_error(msg);
    }

    setClass(aPath);
    deriveModule(aPath);
}

// [-0, +0]
void eFsm::deriveModule(const char* aPath)
{
    lua_getfield(iLuaState, -1, "Derives");

    if (lua_isstring(iLuaState, -1)) {
	const char* module = lua_tostring(iLuaState, -1);

	try {
	    loadModule(module);
	} catch (const std::exception& aErr) {
	    throw std::runtime_error(std::string("while loading 'Derives' for ") + aPath  + ": " + aErr.what());
	}

	lua_newtable(iLuaState);
	lua_pushvalue(iLuaState, -2);
	lua_setfield(iLuaState, -2, "__index");
	lua_setmetatable(iLuaState, -4);
	lua_pop(iLuaState, 1);

    } else if (! lua_isnil(iLuaState, -1)) {
	std::string msg = std::string("'Derives' field from module ") + aPath + " must be a table.";
	throw std::runtime_error(msg);
    }

    lua_pop(iLuaState, 1);
}

// [-0, +0]
void eFsm::doScript()
{
    loadModule(iScript.c_str());
    iModule = luaL_ref(iLuaState, LUA_REGISTRYINDEX);
}

// [-0, +0]
const void* eFsm::getModuleAsPtr() const
{
    lua_rawgeti(iLuaState, LUA_REGISTRYINDEX, iModule);
    const void* result = lua_topointer(iLuaState, -1);
    lua_pop(iLuaState, 1);
    
    return result;
}

// [-0, +3]
void eFsm::callLuaFunct(const char* aFunctName)
{
    lua_rawgeti(iLuaState, LUA_REGISTRYINDEX, iModule);
    lua_getfield(iLuaState, -1, aFunctName);
    lua_pcall(iLuaState, 0, 1, 0);
}

void eFsm::update()
{

}

