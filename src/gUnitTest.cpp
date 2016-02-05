#include "gUnitTest.h"

// props
DEFINE_GADGET_ACCESSOR(gUnitTest, getVal, setVal, iVal)
DEFINE_GADGET_WRITER(gUnitTest, notifyWhenEqualTo, iEqualToVal)

// methods
DEFINE_GADGET_METHOD_1(gUnitTest, isValGreater)

// events
DEFINE_GADGET_EVENT(gUnitTest, onEqual)
DEFINE_GADGET_EVENT(gUnitTest, onEven)
DEFINE_GADGET_EVENT(gUnitTest, onGreater)

DEFINE_GADGET_API(gUnitTest)
{
    // props
    REGISTER_GADGET_ACCESSOR(getVal, setVal)
    REGISTER_GADGET_WRITER(notifyWhenEqualTo)

    // methods
    REGISTER_GADGET_METHOD(isValGreater)

    // events
    REGISTER_GADGET_EVENT(onEqual)
    REGISTER_GADGET_EVENT(onEven)
    REGISTER_GADGET_EVENT(onGreater)
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

    if (iVal % 2 == 0)
        return emit(aLua, onEven, iVal);

    if (iVal == iEqualToVal)
	return emit(aLua, onEqual);

    if (iVal > iEqualToVal) {
        Script::setField(aLua, iOnGreaterReturn, "currentVal", iVal);
        Script::setField(aLua, iOnGreaterReturn, "delta", iVal - iEqualToVal);

        return emit(aLua, onGreater, iOnGreaterReturn);
    }

    return 0;
}

bool gUnitTest::isValGreater(lua_State* aLua)
{
    int val = lua_tonumber(aLua, 2);
    return iVal > val;
}
