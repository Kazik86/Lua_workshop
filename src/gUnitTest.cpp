#include "gUnitTest.h"

#include "actor.h"

#include <type_traits>

DEFINE_GADGET_ACCESSOR(gUnitTest, getVal, setVal, iVal)
DEFINE_GADGET_METHOD(gUnitTest, isValGreater)

DEFINE_GADGET_API(gUnitTest)
{
    REGISTER_GADGET_ACCESSOR(getVal, setVal)
    REGISTER_GADGET_METHOD(isValGreater)
    {0, 0}
};

DEFINE_GADGET_CLASS(gUnitTest)

gUnitTest::gUnitTest():
    iVal(0)
{

}

void gUnitTest::update(lua_State* /* aLua */, float /* aDelta */)
{
    ++iVal;
}

bool gUnitTest::isValGreater(lua_State* aLua)
{
    int val = lua_tonumber(aLua, 2);
    return iVal > val;
}
