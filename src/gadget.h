#ifndef GADGET_H
#define GADGET_H

#include "userdata.h"

#include <new>

#define DEFINE_GADGET_API(aClass) \
    DEFINE_USERDATA_API(aClass)


// property ///////////////////////////////////////////////////////////

#define DECLARE_GADGET_READER(aName) \
    DECLARE_USERDATA_READER(aName)

#define DECLARE_GADGET_WRITER(aName) \
    DECLARE_USERDATA_WRITER(aName)

#define DECLARE_GADGET_ACCESSOR(aReader, aWriter)    \
    DECLARE_GADGET_READER(aReader)		    \
    DECLARE_GADGET_WRITER(aWriter)

#define DEFINE_GADGET_READER(aClass, aName, aVar)		    \
    DEFINE_USERDATA_READER_COMMON(aClass, aName, aVar, getGadget)

#define DEFINE_GADGET_WRITER(aClass, aName, aVar)		    \
    DEFINE_USERDATA_WRITER_COMMON(aClass, aName, aVar, getGadget)

#define DEFINE_GADGET_ACCESSOR(aClass, aReader, aWriter, aVar)	    \
    DEFINE_GADGET_READER(aClass, aReader, aVar)			    \
    DEFINE_GADGET_WRITER(aClass, aWriter, aVar)			    \

#define REGISTER_GADGET_READER(aName)    \
    REGISTER_USERDATA_READER(aName)

#define REGISTER_GADGET_WRITER(aName)    \
    REGISTER_USERDATA_WRITER(aName)

#define REGISTER_GADGET_ACCESSOR(aReader, aWriter)   \
    REGISTER_GADGET_READER(aReader)		    \
    REGISTER_GADGET_WRITER(aWriter)

// method /////////////////////////////////////////////////////////////

#define DECLARE_GADGET_METHOD(aName) \
    DECLARE_USERDATA_METHOD(aName)

#define DEFINE_GADGET_METHOD(aClass, aName)	\
    DEFINE_USERDATA_METHOD_1_COMMON(aClass, aName, getGadget)

#define REGISTER_GADGET_METHOD(aName)	\
    REGISTER_USERDATA_METHOD(aName)


// event //////////////////////////////////////////////////////////////

#define DECLARE_GADGET_EVENT(aName) \
    DECLARE_USERDATA_EVENT(aName)

#define DEFINE_GADGET_EVENT(aClass, aName)  \
    DEFINE_USERDATA_EVENT_COMMON(aClass, aName, getGadget)

#define REGISTER_GADGET_EVENT(aName)	\
    REGISTER_USERDATA_EVENT(aName)


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
    luaL_checktype(aLua, 1, LUA_TTABLE);				\
    lua_getfield(aLua, 1, "eActor");					\
    eActor* owner = static_cast<eActor*>(lua_touserdata(aLua, -1));	\
    lua_pop(aLua, 1);							\
									\
    size_t classSize = sizeof(aClass);                                  \
    aClass* g = static_cast<aClass*>(lua_newuserdata(aLua, classSize)); \
    new((void*)g) aClass();                                             \
    g->iActor = owner;							\
    owner->addGadget(g);						\
									\
    luaL_getmetatable(aLua, #aClass);					\
    lua_setmetatable(aLua, -2);						\
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

class eActor;
class sEvent;

class eGadget
{
public:
    bool isEnabled() const { return iIsEnabled; }
    virtual void update(lua_State* /* aLua */, float /* aDelta */) {}
    virtual void restart() {}
    virtual void begin() {}

protected:
    eGadget();
    virtual ~eGadget() {}

    static void registerCommonMethods(lua_State* aLua);
    eActor* getActor() { return iActor; }
    void emit(lua_State* aLua, const sEvent& aEvent);

private:
    eGadget(const eGadget& aOther);
    eGadget& operator=(const eGadget& aOther);

    static int enable(lua_State* aLua);
    static int disable(lua_State* aLua);

protected:
    virtual void enable() {}
    virtual void disable() {}

protected:
    eActor* iActor;
    bool iIsEnabled;

private:
    static const struct luaL_Reg iCommonMethods[];
};

#endif // GADGET_H
