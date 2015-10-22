#include "fsm.h"

#include "actor.h"
#include "game.h"
#include "luaState.h"

#include <stdexcept>

namespace
{
    // not used anymore but maybe someday I'm gonna need it
    const unsigned int KExecutingUpdate = 0x00000001;
    const unsigned int KExecutingLeave  = 0x00000002;
}

eFsm::eFsm(eActor& aActor):
    iActor(aActor),
    iFlags(0),
    iStateRef(LUA_NOREF),
    iUpdateRef(LUA_NOREF)
{
}

eFsm::~eFsm()
{
    lua_State* lua = eGame::getMe()->getLua()->getRaw();
    luaL_unref(lua, LUA_REGISTRYINDEX, iStateRef);
    luaL_unref(lua, LUA_REGISTRYINDEX, iUpdateRef);
}

void eFsm::shift(lua_State* aLua)
{
    luaL_checktype(aLua, 2, LUA_TTABLE);
    setName(aLua);
    saveStage(aLua, "UpdateEx", iUpdateRef);
    lua_pushvalue(aLua, 2);

    if (iStateRef == LUA_NOREF)
	iStateRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
    else
	lua_rawseti(aLua, LUA_REGISTRYINDEX, iStateRef);
}

void eFsm::saveStage(lua_State* aLua, const char* aName, /* in/out */ int& aRef)
{
    lua_getfield(aLua, 2, aName);
    if (lua_type(aLua, -1) != LUA_TFUNCTION)
	luaL_error(aLua, "'%s' in state '%s' not defined or not a function.", aName, iName.c_str());

    if (aRef == LUA_NOREF)
	aRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
    else
	lua_rawseti(aLua, LUA_REGISTRYINDEX, aRef);
}

void eFsm::update(lua_State* aLua)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iUpdateRef);
    eLuaState::callLuaFun(aLua, iActor.getMeRef());
}

void eFsm::setName(lua_State* aLua)
{
    lua_getfield(aLua, 2, "FullName");
    if (lua_type(aLua, -1) != LUA_TSTRING)
	luaL_error(aLua, "'FullName' is not defined. Usually it means you didn't use 'DefState' to create state.");

    iName = lua_tostring(aLua, -1);
    lua_pop(aLua, 1);
}
