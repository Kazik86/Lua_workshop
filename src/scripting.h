#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <lua.hpp>

#define DECLARE_SCRIPT_SUPPORT()                  \
    public:                                       \
        static int luaOpen(lua_State* aLua);      \
                                                  \
    private:                                      \
        static const struct luaL_Reg iFunctions[];


#define DEFINE_SCRIPT_SUPPORT(aClass)             \
    int aClass::luaOpen(lua_State* aLua)          \
    {                                             \
	luaL_newlib(aLua, iFunctions);            \
	return 1;                                 \
    }

#define DEFINE_USERDATA_FUNCTIONS(aClass)         \
    const struct luaL_Reg aClass::iFunctions[] =


namespace Script
{

template <typename T>
inline T* getMe(lua_State* aLua)
{
    luaL_checktype(aLua, 1, LUA_TLIGHTUSERDATA);
    return static_cast<T*>(lua_touserdata(aLua, 1));
}

template <typename T>
inline T* getUdata(lua_State* aLua)
{
    return static_cast<T*>(luaL_checkudata(aLua, 1, T::iClassName));
}

template <typename T>
inline T* getUdata(lua_State* aLua, int aIdx)
{
    return static_cast<T*>(luaL_checkudata(aLua, aIdx, T::iClassName));
}

inline int getInt(lua_State* aLua, int aIdx)
{
    return luaL_checkint(aLua, aIdx);
}

inline float getNumber(lua_State* aLua, int aIdx)
{
    return static_cast<float>(luaL_checknumber(aLua, aIdx));
}

inline const char* getString(lua_State* aLua, int aIdx)
{
    return luaL_checkstring(aLua, aIdx);
}

}
#endif // SCRIPTING_H
