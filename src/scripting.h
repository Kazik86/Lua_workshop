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


#define DEFINE_GADGET_METHODS(aClass)             \
    const struct luaL_Reg aClass::iMethods[] =

template <typename T>
T* getMe(lua_State* aLua)
{
    luaL_checktype(aLua, 1, LUA_TLIGHTUSERDATA);
    return static_cast<T*>(lua_touserdata(aLua, 1));
}

#endif // SCRIPTING_H
