#include "fsm.h"

#include "actor.h"
#include "game.h"
#include "luaState.h"

#include <cstring>
#include <stdexcept>

namespace
{
    const unsigned int KExecutingUpdate = 0x00000001;
    const unsigned int KExecutingLeave  = 0x00000002;
}

eFsm::eFsm(eActor& aActor):
    iActor(aActor),
    iFlags(0),
    iStateRef(LUA_NOREF),
    iEnterRef(LUA_NOREF),
    iUpdateRef(LUA_NOREF),
    iLeaveRef(LUA_NOREF)
{
}

eFsm::~eFsm()
{
    lua_State* lua = eGame::getMe()->getLua()->getRaw();
    luaL_unref(lua, LUA_REGISTRYINDEX, iStateRef);
    luaL_unref(lua, LUA_REGISTRYINDEX, iEnterRef);
    luaL_unref(lua, LUA_REGISTRYINDEX, iUpdateRef);
    luaL_unref(lua, LUA_REGISTRYINDEX, iLeaveRef);
}

void eFsm::shift(lua_State* aLua)
{
    if (iStateRef != LUA_NOREF) {
	//to zapobiega przepełnieniu stosu gdy ktoś w leave'ie stanu
	//spróbuje zrobić operację 'shift'
	if(checkFlag(KExecutingLeave))
	    return;

	setFlag(KExecutingLeave);
	leave(aLua);
	unsetFlag(KExecutingLeave);
    }

    setState(aLua);
    enter(aLua);
}

void eFsm::setState(lua_State* aLua)
{
    luaL_checktype(aLua, 2, LUA_TTABLE);

    saveStage(aLua, "enter", iEnterRef);
    saveStage(aLua, "update", iUpdateRef);
    saveStage(aLua, "leave", iLeaveRef);

    lua_pushvalue(aLua, 2);

    if (iStateRef == LUA_NOREF)
	iStateRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
    else
	lua_rawseti(aLua, LUA_REGISTRYINDEX, iStateRef);
}

void eFsm::saveStage(lua_State* aLua, const char* aName, /* in/out */ int& aRef)
{
    lua_getfield(aLua, 2, aName);
    luaL_checktype(aLua, -1, LUA_TFUNCTION);

    if (aRef == LUA_NOREF)
	aRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
    else
	lua_rawseti(aLua, LUA_REGISTRYINDEX, aRef);
}

void eFsm::update(lua_State* aLua)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iUpdateRef);
    setFlag(KExecutingUpdate);
    callFun(aLua);
    unsetFlag(KExecutingUpdate);
}

void eFsm::enter(lua_State* aLua)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iEnterRef);
    callFun(aLua);
}

void eFsm::leave(lua_State* aLua)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iLeaveRef);
    callFun(aLua);
}

void eFsm::callFun(lua_State* aLua)
{
    replaceEnv(aLua);
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iActor.getMeRef());
    if (lua_pcall(aLua, 1, 0, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(aLua, -1));
}

void eFsm::replaceEnv(lua_State* aLua)
{
    for (int i = 1; ; ++i) {
	const char* upvalueName = lua_getupvalue(aLua, -1, i);
	if (upvalueName == 0)
	    break;
	else if(strcmp(upvalueName, "_ENV") == 0) {
	    lua_rawgeti(aLua, LUA_REGISTRYINDEX, iActor.getModuleRef());
	    lua_setupvalue(aLua, -3, i);
	    lua_pop(aLua, 1);
	    break;
	}

	lua_pop(aLua, 1);
    }
}
