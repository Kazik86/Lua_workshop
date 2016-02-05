#include "luaState.h"

#include <cstring>
#include <stdexcept>

#include <SDL2/SDL_rwops.h>

eLuaState::eLuaState():
    iState(0)
{
    iState = luaL_newstate();
    if (! iState)
	throw std::runtime_error("eLuaState: luaL_newstate failed.");

    openBaseLibs();
    openGadgetLibs();
}

eLuaState::~eLuaState()
{
    lua_close(iState);
}

void eLuaState::openBaseLibs()
{
    openlibs(iBaseLibs);
}

void eLuaState::openGadgetLibs()
{
    openlibs(iGadgetLibs);
}

/* This function was copied from Lua source file linit.c and slightly modified.
 * Original function loads hardcoded set of libraries. This version lets me load
 * any set of libraries.
 */
void eLuaState::openlibs(const luaL_Reg* aLibs)
{
  const luaL_Reg *lib;
  /* call open functions from 'loadedlibs' and set results to global table */
  for (lib = aLibs; lib->func; lib++) {
    luaL_requiref(iState, lib->name, lib->func, 1);
    lua_pop(iState, 1);  /* remove lib */
  }
}

void eLuaState::stackDump(lua_State* aLua)
{
    printf("%s\n", "STACK DUMP BEGIN:");
    int top = lua_gettop(aLua);

    for(int i = 1; i <= top; i++)
    {  
	printf("%d: ", i);
	int t = lua_type(aLua, i);
       
	switch(t)
	{
	case LUA_TSTRING:
	    printf("'%s'", lua_tostring(aLua, i));
	    break;
	    
	case LUA_TBOOLEAN:
	    printf(lua_toboolean(aLua, i) ? "true" : "false");
	    break;
	    
	case LUA_TNUMBER:
	    printf("%g", lua_tonumber(aLua, i));
	    break;
	    
	default:
	    printf("%s\t%p", lua_typename(aLua, t), lua_topointer(aLua, i));
	    break;
	}
	printf("\n");
    }
    printf("%s\n", "STACK DUMP END");
}

void eLuaState::tableDump(lua_State* aLua, int aIdx)
{
    printf("%s\n", "TABLE DUMP BEGIN:");

    if (! lua_istable(aLua, aIdx)) {
        printf("value at index '%d' is not a table!\n", aIdx);
        printf("%s\n", "TABLE DUMP END");
        return;
    }

    lua_pushnil(aLua);

    int idx = aIdx > 0 ? aIdx : aIdx - 1;

    while (lua_next(aLua, idx) != 0) {
        printf("key type '%s': ", lua_typename(aLua, lua_type(aLua, -2))); printValue(aLua, -2); printf(";\t val type '%s': ", lua_typename(aLua, lua_type(aLua, -1))); printValue(aLua, -1); printf("\n");
        lua_pop(aLua, 1);
    }

    printf("%s\n", "TABLE DUMP END");
}

void eLuaState::printValue(lua_State* aLua, int aIdx)
{
    int t = lua_type(aLua, aIdx);

    switch(t)
    {
        case LUA_TSTRING:
            printf("'%s'", lua_tostring(aLua, aIdx));
            break;

        case LUA_TBOOLEAN:
            printf(lua_toboolean(aLua, aIdx) ? "true" : "false");
            break;

        case LUA_TNUMBER:
            printf("%g", lua_tonumber(aLua, aIdx));
            break;

        default:
            printf("%p", lua_topointer(aLua, aIdx));
            break;
    }
}

int eLuaState::loadFile(lua_State* aLua, const std::string& aPath)
{
    ::memset(iBuf, 0, sizeof(iBuf));
    SDL_RWops* file = SDL_RWFromFile(aPath.c_str(), "r");
    // "@" meaning is described in 'source' field documentation of 'lua_Debug'
    // structure in RM
    std::string errorMsg = "@" + aPath;

    if (file == 0)
	throw std::runtime_error(errorMsg + ": " + SDL_GetError());

    auto fileSize = SDL_RWsize(file);
    if (fileSize > sizeof(iBuf)) {
	SDL_RWclose(file);
	throw std::runtime_error(errorMsg + ": File too large to load." );
    }

    if (SDL_RWread(file, iBuf, fileSize, 1) != 1) {
	SDL_RWclose(file);
	throw std::runtime_error(errorMsg + ": " + SDL_GetError());
    }
    
    SDL_RWclose(file);
    return luaL_loadbuffer(aLua, iBuf, fileSize, errorMsg.c_str());
}

// this function is copied from 'lua.c'
int eLuaState::traceback(lua_State* aLua)
{
    const char* msg = lua_tostring(aLua, 1);

    if (msg)
        luaL_traceback(aLua, aLua, msg, 1);
    else if (!lua_isnoneornil(aLua, 1)) {  /* is there an error object? */
        if (!luaL_callmeta(aLua, 1, "__tostring"))  /* try its 'tostring' metamethod */
            lua_pushliteral(aLua, "(no error message)");
    }

    return 1;
}

// based on 'docall' from 'lua.c'
int eLuaState::pcall(lua_State* aLua, int aNarg, int aNres)
{
    int base = lua_gettop(aLua) - aNarg;  /* function index */
    lua_pushcfunction(aLua, eLuaState::traceback);  /* push traceback function */
    lua_insert(aLua, base);  /* put it under chunk and args */
    int status = lua_pcall(aLua, aNarg, aNres, base);
    lua_remove(aLua, base);  /* remove traceback function */
    return status;
}
