#ifndef G_FOO_H
#define G_FOO_H

#include "gadget.h"

class gFoo: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    void update(lua_State* aLua);

    // props
    DECLARE_GADGET_PROPERTY(Val, int)
};

#endif // G_FOO_H
