#ifndef USERDATA_H
#define USERDATA_H

#include <lua.hpp>
#include <string>
#include <type_traits>

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


// property ///////////////////////////////////////////////////////////

#define DECLARE_USERDATA_READER(aName)     \
    static int aName(lua_State*);

#define DECLARE_USERDATA_WRITER(aName)     \
    static int aName(lua_State*);

#define DECLARE_USERDATA_ACCESSOR(aReader, aWriter)   \
    DECLARE_USERDATA_READER(aReader)                  \
    DECLARE_USERDATA_WRITER(aWriter)


#define DEFINE_USERDATA_READER_COMMON(aClass, aName, aVar, aGetFun)  \
    int aClass::aName(lua_State* aLua)                                      \
    {                                                                       \
	aClass* me = Script::aGetFun<aClass>(aLua);                         \
	Script::pushVal<decltype(me->aVar)>(aLua, me->aVar);                \
	return 1;                                                           \
    }

#define DEFINE_USERDATA_WRITER_COMMON(aClass, aName, aVar, aGetFun)  \
    int aClass::aName(lua_State* aLua)                                      \
    {                                                                       \
        aClass* me = Script::aGetFun<aClass>(aLua);                         \
        me->aVar = Script::getVal<decltype(me->aVar)>(aLua, 2);             \
	return 0;                                                           \
    }


#define DEFINE_USERDATA_READER(aClass, aName, aVar)		    \
    DEFINE_USERDATA_READER_COMMON(aClass, aName, aVar, getUdata)

#define DEFINE_USERDATA_WRITER(aClass, aName, aVar)		    \
    DEFINE_USERDATA_WRITER_COMMON(aClass, aName, aVar, getUdata)

#define DEFINE_USERDATA_ACCESSOR(aClass, aReader, aWriter, aVar)     \
    DEFINE_USERDATA_READER(aClass, aReader, aVar)                    \
    DEFINE_USERDATA_WRITER(aClass, aWriter, aVar)


#define REGISTER_USERDATA_READER(aName) { #aName, aName },
#define REGISTER_USERDATA_WRITER(aName) { #aName, aName },

#define REGISTER_USERDATA_ACCESSOR(aReader, aWriter)	    \
    REGISTER_USERDATA_READER(aReader)			    \
    REGISTER_USERDATA_WRITER(aWriter)


// method /////////////////////////////////////////////////////////////

#define DECLARE_USERDATA_METHOD(aName)		    \
    public:					    \
	static int aName##_lua(lua_State* aLua);    \
    private:

#define DEFINE_USERDATA_METHOD_0_COMMON(aClass, aName, aGetFun) \
    int aClass::aName##_lua(lua_State* aLua)			\
    {								\
	aClass* me = Script::aGetFun<aClass>(aLua);		\
	me->aName(aLua);					\
	return 0;						\
    }

#define DEFINE_USERDATA_METHOD_1_COMMON(aClass, aName, aGetFun) \
    int aClass::aName##_lua(lua_State* aLua)			\
    {								\
	aClass* me = Script::aGetFun<aClass>(aLua);		\
	Script::pushVal<std::result_of<decltype(&aClass::aName)(aClass, lua_State*)>::type>(aLua, me->aName(aLua));\
	return 1;						\
    }

#define DEFINE_USERDATA_METHOD_0(aClass, aName)	\
    DEFINE_USERDATA_METHOD_0_COMMON(aClass, aName, getUdata)

#define DEFINE_USERDATA_METHOD_1(aClass, aName)	\
    DEFINE_USERDATA_METHOD_1_COMMON(aClass, aName, getUdata)

#define REGISTER_USERDATA_METHOD(aName)	\
    {#aName, aName##_lua},


// return multiple values stuff ///////////////////////////////////////

class eLuaTable
{
    public:
        eLuaTable();
        ~eLuaTable();

    private:
        eLuaTable(const eLuaTable& aOther);
        eLuaTable& operator=(const eLuaTable& aOther);

    public:
        int getRef() const { return iRef; }

    private:
        int iRef;
};


// event //////////////////////////////////////////////////////////////

struct sEvent
{
    sEvent();
    ~sEvent();
    int iEventRef;
};

#define DECLARE_USERDATA_EVENT(aName)		    \
    public:					    \
	static int aName##_event(lua_State* aLua);  \
    private:					    \
	sEvent aName;

#define DEFINE_USERDATA_EVENT_COMMON(aClass, aName, aGetFun)	\
    int aClass::aName##_event(lua_State* aLua)			\
    {								\
	aClass* me = Script::aGetFun<aClass>(aLua);		\
								\
	luaL_checktype(aLua, 2, LUA_TFUNCTION);			\
	lua_pushvalue(aLua, 2);					\
								\
	int& eventRef = me->aName.iEventRef;			\
								\
	if (eventRef == LUA_NOREF)				\
	    eventRef = luaL_ref(aLua, LUA_REGISTRYINDEX);	\
	else							\
	    lua_rawseti(aLua, LUA_REGISTRYINDEX, eventRef);	\
								\
	return 0;						\
    }

#define DEFINE_USERDATA_EVENT(aClass, aName)	\
    DEFINE_USERDATA_EVENT_COMMON(aClass, aName, getUdata)

#define REGISTER_USERDATA_EVENT(aName)	\
    {#aName, aName##_event},

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
    // tu obsługuję przekazywanie gadżetów, pozostałe typy danych opędzane są
    // przez wersje specjalizowane
    return static_cast<T>(luaL_checkudata(aLua, aIdx, std::remove_pointer<T>::type::iClassName));
}

template <>
inline int getVal(lua_State* aLua, int aIdx)
{
    return luaL_checkint(aLua, aIdx);
}

template <>
inline unsigned int getVal(lua_State* aLua, int aIdx)
{
    return luaL_checkunsigned(aLua, aIdx);
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
inline void pushVal(lua_State* aLua, const T& aVal)
{
    // z tego szablonu nigdy nie powinienem korzystać. Wszystkie obsługiwane
    // typy opędzane są za pomocą wersji specjalizowanych. Celowo poniżej dodaję
    // 'return' aby w razie użycia typu, dla którego nie ma wersji
    // specjalizowanej, kompilator zgłosił bład.
    return 1;
}

template <>
inline void pushVal(lua_State* aLua, const int& aVal)
{
    lua_pushinteger(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, const unsigned int& aVal)
{
    lua_pushunsigned(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, const float& aVal)
{
    lua_pushnumber(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, const char* const& aVal)
{
    lua_pushstring(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, const std::string& aVal)
{
    lua_pushstring(aLua, aVal.c_str());
}

template <>
inline void pushVal(lua_State* aLua, const bool& aVal)
{
    lua_pushboolean(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, void* const& aVal)
{
    lua_pushlightuserdata(aLua, aVal);
}

template <>
inline void pushVal(lua_State* aLua, const eLuaTable& aVal)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aVal.getRef());
}

template <typename T>
void setField(lua_State* aLua, const eLuaTable& aTable, const char* aField, const T& aVal)
{
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aTable.getRef());
    lua_pushstring(aLua, aField);
    pushVal(aLua, aVal);
    lua_rawset(aLua, -3);
    lua_pop(aLua, 1);
}

}
#endif // USERDATA_H
