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

// it presumes that function is already on the top of the stack and needs
// environment change. Using this function only makes sense for calling lua
// functions via registry index i.e:
//	lua_rawgeti(aLua, LUA_REGISTRYINDEX, iFunctionRef);
// If you want to call function using its name i.e:
//	lua_getfield(aLua, -1, aFunctionName);
// use plain 'lua_pcall' instead. In this case environment
// replacement is already done thanks to metatables. There is no point in
// doing this twice.
int eLuaState::callLuaFunWithEnv(lua_State* aLua, int aEnvRef, int aMeRef)
{
    replaceEnv(aLua, aEnvRef);
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aMeRef);
    if (lua_pcall(aLua, 1, 1, 0) != LUA_OK)
	throw std::runtime_error(lua_tostring(aLua, -1));

    int ret = static_cast<int>(lua_tointeger(aLua, -1));
    lua_pop(aLua, 1);

    return ret;
}

void eLuaState::replaceEnv(lua_State* aLua, int aEnvRef)
{
    for (int i = 1; ; ++i) {
	const char* upvalueName = lua_getupvalue(aLua, -1, i);
	if (upvalueName == 0)
	    break;
	else if(strcmp(upvalueName, "_ENV") == 0) {
	    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aEnvRef);
	    lua_setupvalue(aLua, -3, i);
	    lua_pop(aLua, 1);
	    break;
	}

	lua_pop(aLua, 1);
    }
}

int eLuaState::loadFile(lua_State* aLua, const std::string& aPath)
{
    ::memset(iBuf, 0, sizeof(iBuf));
    SDL_RWops* file = SDL_RWFromFile(aPath.c_str(), "r");
    std::string errorMsg = "eLuaState::loadFile, " + aPath;

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
