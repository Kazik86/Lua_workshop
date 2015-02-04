#include "gUnitTest.h"

#include "actor.h"

DEFINE_GADGET_ACCESSOR(gUnitTest, getVal, setVal, iVal)

DEFINE_GADGET_API(gUnitTest)
{
    REGISTER_GADGET_ACCESSOR(getVal, setVal)
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
