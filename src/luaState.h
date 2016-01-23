#ifndef LUA_STATE_H
#define LUA_STATE_H

#include <lua.hpp>
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
    static int callLuaFun(lua_State* aLua, int aMeRef);
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

#endif // LUA_STATE_H
