#include "gFoo.h"

namespace
{
    DEFINE_GADGET_PROPERTY(gFoo, Val, int)
}

DEFINE_GADGET_API(gFoo)
{
    REGISTER_GADGET_PROPERTY(Val),
    {0, 0}
};

DEFINE_GADGET_CLASS(gFoo)

void gFoo::update(lua_State* aLua)
{
    ++iVal;
}
