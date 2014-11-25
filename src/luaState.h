#ifndef LUA_STATE_H
#define LUA_STATE_H

#include <lua.hpp>

class eLuaState
{
public:
    eLuaState();
    ~eLuaState();

    lua_State* getRaw() { return iState; }
    static void stackDump(lua_State* aLua);

private:
    eLuaState(const eLuaState& aOther);
    eLuaState& operator=(const eLuaState& aOther);

    void openBaseLibs();
    void openGadgetLibs();
    void openlibs(const luaL_Reg* aLibs);

private:
    lua_State* iState;
    static const luaL_Reg iBaseLibs[];
    static const luaL_Reg iGadgetLibs[];
};

#endif // LUA_STATE_H
