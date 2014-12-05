#include "luaState.h"

#include <stdexcept>

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
