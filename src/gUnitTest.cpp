#include "gUnitTest.h"

// props
DEFINE_GADGET_ACCESSOR(gUnitTest, getVal, setVal, iVal)
DEFINE_GADGET_WRITER(gUnitTest, notifyWhenEqualTo, iEqualToVal)

// methods
DEFINE_GADGET_METHOD_1(gUnitTest, isValGreater)

// events
DEFINE_GADGET_EVENT(gUnitTest, onEqual)

DEFINE_GADGET_API(gUnitTest)
{
    // props
    REGISTER_GADGET_ACCESSOR(getVal, setVal)
    REGISTER_GADGET_WRITER(notifyWhenEqualTo)

    // methods
    REGISTER_GADGET_METHOD(isValGreater)

    // events
    REGISTER_GADGET_EVENT(onEqual)
    {0, 0}
};

DEFINE_GADGET_CLASS(gUnitTest)

gUnitTest::gUnitTest():
    iVal(0)
{

}

int gUnitTest::update(lua_State* aLua, float /* aDelta */)
{
    ++iVal;

    if (iVal == iEqualToVal)
	return emit(aLua, onEqual);

    return 0;
}

bool gUnitTest::isValGreater(lua_State* aLua)
{
    int val = lua_tonumber(aLua, 2);
    return iVal > val;
}
