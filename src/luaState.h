#ifndef LUA_STATE_H
#define LUA_STATE_H

#include <lua.hpp>
#include <stdexcept>
#include <string>

class eLuaState
{
public:
    eLuaState();
    ~eLuaState();

    lua_State* getRaw() { return iState; }
    static void stackDump(lua_State* aLua);
    static void tableDump(lua_State* aLua, int aIdx);
    static void printValue(lua_State* aLua, int aIdx);
    template <int N = 1> static int callLuaFun(lua_State* aLua);
    int loadFile(lua_State* aLua, const std::string& aPath);
    static int traceback(lua_State* aLua);
    static int pcall(lua_State* aLua, int aNarg, int aNres);

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
    char iBuf[128 * 1024];
};

// it presumes that function is already on top of the stack
template <int N>
int eLuaState::callLuaFun(lua_State* aLua)
{
    if (eLuaState::pcall(aLua, N, 1) != LUA_OK)
        throw std::runtime_error(lua_tostring(aLua, -1));

    int ret = static_cast<int>(lua_tointeger(aLua, -1));
    lua_pop(aLua, 1);

    return ret;
}

#endif // LUA_STATE_H
