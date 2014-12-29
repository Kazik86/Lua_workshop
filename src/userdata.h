#ifndef USERDATA_H
#define USERDATA_H

#include <lua.hpp>
#include <string>

#define DECLARE_USERDATA_CLASS()                  \
    public:                                       \
        static int luaOpen(lua_State* aLua);      \
                                                  \
    private:                                      \
        static const struct luaL_Reg iMethods[];


#define DEFINE_USERDATA_CLASS(aClass)             \
    int aClass::luaOpen(lua_State* aLua)          \
    {                                             \
	luaL_newlib(aLua, iMethods);              \
	return 1;                                 \
    }


#define DEFINE_USERDATA_API(aClass)               \
    const struct luaL_Reg aClass::iMethods[] =


#define DECLARE_USERDATA_PROPERTY(aName)                       \
	static int set##aName(lua_State*); \
        static int get##aName(lua_State*);


#define DEFINE_USERDATA_PROPERTY_COMMON(aClass, aName, aVar, aGetFun)      \
    int aClass::set##aName(lua_State* aLua)                                         \
    {                                                                       \
        aClass* me = Script::aGetFun<aClass>(aLua);                         \
        me->aVar = Script::getVal<decltype(me->aVar)>(aLua, 2);                     \
	return 0;                                                           \
    }                                                                       \
    								            \
    int aClass::get##aName(lua_State* aLua)                                         \
    {                                                                       \
        aClass* me = Script::aGetFun<aClass>(aLua);                         \
        Script::pushVal<decltype(me->aVar)>(aLua, me->aVar);                     \
        return 1;                                                           \
    }


#define DEFINE_USERDATA_PROPERTY(aClass, aName, aType)         \
    DEFINE_USERDATA_PROPERTY_COMMON(aClass, aName, aType, getUdata)


#define STRINGIZE(aParam) #aParam


#define REGISTER_USERDATA_PROPERTY(aName)    \
    { STRINGIZE(set##aName), set##aName }, \
    { STRINGIZE(get##aName), get##aName }

namespace Script
{

template <typename T>
inline T* getUdata(lua_State* aLua)
{
    luaL_checktype(aLua, 1, LUA_TLIGHTUSERDATA);
    return static_cast<T*>(lua_touserdata(aLua, 1));
}

template <typename T>
inline T* getGadget(lua_State* aLua)
{
    return static_cast<T*>(luaL_checkudata(aLua, 1, T::iClassName));
}

template <typename T>
inline T* getGadget(lua_State* aLua, int aIdx)
{
    return static_cast<T*>(luaL_checkudata(aLua, aIdx, T::iClassName));
}

template <typename T>
inline T getVal(lua_State* aLua, int aIdx)
{
    // z tego szablonu nigdy nie powinienem korzystać. Wszystkie obsługiwane
    // typy opędzane są za pomocą wersji specjalizowanych. Celowo pomijam
    // 'return' aby w razie użycia typu, dla którego nie ma wersji
    // specjalizowanej, kompilator zgłosił bład.
}

template <>
inline int getVal(lua_State* aLua, int aIdx)
{
    return luaL_checkint(aLua, aIdx);
}

template <>
inline float getVal(lua_State* aLua, int aIdx)
{
    return static_cast<float>(luaL_checknumber(aLua, aIdx));
}

template <>
inline const char* getVal(lua_State* aLua, int aIdx)
{
    return luaL_checkstring(aLua, aIdx);
}

template <>
inline std::string getVal(lua_State* aLua, int aIdx)
{
    return luaL_checkstring(aLua, aIdx);
}

template <>
inline bool getVal(lua_State* aLua, int aIdx)
{
    return lua_toboolean(aLua, aIdx);
}

template <typename T>
inline void pushVal(lua_State* aLua, T aVal)
{
    // patrz uwagi w 'getVal'
}

template <>
inline void pushVal(lua_State* aLua, int aVal)
{
    lua_pushinteger(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, float aVal)
{
    lua_pushnumber(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, const char* aVal)
{
    lua_pushstring(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, std::string aVal)
{
    lua_pushstring(aLua, aVal.c_str());
}

template <>
inline void pushVal(lua_State* aLua, bool aVal)
{
    lua_toboolean(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, void* aVal)
{
    lua_pushlightuserdata(aLua, aVal);
}

}
#endif // USERDATA_H
