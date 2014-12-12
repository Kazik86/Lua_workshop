#ifndef GADGET_H
#define GADGET_H

#include "userdata.h"

#define DECLARE_GADGET_PROPERTY(aName, aType) \
    DECLARE_USERDATA_PROPERTY(aName, aType)

#define DEFINE_GADGET_PROPERTY(aClass, aName, aType) \
    DEFINE_USERDATA_PROPERTY_COMMON(aClass, aName, aType, getGadget)

#define DEFINE_GADGET_API(aClass) \
    DEFINE_USERDATA_API(aClass)


#define DECLARE_GADGET_CLASS(aClass)                                \
private:                                                            \
    aClass();                                                       \
    aClass(const aClass&);                                          \
    aClass& operator=(const aClass&);                               \
								    \
public:                                                             \
    ~aClass();                                                      \
								    \
private:                                                            \
    static int create(lua_State* aLua);                             \
    static int cleanup(lua_State* aLua);                            \
								    \
    static int luaOpen (lua_State* aLua);                           \
								    \
private:                                                            \
    static const struct luaL_reg iFunctions[];                      \
    static const struct luaL_reg iMethods[];


class eGadget
{
protected:
    eGadget();
    virtual ~eGadget();

private:
    eGadget(const eGadget& aOther);
    eGadget& operator=(const eGadget& aOther);

protected:
    virtual void update(lua_State* aLua);
    virtual void restart();
    virtual void begin(lua_State* aLua);
    virtual void enable();
    virtual void disable();
};

#endif // GADGET_H
