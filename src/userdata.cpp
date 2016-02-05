#include "userdata.h"

#include "game.h"
#include "luaState.h"

eLuaTable::eLuaTable():
    iRef(LUA_NOREF)
{
    lua_State* lua = eGame::getMe()->getLua()->getRaw();
    lua_newtable(lua);
    iRef = luaL_ref(lua, LUA_REGISTRYINDEX);
}

eLuaTable::~eLuaTable()
{
    luaL_unref(eGame::getMe()->getLua()->getRaw(), LUA_REGISTRYINDEX, iRef);
}

sEvent::sEvent():
    iEventRef(LUA_NOREF)
{
}

sEvent::~sEvent()
{
    luaL_unref(eGame::getMe()->getLua()->getRaw(), LUA_REGISTRYINDEX, iEventRef);
}
