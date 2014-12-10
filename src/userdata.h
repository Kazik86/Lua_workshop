#ifndef USERDATA_H
#define USERDATA_H

#include <lua.hpp>

template <typename T>
class eUserdata
{
private:                                       
    eUserdata();
    eUserdata(const eUserdata&);
    eUserdata& operator=(const eUserdata&);                                                   

public:
    ~eUserdata();

private:
    static int create(lua_State* aLua);
    static int cleanup(lua_State* aLua);
    static int setProperty(lua_State* aLua);

    static int luaOpen (lua_State* aLua);

private:
    static const struct luaL_Reg iFunctions[];
    static const struct luaL_Reg iMethods[];
    static const sPropertyInfo iProperties[];
};

template <typename T>
int eUserdata<T>::create(lua_State* aLua)
{
    size_t classSize = sizeof(T);
    T* g = (T*)lua_newuserdata(aLua, classSize);

    /* musze wywolac ctor m.in po to aby zainicjalizowac wskaznik VPTR */
    new((void*)g) T;
    g->bind();

    luaL_getmetatable(aLua, T::iClassName);
    lua_setmetatable(aLua, -2);

    if(lua_istable(aLua, 1))
	g->loadPropertiesFromTable(aLua, 1, T::iProperties);

    g->saveDefaultPropValues(aLua);
    g->begin(aLua);

    return 1;
}

template <typename T>
int eUserdata<T>::setProperty(lua_State* aLua)
{
    T* me = KPR_LUA_GET_USERDATA(aLua, 1, T);
    ASSERT(me != 0);

    int idx = me->checkOption(aLua, 2, T::iProperties);
    me->getValue(aLua, 3, T::iProperties, idx);
    me->iIsPropModified = true;
    return 0;
}

template <typename T>
int eUserdata<T>::cleanup(lua_State* aLua)
{
    T* me = KPR_LUA_GET_USERDATA(aLua, 1, T);
    ASSERT(me != 0);

    if(me->iIsEnabled)
	me->disable();

    me->~T();
    return 0;
}

template <typename T>
int eUserdata<T>::luaOpen(lua_State* aLua)
{
    luaL_newmetatable(aLua, T::iClassName);
    lua_pushstring(aLua, "__index");
    lua_pushvalue(aLua, -2);
    lua_settable(aLua, -3);

    lua_pushstring(aLua, "__gc");
    lua_pushcfunction(aLua, eUserdata<T>::cleanup);
    lua_settable(aLua, -3);

    lua_pushstring(aLua, "__newindex");
    lua_pushcfunction(aLua, eUserdata<T>::setProperty);
    lua_settable(aLua, -3);

    luaL_register(aLua, NULL, eUserdata<T>::iMethods);
    luaL_register(aLua, NULL, eUserdata<T>::iCommonMethods);
    luaL_register(aLua, T::iClassName, eUserdata<T>::iFunctions);

    lua_pop(aLua, 2);

    return 0;
}

template <typename T>
const struct luaL_Reg eUserdata<T>::iFunctions[] =
{
    {"create", eUserdata<T>::create},
    {NULL, NULL}
};

#endif // USERDATA_H
