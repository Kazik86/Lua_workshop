#ifndef GADGET_H
#define GADGET_H

#include "userdata.h"

#include <new>

#define DECLARE_GADGET_PROPERTY(aName, aType) \
    DECLARE_USERDATA_PROPERTY(aName, aType)

#define DEFINE_GADGET_PROPERTY(aClass, aName, aType) \
    DEFINE_USERDATA_PROPERTY_COMMON(aClass, aName, aType, getGadget)

#define DEFINE_GADGET_API(aClass) \
    DEFINE_USERDATA_API(aClass)

#define REGISTER_GADGET_PROPERTY(aName)    \
    REGISTER_USERDATA_PROPERTY(aName)

#define DECLARE_GADGET_CLASS()                                      \
public:								    \
    static int luaOpen (lua_State* aLua);			    \
    static const char* iClassName;				    \
								    \
private:                                                            \
    static int create(lua_State* aLua);                             \
    static int cleanup(lua_State* aLua);                            \
								    \
private:                                                            \
    static const struct luaL_Reg iFunctions[];                      \
    static const struct luaL_Reg iMethods[];


#define DEFINE_GADGET_CLASS(aClass)                                     \
									\
const char* aClass::iClassName = #aClass;				\
									\
int aClass::create(lua_State* aLua)                                     \
{                                                                       \
    size_t classSize = sizeof(aClass);                                  \
    aClass* g = static_cast<aClass*>(lua_newuserdata(aLua, classSize)); \
    new((void*)g) aClass;                                               \
									\
    luaL_getmetatable(aLua, #aClass);					\
    lua_setmetatable(aLua, -2);						\
									\
    g->begin(aLua);                                                     \
									\
    return 1;								\
}									\
									\
int aClass::cleanup(lua_State* aLua)                                    \
{                                                                       \
    aClass* me = Script::getGadget<aClass>(aLua);       	        \
									\
    if(me->iIsEnabled)							\
	me->disable();							\
									\
    me->~aClass();							\
    return 0;								\
}									\
									\
const struct luaL_Reg aClass::iFunctions[] =				\
{									\
    {"create", aClass::create},						\
    {NULL, NULL}							\
};									\
									\
int aClass::luaOpen(lua_State* aLua)                                    \
{                                                                       \
    luaL_newmetatable(aLua, #aClass);					\
    lua_pushstring(aLua, "__index");					\
    lua_pushvalue(aLua, -2);						\
    lua_settable(aLua, -3);						\
									\
    lua_pushstring(aLua, "__gc");					\
    lua_pushcfunction(aLua, aClass::cleanup);				\
    lua_settable(aLua, -3);						\
									\
    luaL_setfuncs(aLua, aClass::iMethods, 0);				\
    registerCommonMethods(aLua);					\
    lua_pop(aLua, 1);							\
    luaL_newlib(aLua, aClass::iFunctions);				\
									\
    return 1;								\
}

class eGadget
{
public:
    bool isEnabled() const { return iIsEnabled; }

protected:
    eGadget();
    virtual ~eGadget() {}

    static void registerCommonMethods(lua_State* aLua);

private:
    eGadget(const eGadget& aOther);
    eGadget& operator=(const eGadget& aOther);

    static int enable(lua_State* aLua);
    static int disable(lua_State* aLua);

protected:
    virtual void update(lua_State* /* aLua */) {}
    virtual void restart() {}
    virtual void begin(lua_State* /* aLua */) {}
    virtual void enable() {}
    virtual void disable() {}

protected:
    bool iIsEnabled;

private:
    static const struct luaL_Reg iCommonMethods[];
};

#endif // GADGET_H
