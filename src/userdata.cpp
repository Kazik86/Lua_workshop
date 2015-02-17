#include "userdata.h"

#include "game.h"
#include "luaState.h"

sEvent::sEvent():
    iEventRef(LUA_NOREF)
{
}

sEvent::~sEvent()
{
    luaL_unref(eGame::getMe()->getLua()->getRaw(), LUA_REGISTRYINDEX, iEventRef);
}
