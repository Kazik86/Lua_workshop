#ifndef G_UNIT_TEST_H
#define G_UNIT_TEST_H

#include "gadget.h"

class gUnitTest: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gUnitTest();

    void update(lua_State* aLua, float aDelta);

    // props
    DECLARE_GADGET_ACCESSOR(getVal, setVal)
    DECLARE_GADGET_WRITER(notifyWhenEqualTo)

    // methods
    DECLARE_GADGET_METHOD(isValGreater)

    // events
    DECLARE_GADGET_EVENT(onEqual)

private:
    bool isValGreater(lua_State* aLua);

private:
    int iVal;
    int iEqualToVal;
};

#endif // G_UNIT_TEST_H
